#ifndef STACK_H
#define STACK_H

#ifndef NDEBUG
#define ON_DEBUG(X) X
#else
#define ON_DEBUG(X)
#endif

#ifndef NDEBUG
#define STACK_ASSERT(stk) stack_assert(stk, __FILE__, __LINE__)
#else
#define STACK_ASSERT(stk)
#endif

typedef double stack_elem_t;

struct stack_t {
    stack_elem_t* data;
    int size;
    int capacity;
};

void stack_assert(struct stack_t* stk, const char* file, int line);

int stack_push(stack_t* stk, stack_elem_t x);
int stack_pop(stack_t* stk, stack_elem_t* x);
int stack_error(struct stack_t* stk);
int stack_init(struct stack_t* stk);
int stack_destroy(stack_t* stk);
int stack_printf(stack_t* stk);
#endif
