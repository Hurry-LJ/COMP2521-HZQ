// Interface to the Set ADT
// COMP2521 22T2 Assignment 1

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef SET_H
#define SET_H

#include <limits.h>
#include <stdbool.h>

#define UNDEFINED INT_MIN

typedef struct set *Set;


////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty set
 */
Set SetNew(void);

/**
 * Frees all memory associated with the given set
 */
void SetFree(Set s);

/**
 * Inserts an item into the set
 */
void SetInsert(Set s, int item);

/**
 * Returns the number of elements in the set
 */
int SetSize(Set s);

/**
 * Returns true if the set contains the given item, and false otherwise
 */
bool SetContains(Set s, int item);

/**
 * Prints the given set in the format
 * {elem1, elem2, elem3}
 */
void SetShow(Set s);

////////////////////////////////////////////////////////////////////////
// Further Operations

/**
 * Returns a new set representing the union of the two sets
 */
Set SetUnion(Set s1, Set s2);

/**
 * Returns a new set representing the intersection of the two sets
 */
Set SetIntersection(Set s1, Set s2);

/**
 * Returns a new set representing the set difference s1 - s2
 */
Set SetDifference(Set s1, Set s2);

/**
 * Returns true if the two sets are equal, and false otherwise
 */
bool SetEquals(Set s1, Set s2);

/**
 * Returns true if set s1 is a subset of set s2, and false otherwise
 */
bool SetSubset(Set s1, Set s2);

/**
 * Finds the floor of a given item in a set
 */
int SetFloor(Set s, int item);

/** 
 * Returns the ceiling of a given item in a set
 */
int SetCeiling(Set s, int item);

////////////////////////////////////////////////////////////////////////
// Cursor Operations

typedef struct cursor *SetCursor;

/**
 * Creates a new cursor positioned at the smallest element of the set
 */
SetCursor SetCursorNew(Set s);

/**
 * Frees all memory associated with the given cursor
 */
void SetCursorFree(SetCursor cur);

/**
 * Returns the element at the cursor's current position, and then moves
 * the cursor to the next greatest element. If there is no next greatest
 * element, then all subsequent calls to SetCursorNext on this cursor
 * should return UNDEFINED.
 */
int SetCursorNext(SetCursor cur);

////////////////////////////////////////////////////////////////////////

#endif

