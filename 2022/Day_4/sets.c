/*******************************************************************************
*
* Program: Set Data Structure
*
* Description: Example of a set data structure and operations (union, subset,
* intersection, etc.) implemented in C.  See the Wikipedia article on Sets:
* https://en.wikipedia.org/wiki/Set_(mathematics)
*
*
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "sets.h"

// returns a pointer to a new Set allocated on the heap
Set* init()
{
    // allocate space for the set
    Set *new_set = (Set *) malloc(sizeof(Set));

    // initially the set will be empty with no members, so set length to 0
    new_set->length = 0;

    // allocate enough space to store 1 member, we'll expand this as needed
    new_set->members = (int *) malloc(sizeof(int));

    // return the new Set (or more specifically, a pointer to it)
    return new_set;
}

// returns true if the Set is empty, and false otherwise
bool is_empty(Set *set)
{
    return (set->length == 0);
}

// returns true if value is a member of set, and false otherwise
bool is_member(Set *set, int value)
{
    // if we can find the value in the set's members, it is in the set
    for (int i = 0; i < set->length; i++)
        if (set->members[i] == value) return true;

    // if after checking all the set's members we can't find the value, it is
    // not a member of the set
    return false;
}

// inserts the member value into the set (if it is not already in the set!)
void insert(Set *set, int member)
{
    // check to make the member value is not in the set already
    if (!is_member(set, member))
    {
        // allocate space to store the *new* amount of members in the set
        set->members =
                realloc(set->members, sizeof(int) * (set->length + 1));

        // put the member into the set at the next available index
        set->members[set->length] = member;

        // increment the set length to acknowledge the new length of the set
        set->length = set->length + 1;
    }
}

// prints out the set
void print_set(Set *set)
{
    // loop through the array of set values, print each of them out separated by
    // a comma, except the last element
    printf("{");
    for (int i = 0; i < set->length; i++) {
        if (i == (set->length - 1))
            printf("%d", set->members[i]);
        else
            printf("%d,", set->members[i]);
    }
    printf("}");
}

// returns a pointer to a new set resulting from the union of setA and setB
Set *set_union(Set *setA, Set *setB)
{
    // create the new set
    Set *new = init();

    // insert all the values from set A
    for (int i = 0; i < setA->length; i++)
        insert(new, setA->members[i]);

    // insert all the values form Set B
    for (int i = 0; i < setB->length; i++)
        insert(new, setB->members[i]);

    // return the pointer to the new set
    return new;
}

// returns a pointer to the new set created by the intersection of setA and setB
Set *set_intersection(Set *setA, Set *setB)
{
    // create the new set
    Set *new = init();

    // insert each element of A into the new set if it is also a member of B
    for (int i = 0; i < setA->length; i++)
        if (is_member(setB, setA->members[i]))
            insert(new, setA->members[i]);

    // return the pointer to the new set
    return new;
}

// returns a pointer to a new set with difference of setA - setB
Set *set_difference(Set *setA, Set *setB)
{
    // create the new set
    Set *new = init();

    // insert the element from set A into the new set ONLY if it is not also
    // a member of set B
    for (int i = 0; i < setA->length; i++)
        if (!is_member(setB, setA->members[i]))
            insert(new, setA->members[i]);

    // return the pointer to the new set
    return new;
}

// returns true if setA is a subset of setB, and false otherwise
bool is_subset(Set *setA, Set *setB)
{
    // if any element of setA is NOT a member of setB, then it is not a subset
    for (int i = 0; i < setA->length; i++)
        if (!is_member(setB, setA->members[i])) return false;

    // if ALL the elements of setA ARE members of setB, then it IS a subset
    return true;
}

// returns true if setA is equal to setB
bool set_equality(Set *setA, Set *setB)
{
    // if the sets do not have the same length, it is not possible for them to
    // be equal, and we return false
    if (setA->length != setB->length) return false;

    // if the sets are equal length, and all the elements of setA are in setB,
    // then they MUST be equal, and so we let is_subset() determine this
    return is_subset(setA, setB);
}


// returns true if setA is equal to setB
void destroy_set(Set **setA)
{
    free((*setA)->members);
    free(*setA);
    
}