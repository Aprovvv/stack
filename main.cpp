#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "stack.h"

typedef double stack_elem_t;

static int print_double(const void* ptr);

int main()
{
    struct stack_t* my_stack = stack_init(sizeof(stack_elem_t));
    if(NULL == my_stack)
    {
        fprintf(stderr, "ERROR: enable to "
                "allocate memory: %s\n", strerror(errno));
        return EXIT_FAILURE;
    }
    stack_printf(my_stack, print_double);

    for (int i = 1; i <= 10; i++)
    {
        double t = (double)i*10;
        if(stack_push(my_stack, &t) != 0)
            fprintf(stderr, "ERROR: unable to reallocate "
                    "memory for data: %s\n", strerror(errno));
    }

    stack_printf(my_stack, print_double);

    stack_elem_t x = 4;
    for (int i = 1; i <= 4; i++)
    {
        if (stack_pop(my_stack, &x) != 0)
        {
            printf("ERROR: unable to reallocate "
                   "memory for data: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        printf("main x = %.1f\n", x);
    }
    stack_printf(my_stack, print_double);
    for (int i = 1; i <= 4; i++)
    {
        if (stack_pop(my_stack, &x) != 0)
        {
            printf("ERROR: unable to reallocate "
                   "memory for data: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        printf("main x = %.1f\n", x);
    }
    stack_printf(my_stack, print_double);
    for (int i = 1; i <= 2; i++)
    {
        if (stack_pop(my_stack, &x) != 0)
        {
            printf("ERROR: unable to reallocate "
                   "memory for data: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }
        printf("main x = %.1f\n", x);
    }
    stack_printf(my_stack, print_double);

    stack_destroy(my_stack);
}

static int print_double(const void* ptr)
{
    const double* double_ptr = (const double*)ptr;
    return printf("%f", *double_ptr);
}
