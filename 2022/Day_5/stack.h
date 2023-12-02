//
// Created by btitus1 on 12/6/22.
//

#ifndef DAY_5_STACK_H
#define DAY_5_STACK_H

#include <stdbool.h>

// Stack data structure definition.  An array is used to implement a stack,
// it is possible to implement stacks in other ways (e.g. a linked list).  The
// 0th index becomes the "bottom" of the stack, and we "stack" items at each
// subsequent index in the array.
typedef struct
{
    // pointer to dynamically allocated array which will store stack items
    int *collection;

    // the length of the collections array is the max capacity of items
    int capacity;

    // the current number of items in the stack
    int size;

} Stack;

// library of functions for working with Stack data structure
Stack *create_stack(int capacity);
void destroy_stack(Stack *stack);
bool is_full(Stack *stack);
bool is_empty(Stack *stack);
bool pop(Stack *stack, int *item);
bool push(Stack *stack, int item);
bool peek(Stack *stack, int *item);
bool print_stack(Stack *stack, bool vert);
bool print_stacks(Stack **stack, int count);

#endif //DAY_5_STACK_H