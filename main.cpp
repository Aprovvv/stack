#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <cstdint>
#include "stack.h"
#include "color_print/color_print.h"

typedef double stack_elem_t;

static int print_double(const void* ptr);

int main()
{
    struct stack_t* my_stack = stack_init(sizeof(stack_elem_t), 1);
    if(NULL == my_stack)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                      "ERROR: unable to allocate memory: %s\n",
                      strerror(errno));
        return EXIT_FAILURE;
    }

    stack_elem_t val = 228;
    stack_push(my_stack, &val);
    stack_printf(my_stack, print_double);
    for (int i = 1; i < 6; i++)
    {
        val = i*11;
        stack_push(my_stack, &val);
    }
    stack_printf(my_stack, print_double);

    for (int i = 0; i < 3; i++)
    {
        stack_pop(my_stack, &val);
        printf("val = %.1f\n", val);
    }
    stack_printf(my_stack, print_double);

    stack_destroy(my_stack);
}

static int print_double(const void* ptr)
{
    const stack_elem_t* double_ptr = (const stack_elem_t*)ptr;
    return fprintf(stderr, "%.1f", *double_ptr);
}
