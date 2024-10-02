#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "stack.h"

typedef unsigned long canary_t;

const int START_CAPACITY = 4;
const canary_t CANARY = 0XFEE1DEAD;

struct stack_t {
    canary_t left_str_protect;
    size_t elem_size;
    void* data;
    int size;
    size_t capacity;
    unsigned long str_hash;
    unsigned long data_hash;
    canary_t right_str_protect;
};

static void stack_assert(struct stack_t* stk, const char* file, int line);
static void set_hash(struct stack_t* stk);

static unsigned long hash(void* str, size_t size);

static int data_hash_ok(struct stack_t* stk);
static int str_hash_ok(struct stack_t* stk);
static int stack_error(struct stack_t* stk);

static void* resize(void* ptr,
                    size_t new_capacity,
                    size_t old_capacity,
                    size_t elem_size);

struct stack_t* stack_init(size_t elem_size)
{
    struct stack_t* stk =
        (struct stack_t*)calloc(1, sizeof(struct stack_t));
    if (stk == NULL)
    {
        printf("ERROR: enable to allocate memory for"
               "struct stack_t: %s\n", strerror(errno));
        return NULL;
    }
    stk->data = malloc(elem_size*START_CAPACITY + 2*sizeof(long));
    stk->data = (long*)stk->data + 1;
    if (stk->data == NULL)
    {
        printf("ERROR: enable to allocate memory"
               "for data: %s\n", strerror(errno));
        free(stk);
        return NULL;
    }
    stk->left_str_protect = CANARY;
    stk->right_str_protect = CANARY;
    stk->capacity = START_CAPACITY;
    stk->size = 0;
    stk->elem_size = elem_size;
    *((canary_t*)stk->data - 1) = CANARY;
    *(canary_t*)((char*)stk->data +
                      stk->elem_size*START_CAPACITY) = CANARY;
    set_hash(stk);

    STACK_ASSERT(stk);
    return stk;
}

int stack_destroy(stack_t* stk)
{
    STACK_ASSERT(stk);
    if (int err = stack_error(stk))
        return err;
    free((long*)stk->data - 1);
    free(stk);
    return 0;
}

int stack_printf(stack_t* stk)
{
    printf("\n-----------------------------------\n\n");
    STACK_ASSERT(stk);
    if (int err = stack_error(stk))
        return err;
    canary_t left_canary =
        *((canary_t*)stk->data - 1);
    canary_t right_canary =
        *((canary_t*)((char*)stk->data +
                           (size_t)stk->capacity*stk->elem_size));

    printf("left_str_protect = %#lX\n", stk->left_str_protect);
    printf("right_str_protect = %#lX\n", stk->right_str_protect);
    printf("left_data_protect = %#lX\n", left_canary);
    printf("right_data_protect = %#lX\n", right_canary);

    printf("str_hash = %#lX\n", stk->str_hash);
    printf("data_hash = %#lX\n\n", stk->data_hash);

    printf("size = %d; &size = %p\n", stk->size, &(stk->size));
    printf("capasity = %lu; &capasity = %p\n",
            stk->capacity, &(stk->capacity));
    printf("elem_size = %lu, &elem_size = %p\n\n",
            stk->elem_size, &stk->elem_size);
    printf("data = %p, &data = %p\n", stk->data, &stk->data);

    for (int i = 0; i < (int)stk->capacity; i++)
    {
        if (i < stk->size)
            printf("*%d = %f\n", i,
                    *(double*)((char*)stk->data +
                    (size_t)i*stk->elem_size));
        else
            printf(" %d = %f\n", i,
                    *(double*)((char*)stk->data +
                    (size_t)i*stk->elem_size));
    }
    printf("\n-----------------------------------\n\n");
    return 0;
}

int stack_push(stack_t* stk, void* p)
{
    STACK_ASSERT(stk);
    if (int err = stack_error(stk))
        return err;

    if ((size_t)(stk->size + 1) > stk->capacity)
    {
        stk->data = resize(stk->data,
                           stk->capacity*2,
                           stk->capacity,
                           stk->elem_size);
        stk->capacity *= 2;
        if (stk->data == NULL)
        {
            printf("ERROR: unable to reallocate "
                   "memory for data\n");
            return 0;
        }
    }

    memcpy((char*)stk->data +
           (size_t)(stk->size)*(stk->elem_size),
           p, stk->elem_size);
    stk->size++;
    set_hash(stk);
    return 0;
}

int stack_pop(stack_t* stk, void* p)
{
    STACK_ASSERT(stk);
    if (int err = stack_error(stk))
        return err;

    memcpy(p, (char*)stk->data +
          (size_t)(stk->size - 1)*(stk->elem_size),
          stk->elem_size);
    stk->size--;

    if ((size_t)(stk->size) <= stk->capacity/4 &&
        stk->capacity >= 2*START_CAPACITY)
    {
        stk->data = resize(stk->data,
                           stk->capacity/2,
                           stk->capacity,
                           stk->elem_size);
        stk->capacity /= 2;
    }
    set_hash(stk);
    return 0;
}

static void* resize(void* ptr,
             size_t new_capacity,
             size_t old_capacity,
             size_t elem_size)
{
    ptr = (long*)ptr - 1;
    ptr = realloc(ptr, new_capacity*elem_size + 2*sizeof(long));
    if (ptr == NULL)
    {
        printf("ERROR: unable to reallocate "
               "memory for data: %s\n", strerror(errno));
    }
    *(canary_t*)((char*)ptr +
                 sizeof(long) +
                 (size_t)new_capacity*elem_size) = CANARY;
    return (long*)ptr + 1;
}

static void stack_assert(struct stack_t* stk, const char* file, int line)
{
    int n = stack_error(stk);
    if (n)
        printf("ASSERT triggered "
            "in %s:%d\n", file, line);
    switch (n)
    {
    case 0:
        break;
    case 1:
        printf("ERROR: stk is a null pointer "
               "in %s:%d\n", file, line);
        abort();
    case 2:
        printf("ERROR: stk->data is a null pointer "
               "in %s:%d\n", file, line);
        abort();
    case 3:
        printf("ERROR: stk->size < 0 "
               "in %s:%d\n", file, line);
        abort();
    case 4:
        printf("ERROR: capacity < size "
               "in %s:%d\n", file, line);
        abort();
    case 5:
        printf("ERROR: left_str_protect = %lX "
               "in %s:%d\n", stk->left_str_protect, file, line);
        abort();
    case 6:
        printf("ERROR: right_str_protect = %lX "
               "in %s:%d\n", stk->right_str_protect, file, line);
        abort();
    case 7:
        printf("ERROR: right_data_protect = %lX "
               "in %s:%d\n",
               *((canary_t*)stk->data - 1),
               file, line);
        abort();
    case 8:
        printf("ERROR: right_data_protect = %lX "
               "in %s:%d\n",
               *((canary_t*)((char*)stk->data +
                                  (size_t)stk->capacity*stk->elem_size)),
                                  file, line);
        abort();
    case 9:
        printf("ERROR: str_hash has wrong value "
                "in %s:%d\n", file, line);
        break;
    case 10:
        printf("ERROR: data_hash has wrong value "
                "in %s:%d\n", file, line);
        break;
    default:
        printf("UNDEFINED ERROR\n");
        abort();
    }
}

static int stack_error(struct stack_t* stk)
{
    if (stk == NULL)
        return 1;
    if (stk->left_str_protect != CANARY)
        return 5;
    if (stk->right_str_protect != CANARY)
        return 6;
    if (stk->data == NULL)
        return 2;
    canary_t left_canary =
        *((canary_t*)stk->data - 1);
    canary_t right_canary =
        *((canary_t*)((char*)stk->data +
                      (size_t)stk->capacity*stk->elem_size));
    if (left_canary != CANARY)
        return 7;
    if (right_canary != CANARY)
        return 8;
    if (!str_hash_ok(stk))
        return 9;
    if (!data_hash_ok(stk))
        return 10;
    if (stk->size < 0)
        return 3;
    if ((int)stk->capacity < stk->size)
        return 4;
    return 0;
}

static unsigned long hash(void* str, size_t size)
{
    unsigned char* p = (unsigned char*)str;
    unsigned long hash = 5381;
    long unsigned int c;

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
    stk->data_hash = hash((long*)stk->data - 1,
                          stk->capacity*stk->elem_size + 2*sizeof(long));
}

static int data_hash_ok(struct stack_t* stk)
{
    return stk->data_hash ==
        hash((long*)stk->data - 1,
            stk->capacity*stk->elem_size + 2*sizeof(long));
}

static int str_hash_ok(struct stack_t* stk)
{
    return stk->str_hash ==
        hash(&stk->elem_size,
             (char*)&stk->str_hash -
             (char*)&stk->data);
}
