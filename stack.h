#ifndef STACK_H
#define STACK_H

#ifndef NDEBUG
#define STACK_ASSERT(stk) stack_assert(stk, __FILE__, __LINE__)
#else
#define STACK_ASSERT(stk)
#endif

struct stack_t;

struct stack_t* stack_init(size_t elem_size, size_t start_capacity);

int stack_push(stack_t* stk, void* x);
int stack_pop(stack_t* stk, void* x);
int stack_destroy(stack_t* stk);
int stack_printf(stack_t* stk, int(*print_func)(const void*));

void stack_view(struct stack_t* stk, size_t n, void* dest);

void* stack_data (stack_t* stk);
size_t stack_size (stack_t* stk);

#endif
