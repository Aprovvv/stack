#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "stack.h"

const int START_CAPACITY = 4;

struct stack_t {

    size_t elem_size;
    void* data;
    int size;
    int capacity;
};

struct stack_t* stack_init(size_t elem_size)
{
    struct stack_t* stk = (struct stack_t*)calloc(1, sizeof(struct stack_t));
    if (stk == NULL)
    {
        printf("ERROR: enable to allocate memory for"
               "struct stack_t: %s\n", strerror(errno));
        return NULL;
    }
    stk->data = calloc(elem_size, START_CAPACITY);
    if (stk == NULL)
    {
        printf("ERROR: enable to allocate memory"
               "for data: %s\n", strerror(errno));
        return NULL;
    }
    stk->capacity = START_CAPACITY;
    stk->size = 0;
    stk->elem_size = elem_size;
    return stk;
}

int stack_destroy(stack_t* stk)
{
    STACK_ASSERT(stk);
    if (int err = stack_error(stk))
        return err;
    free(stk->data);
    free(stk);
    return 0;
}

int stack_printf(stack_t* stk)
{
    STACK_ASSERT(stk);
    if (int err = stack_error(stk))
        return err;
    printf("size = %d; &size = %p\n", stk->size, &(stk->size));
    printf("capasity = %d; &capasity = %p\n",
            stk->capacity, &(stk->capacity));
    printf("elem_size = %lu, &elem_size = %p\n",
            stk->elem_size, &stk->elem_size);
    printf("data = %p, &data = %p\n", stk->data, &stk->data);
    for (int i = 0; i < stk->capacity; i++)
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
    printf("\n");
    return 0;
}

int stack_push(stack_t* stk, void* p)
{
    STACK_ASSERT(stk);
    if (int err = stack_error(stk))
        return err;
    if (stk->size + 1 > stk->capacity)
    {
        stk->capacity *= 2;
        stk->data = realloc(stk->data,
                    (size_t)stk->capacity*stk->elem_size);
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
    return 1;
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
    if (stk->size <= stk->capacity/4 &&
        stk->capacity >= 2*START_CAPACITY)
    {
        stk->capacity /= 2;
        stk->data = realloc(stk->data,
                    (size_t)stk->capacity*stk->elem_size);
    }
    return 1;
}

void stack_assert(struct stack_t* stk, const char* file, int line)
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
        break;
    case 2:
        printf("ERROR: stk->data is a null pointer "
               "in %s:%d\n", file, line);
        break;
    case 3:
        printf("ERROR: stk->size < 0 "
               "in %s:%d\n", file, line);
        break;
    case 4:
        printf("ERROR: capacity < size "
               "in %s:%d\n", file, line);
        break;
    default:
        printf("UNDEFINED ERROR\n");
        break;
    }
}

int stack_error(struct stack_t* stk)
{
    if (stk == NULL)
        return 1;
    if (stk->data == NULL)
        return 2;
    if (stk->size < 0)
        return 3;
    if (stk->capacity < stk->size)
        return 4;
    return 0;
}
