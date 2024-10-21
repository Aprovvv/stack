#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include "stack.h"
#include "color_print/color_print.h"
//TODO recalloc
//TODO static library

typedef uint64_t canary_t;

struct stack_t {
#ifndef NDEBUG
    canary_t left_str_protect;
#endif
    size_t elem_size;
    size_t start_capacity;
    void* data;
    size_t size;
    size_t capacity;
#ifndef NDEBUG
    unsigned long str_hash;
    unsigned long data_hash;
    canary_t right_str_protect;
#endif
};
enum ERROR_CODES {
    NO_ERROR,
    STK_NULL,
    STK_DATA_NULL,
    LEFT_STR_CANARY,
    RIGHT_STR_CANARY,
    LEFT_DATA_CANARY,
    RIGHT_DATA_CANARY,
    CAPAC_LESS_SIZE,
    STR_HASH,
    DATA_HASH,
    UNDEFINED,
};


const int DEFAULT_CAPACITY = 4;
const int REALLOC_COEF = 2;
#ifndef NDEBUG
const canary_t CANARY = 0XFEE1DEADL;
#endif

#ifndef NDEBUG
static void stack_assert(const struct stack_t* stk,
                         const char* file,
                         int line);
static void set_hash(struct stack_t* stk);

static unsigned long hash(const void* str, size_t size);

static int data_hash_ok(const struct stack_t* stk);
static int str_hash_ok(const struct stack_t* stk);
static int stack_error(const struct stack_t* stk);
#endif

static void* resize(void* ptr,
                    size_t new_capacity,
                    size_t old_capacity,
                    size_t elem_size);


struct stack_t* stack_init(size_t elem_size, size_t start_capacity)
{
    struct stack_t* stk =
        (struct stack_t*)calloc(1, sizeof(struct stack_t));
    if (stk == NULL)
        return NULL;
    size_t necessary_size = elem_size*start_capacity + 2*sizeof(canary_t);
    if (necessary_size%8 != 0)
        necessary_size += 8 - necessary_size%8;
    stk->data = malloc(elem_size*start_capacity + 2*sizeof(canary_t));
#ifndef NDEBUG
    stk->data = (canary_t*)stk->data + 1;
#else
    stk->data = (canary_t*)stk->data;
#endif
    if (stk->data == NULL)
    {
        free(stk);
        return NULL;
    }
    stk->capacity = start_capacity;
    stk->start_capacity = start_capacity;
    stk->size = 0;
    stk->elem_size = elem_size;

#ifndef NDEBUG
    stk->left_str_protect = CANARY;
    stk->right_str_protect = CANARY;
    *((canary_t*)stk->data - 1) = CANARY;
    memcpy((char*)stk->data + stk->elem_size*start_capacity,
           &CANARY,
           sizeof(canary_t));
    set_hash(stk);
#endif

    STACK_ASSERT(stk);
    return stk;
}

int stack_destroy(stack_t* stk)
{
    free((canary_t*)stk->data - 1);
    free(stk);
    return 0;
}

int stack_printf(stack_t* stk, int(*print_func)(const void*))
{
    fprintf(stderr, "\n-----------------------------------\n\n");
#ifndef NDEBUG
    if (int err = stack_error(stk))
        return err;

    canary_t left_canary =
        *((canary_t*)stk->data - 1);
    canary_t right_canary = 0;
    memcpy(&right_canary,
           (char*)stk->data + stk->capacity*stk->elem_size,
           sizeof(canary_t));

    fprintf(stderr, "left_str_canary = %#lX\n", stk->left_str_protect);
    fprintf(stderr, "right_str_canary = %#lX\n", stk->right_str_protect);
    fprintf(stderr, "left_data_canary = %#lX\n", left_canary);
    fprintf(stderr, "right_data_canary = %#lX\n", right_canary);

    fprintf(stderr, "str_hash = %#lX\n", stk->str_hash);
    fprintf(stderr, "data_hash = %#lX\n\n", stk->data_hash);
#endif

    fprintf(stderr, "size = %zu; &size = %p\n", stk->size, &(stk->size));
    fprintf(stderr, "capasity = %zu; &capasity = %p\n",
            stk->capacity, &(stk->capacity));
    fprintf(stderr, "elem_size = %zu, &elem_size = %p\n\n",
            stk->elem_size, &stk->elem_size);
    fprintf(stderr, "data = %p, &data = %p\n", stk->data, &stk->data);

    for (size_t i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size)
        {
            fprintf(stderr, "*%zu = ", i);
            print_func((char*)stk->data + i*stk->elem_size);
            fprintf(stderr, "\n");
        }
        else
        {
            fprintf(stderr, "%zu = ", i);
            print_func((char*)stk->data + i*stk->elem_size);
            fprintf(stderr, "\n");
        }
    }
    fprintf(stderr, "\n-----------------------------------\n\n");
    STACK_ASSERT(stk);
    return 0;
}

int stack_push(stack_t* stk, void* p)
{
    STACK_ASSERT(stk);

    if ((stk->size + 1) > stk->capacity)
    {
        void* temp_p = resize(stk->data,
                              stk->capacity*REALLOC_COEF,
                              stk->capacity,
                              stk->elem_size);
        if (temp_p != NULL)
            stk->data = temp_p;
        else
            return 1;
        stk->capacity *= REALLOC_COEF;
    }

    memcpy((char*)stk->data +
           (stk->size)*(stk->elem_size),
           p, stk->elem_size);
    stk->size++;
#ifndef NDEBUG
    set_hash(stk);
#endif
    return 0;
}

int stack_pop(stack_t* stk, void* p)
{
    STACK_ASSERT(stk);

    memcpy(p, (char*)stk->data +
          (stk->size - 1)*(stk->elem_size),
          stk->elem_size);
    stk->size--;

    if (stk->size <= stk->capacity/REALLOC_COEF/REALLOC_COEF &&
        stk->capacity >= REALLOC_COEF*stk->start_capacity)
    {
        void* temp_p = resize(stk->data,
                              stk->capacity/REALLOC_COEF,
                              stk->capacity,
                              stk->elem_size);
        if (temp_p != NULL)
            stk->data = temp_p;
        else
            return 1;
        stk->capacity /= REALLOC_COEF;
    }
#ifndef NDEBUG
    set_hash(stk);
#endif
    return 0;
}

void stack_view(struct stack_t* stk, size_t n, void* dest)
{
    memcpy(dest, (char*)stk->data + n*stk->elem_size, stk->elem_size);
}

static void* resize(void* ptr,
             size_t new_capacity,
             size_t old_capacity,
             size_t elem_size)
{
#ifndef NDEBUG
    ptr = (canary_t*)ptr - 1;
    memset((char*)ptr +
           sizeof(canary_t) +
           old_capacity*elem_size,
           0, sizeof(canary_t));
#else
    ptr = (canary_t*)ptr;
#endif
    ptr = realloc(ptr, new_capacity*elem_size + 2*sizeof(canary_t));
    if (ptr == NULL)
        return NULL;
#ifndef NDEBUG
    memcpy((char*)ptr +
           sizeof(canary_t) +
           new_capacity*elem_size,
           &CANARY, sizeof(canary_t));
    return (canary_t*)ptr + 1;
#else
    return (canary_t*)ptr;
#endif
}

#ifndef NDEBUG
static int stack_error(const struct stack_t* stk)
{
    if (stk == NULL)
        return STK_NULL;
    if (stk->left_str_protect != CANARY)
        return LEFT_STR_CANARY;
    if (stk->right_str_protect != CANARY)
        return RIGHT_STR_CANARY;
    if (stk->data == NULL)
        return STK_DATA_NULL;
    canary_t left_canary =
        *((canary_t*)stk->data - 1);
    canary_t right_canary = 0;
    memcpy(&right_canary,
           (char*)stk->data + stk->capacity*stk->elem_size,
           sizeof(canary_t));
    if (left_canary != CANARY)
        return LEFT_DATA_CANARY;
    if (right_canary != CANARY)
        return RIGHT_DATA_CANARY;
    if (!str_hash_ok(stk))
        return STR_HASH;
    if (!data_hash_ok(stk))
        return DATA_HASH;
    if (stk->capacity < stk->size)
        return CAPAC_LESS_SIZE;
    return NO_ERROR;
}

static void stack_assert(const struct stack_t* stk,
                         const char* file,
                         int line)
{
    int n = stack_error(stk);
    canary_t curr_canary = 0;
    if (n)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                     "ASSERT triggered in %s:%d\n",
                     file, line);
    }

    switch (n)
    {
    case NO_ERROR:
        break;
    case STK_NULL:
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: stk is a null pointer\n");
        abort();
    case STK_DATA_NULL:
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: stk->data is a null pointer\n");
        abort();
    case CAPAC_LESS_SIZE:
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: capacity < size\n");
        abort();
    case LEFT_STR_CANARY:
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: left_str_canary = %lX\n",
                      stk->left_str_protect);
        abort();
    case RIGHT_STR_CANARY:
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: right_str_canary = %lX\n",
                      stk->right_str_protect);
        abort();
    case LEFT_DATA_CANARY:
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: left_data_canary = %lX\n",
                      *((canary_t*)stk->data - 1));
        abort();
    case RIGHT_DATA_CANARY:
        memcpy(&curr_canary,
              (char*)stk->data +
              stk->capacity*stk->elem_size,
              sizeof(canary_t));
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: right_data_canary = %lX\n",
                      curr_canary);
        abort();
    case STR_HASH:
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: str_hash has wrong value\n");
        abort();
    case DATA_HASH:
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: data_hash has wrong value\n");
        abort();
    default:
        fprintf_color(stderr, CONSOLE_TEXT_RED, "UNDEFINED ERROR\n");
        abort();
    }
}

static unsigned long hash(const void* str, size_t size)
{
    const unsigned char* p = (const unsigned char*)str;
    unsigned long hash = 5381;
    unsigned long c;

    for (size_t i = 0; i < size; i++)
    {
        c = *(p+i);
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

static void set_hash(struct stack_t* stk)
{
    stk->str_hash = hash(&stk->elem_size,
                         (char*)&stk->str_hash -
                         (char*)&stk->data);
    stk->data_hash = hash((canary_t*)stk->data - 1,
                          stk->capacity*stk->elem_size + 2*sizeof(canary_t));
}

static int data_hash_ok(const struct stack_t* stk)
{
    return stk->data_hash ==
        hash((canary_t*)stk->data - 1,
            stk->capacity*stk->elem_size + 2*sizeof(canary_t));
}

static int str_hash_ok(const struct stack_t* stk)
{
    return stk->str_hash ==
        hash(&stk->elem_size,
             (const char*)&stk->str_hash -
             (const char*)&stk->data);
}
#endif
