#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

typedef double stack_elem_t;

int main()
{
    struct stack_t* my_stack = stack_init(sizeof(stack_elem_t));
    if(NULL == my_stack)
        return EXIT_FAILURE;
    stack_printf(my_stack);

    for (int i = 1; i <= 10; i++)
    {
        double t = (double)i*10;
        stack_push(my_stack, &t);
    }

    stack_printf(my_stack);

    stack_elem_t x = 4;
    for (int i = 1; i <= 4; i++)
    {
        stack_pop(my_stack, &x);
        printf("main x = %.1f\n", x);
    }
    stack_printf(my_stack);
    for (int i = 1; i <= 4; i++)
    {
        stack_pop(my_stack, &x);
        printf("main x = %.1f\n", x);
    }
    stack_printf(my_stack);
    for (int i = 1; i <= 2; i++)
    {
        stack_pop(my_stack, &x);
        printf("main x = %.1f\n", x);
    }
    stack_printf(my_stack);

    stack_destroy(my_stack);
}
