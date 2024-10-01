#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main()
{
    struct stack_t my_stack = {0};
    if(0 != stack_init(&my_stack))
        return EXIT_FAILURE;
    stack_printf(&my_stack);

    for (int i = 1; i <= 10; i++)
        stack_push(&my_stack, 10*i);

    stack_printf(&my_stack);

    stack_elem_t x = 0;
    for (int i = 1; i <= 4; i++)
    {
        stack_pop(&my_stack, &x);
        printf("x = %.1f\n", x);
    }
    stack_printf(&my_stack);
    for (int i = 1; i <= 4; i++)
    {
        stack_pop(&my_stack, &x);
        printf("x = %.1f\n", x);
    }
    stack_printf(&my_stack);
    for (int i = 1; i <= 2; i++)
    {
        stack_pop(&my_stack, &x);
        printf("x = %.1f\n", x);
    }
    stack_printf(&my_stack);

    stack_destroy(&my_stack);
}
