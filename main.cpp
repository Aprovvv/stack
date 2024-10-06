#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <cstdint>
#include "stack.h"
#include "color_print/color_print.h"

typedef uint64_t stack_elem_t;

struct hack {
    uint64_t can;
    int hui;
};

static int print_double(const void* ptr);

int main()
{
    struct hack a = {0XFEE1DEADL, 666};
    struct stack_t* my_stack = stack_init(sizeof(a), 0);
    if(NULL == my_stack)
    {
        fprintf_color(stderr, CONSOLE_TEXT_RED,
                          "ERROR: unable to allocate memory: %s\n",
                          strerror(errno));
        return EXIT_FAILURE;
    }


    stack_printf(my_stack, print_double);

    stack_push(my_stack, &a);
    stack_printf(my_stack, print_double);
    //stack_pop(my_stack, &a);

    /*for (int i = 1; i <= 10; i++)
    {
        stack_elem_t t = (stack_elem_t)i*10;
        if(stack_push(my_stack, &t) != 0)
        {
            fprintf_color(stderr, CONSOLE_TEXT_RED,
                          "ERROR: unable to reallocate "
                          "memory for data: %s\n",
                          strerror(errno));
            return(EXIT_FAILURE);
        }
    }

    stack_printf(my_stack, print_double);

    stack_elem_t x = 4;
    for (int i = 1; i <= 4; i++)
    {
        if (stack_pop(my_stack, &x) != 0)
        {
            fprintf_color(stderr, CONSOLE_TEXT_RED,
                          "ERROR: unable to reallocate "
                          "memory for data: %s\n",
                          strerror(errno));
            return EXIT_FAILURE;
        }
        printf("main x = %.1f\n", x);
    }
    stack_printf(my_stack, print_double);
    for (int i = 1; i <= 4; i++)
    {
        if (stack_pop(my_stack, &x) != 0)
        {
            fprintf_color(stderr, CONSOLE_TEXT_RED,
                          "ERROR: unable to reallocate "
                          "memory for data: %s\n",
                          strerror(errno));
            return EXIT_FAILURE;
        }
        printf("main x = %.1f\n", x);
    }
    stack_printf(my_stack, print_double);
    for (int i = 1; i <= 2; i++)
    {
        if (stack_pop(my_stack, &x) != 0)
        {
            fprintf_color(stderr, CONSOLE_TEXT_RED,
                          "ERROR: unable to reallocate "
                          "memory for data: %s\n",
                          strerror(errno));
            return EXIT_FAILURE;
        }
        printf("main x = %.1f\n", x);
    }*/

    stack_destroy(my_stack);
}

static int print_double(const void* ptr)
{
    const stack_elem_t* double_ptr = (const stack_elem_t*)ptr;
    return fprintf(stderr, "%.1d", *double_ptr);
}
