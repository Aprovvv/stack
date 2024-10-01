#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

const int START_CAPASITY = 4;

int stack_init(struct stack_t* stk)
{
    if (stk == NULL)
    {
        printf("ERROR: argument is a NULL pointer\n");
        return 1;
    }
    stk->data = (stack_elem_t*)calloc(sizeof(stack_elem_t),
                START_CAPASITY);
    if (stk->data == NULL)
    {
        printf("ERROR: unable to allocate memory "
        "for data\n");
        return 1;
    }
    stk->capacity = START_CAPASITY;
    stk->size = 0;
    return 0;
}

void stack_destroy(stack_t* stk)
{
    STACK_ASSERT(stk);
    free(stk->data);
    stk->capacity = 0;
    stk->size = 0;
}

void stack_printf(stack_t* stk)
{
    STACK_ASSERT(stk);
    printf("size = %d; &size = %p\n", stk->size, &(stk->size));
    printf("capasity = %d; &capasity = %p\n", stk->capacity, &(stk->capacity));
    printf("&data = %p\n", &stk->data);
    for (int i = 0; i < stk->capacity; i++)
    {
        if (i < stk->size)
            printf("*%d = %f\n", i, *(stk->data + i));
        else
            printf(" %d = %f\n", i, *(stk->data + i));
    }
    printf("\n");
}

int stack_push(stack_t* stk, stack_elem_t x)
{
    STACK_ASSERT(stk);
    if (stk->size + 1 > stk->capacity)
    {
        stk->capacity *= 2;
        stk->data = (stack_elem_t*)realloc(stk->data,
                    (size_t)stk->capacity*sizeof(stack_elem_t));
        if (stk->data == NULL)
        {
            printf("ERROR: unable to reallocate memory for data\n");
            return 0;
        }
    }
    *(stk->data + stk->size) = x;
    stk->size++;
    return 1;
}

int stack_pop(stack_t* stk, stack_elem_t* x)
{
    STACK_ASSERT(stk);
    *x = *(stk->data + stk->size - 1);
    stk->size--;
    if (stk->size <= stk->capacity/4 && stk->capacity >= 2*START_CAPASITY)
    {
        stk->capacity /= 2;
        stk->data = (stack_elem_t*)realloc(stk->data,
                    (size_t)stk->capacity*sizeof(stack_elem_t));
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
