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

struct stack_t;

void* resize(void* ptr,
             size_t new_capacity,
             size_t old_capacity,
             size_t elem_size);

void stack_assert(struct stack_t* stk, const char* file, int line);
void set_hash(struct stack_t* stk);

struct stack_t* stack_init(size_t elem_size);

unsigned long hash(void* str, size_t size);

int data_hash_ok(struct stack_t* stk);
int stack_push(stack_t* stk, void* x);
int stack_pop(stack_t* stk, void* x);
int stack_error(struct stack_t* stk);
int str_hash_ok(struct stack_t* stk);
int stack_destroy(stack_t* stk);
int stack_printf(stack_t* stk);
#endif
