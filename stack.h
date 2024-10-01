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

struct stack_t {

    size_t elem_size;
    void* data;
    int size;
    int capacity;
};

void stack_assert(struct stack_t* stk, const char* file, int line);

int stack_init(struct stack_t* stk, size_t elem_size);
int stack_push(stack_t* stk, void* x);
int stack_pop(stack_t* stk, void* x);
int stack_error(struct stack_t* stk);
int stack_destroy(stack_t* stk);
int stack_printf(stack_t* stk);
#endif
