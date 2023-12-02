//
// Created by btitus1 on 12/6/22.
//

#ifndef DAY_4_SETS_H
#define DAY_4_SETS_H

#include <stdbool.h>

// STRUCTS
// define the Set data structure
typedef struct {
    int *members;    // pointer to dynamically allocated array of the set members
    int length;      // the number of members of the set
} Set;


// FORWARD DECLARATIONS
Set* init();
bool is_empty(Set *set);
void insert(Set *set, int member);
void print_set(Set *set);
Set *set_union(Set *setA, Set *setB);
Set *set_intersection(Set *setA, Set *setB);
Set *set_difference(Set *setA, Set *setB);
bool is_subset(Set *setA, Set *setB);
bool set_equality(Set *setA, Set *setB);
bool is_member(Set *set, int value);

#endif //DAY_4_SETS_H
