#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main()
{
    struct stack_t my_stack = {};
    if(0 != stack_init(&my_stack))
        return EXIT_FAILURE;
    stack_printf(&my_stack);

    stack_push(&my_stack, 10);
    stack_push(&my_stack, 20);
    stack_push(&my_stack, 30);
    stack_printf(&my_stack);

    stack_elem_t x = 0;
    stack_pop(&my_stack, &x);
    printf("x = %.1f\n", x);
    stack_pop(&my_stack, &x);
    printf("x = %.1f\n", x);
    stack_printf(&my_stack);

    stack_printf(&my_stack);
    stack_destroy(&my_stack);
}
