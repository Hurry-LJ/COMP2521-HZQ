// Implementation of the Set ADT using a balanced BST
// COMP2521 22T2 Assignment 1

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Set.h"
#include "SetStructs.h"

int max(int a, int b)
{
    if(a>b) return a;
    return b;
}

//AVL Tree Operations

/**
 * Return the node height
 *
 */
int GetHeight(Node avl){
    if(avl == NULL){
        return -1;
    }
    return avl->height;
}
/**
 * Returns true if the tree's height is balanced, and false otherwise
 */
int IsBalanced(Node tree){
    return GetHeight(tree->left) - GetHeight(tree->right);
}

/**
 * Right Rotate
 *
 */
Node RightRotate(Node tree){
    Node node = tree->left;
    tree->left = node->right;
    node->right = tree;
    tree->height = 1 + max(GetHeight(tree->left), GetHeight(tree->right));
    node->height = 1 + max(GetHeight(node->left), GetHeight(node->right));
    return node;
}

/**
 * Left Rotate
 *
 */
Node LeftRotate(Node tree){
    Node node = tree->right;
    tree->right = node->left;
    node->left = tree;
    tree->height = 1 + max(GetHeight(tree->left), GetHeight(tree->right));
    node->height = 1 + max(GetHeight(node->left), GetHeight(node->right));
    return node;
}

/**
 * Balance the height factor
 *
 */
Node Balance(Node tree){
    if(IsBalanced(tree) < -1){
        if(IsBalanced(tree->right) > 0){
            tree->right = RightRotate(tree->right);
        }
        tree = LeftRotate(tree);
    }

    if(IsBalanced(tree) > 1){
        if(IsBalanced(tree->left) < 0){
            tree->left = LeftRotate(tree->left);
        }
        tree = RightRotate(tree);
    }
    return tree;
}

/**
 * Create a new root of AVL
 *
 */
Node CreateRoot(int data){
    Node tree = malloc(sizeof(struct node));
    tree->left   = NULL;
    tree->right  = NULL;
    tree->item   = data;
    tree->height = 0;
    return tree;
}
/**
 * Insert a new data
 *
 */
Node Insert(Node tree, int data){
    if(tree == NULL){
        tree = CreateRoot(data);
    }
    else if(data > tree->item){
        tree->right = Insert(tree->right, data);
    }
    else if(data < tree->item){
        tree->left = Insert(tree->left, data);
    }
    
    tree->height = 1 + max(GetHeight(tree->right), GetHeight(tree->left));
    return Balance(tree);
}

/**
 * Insert a new data
 *
 */
void FreeTree(Node tree){
    while(tree != NULL){
        FreeTree(tree->left);
        FreeTree(tree->right);
        free(tree);
        tree = NULL;
    }
}



// Basic Operations

/**
 * Creates a new empty set
 */
Set SetNew(void){
    Set newset = malloc(sizeof(struct set));
    newset->tree = malloc(sizeof(struct node));
    newset->tree = NULL;
    newset->size = 0;
    return newset;
}

/**
 * Frees all memory associated with the given set
 */
void SetFree(Set s){
    FreeTree(s->tree);
    free(s);
}
/**
 * Inserts an item into the set
 */
bool SetContains(Set s, int item);
void SetInsert(Set s, int item) {
    // TODO
    if(!SetContains(s, item)){
        s->tree = Insert(s->tree, item);
        s->size += 1;
    }
    
}

/**
 * Returns the number of elements in the set
 */
int SetSize(Set s) {
    // TODO
    return s->size;
}

bool NodeContains(Node tree, int item){
    if(tree == NULL){
        return false;
    }
    else if(tree->item == item){
        return true;
    }
    else if(tree->item > item){
        return NodeContains(tree->left, item);
    }
    else{
        return NodeContains(tree->right, item);
    }
}

/**
 * Returns true if the set contains the given item, and false otherwise
 */
bool SetContains(Set s, int item) {
    // TODO
    if(NodeContains(s->tree, item)){
        return true;
    }
    return false;
}


/**
 * Display one line node
 *
 */
int idx = 0;
void Showtree(Node tree, int size){
    
    if(tree != NULL){
        Showtree(tree->left,size );
        printf("%d",tree->item );//sh���飿
        idx ++;
        if(size != idx ){
            printf(", ");
        }
        Showtree(tree->right,size);
    }
}
void SetShow(Set s) {//�������
    
    
    printf("{");
    Showtree(s->tree,s->size );
    printf("}");
    
    // TODO
}


////////////////////////////////////////////////////////////////////////
// Further Operations

void swap(Set s1, Set s2){
    Set t = s2;
    s2 = s1;
    s1 = t;
}


void JiontTree(Set s1, Node n2){
    
    if(n2 == NULL){
        return;
    }
    
    JiontTree(s1, n2->left);
    
    SetInsert(s1, n2->item);
    
    JiontTree(s1, n2->right);
    
}

/**
 * Returns a new set representing the union of the two sets
 */
Set SetUnion(Set s1, Set s2) {
    // TODO
//    if(s1->size < s2->size){
//        swap(s1, s2);
//    }
    Set s3 = SetNew();
    JiontTree(s3, s1->tree);
    JiontTree(s3, s2->tree);
    return s3;
}
void IntersectTree(Set s3, Node n1, Node n2){
    
    if(n2 == NULL){
        return;
    }
    
    IntersectTree(s3, n1, n2->left);
    
    if(NodeContains(n1, n2->item)){
        SetInsert(s3, n2->item);
    }
    
    IntersectTree(s3, n1, n2->right);
    
}
/**
 * Returns a new set representing the intersection of the two sets
 */
Set SetIntersection(Set s1, Set s2) {
    // TODO
    if(s1->size < s2->size){
        swap(s1, s2);
    }
    Set s3 = SetNew();
    IntersectTree(s3, s1->tree, s2->tree);
    
    return s3;
}

void DifferenceTree(Set s3, Node n1, Node n2){
    
    if(n1 == NULL){
        return;
    }
    
    DifferenceTree(s3, n1->left, n2);
    
    if(!NodeContains(n2, n1->item)){
        SetInsert(s3, n1->item);
    }
    
    DifferenceTree(s3, n1->right, n2);
    
}
/**
 * Returns a new set representing the set difference s1 - s2
 */
Set SetDifference(Set s1, Set s2) {
    // TODO
    Set s3 = SetNew();
    DifferenceTree(s3, s1->tree, s2->tree);
    return s3;
}
void EqualTree(bool *is, Node n1, Node n2){
    
    if(n1 == NULL){
        return;
    }
    
    EqualTree(is, n1->left, n2);
    
    if(!NodeContains(n2, n1->item)){
        *is = false;
        return;
    }
    
    EqualTree(is, n1->right, n2);
    
}
/**
 * Returns true if the two sets are equal, and false otherwise
 */
bool SetEquals(Set s1, Set s2) {
    // TODO
    bool is = true;
    bool *is_p = &is;
    EqualTree(is_p, s1->tree, s2->tree);
    return is && (s1->size == s2->size);
}

/**
 * Returns true if set s1 is a subset of set s2, and false otherwise
 */
bool SetSubset(Set s1, Set s2) {
    // TODO
    bool is = true;
    bool *is_p = &is;
    EqualTree(is_p, s1->tree, s2->tree);
    return is;
}

/**
 * Finds the floor of a given item in a set
 */
int SetFloor(Set s, int item) {
    // TODO
    Node tree = s->tree;
    int res = 0;
    bool is = false;
    while (tree != NULL) {
        if(item >= tree->item){
            res = tree->item;
            is = true;
            tree = tree->right;
        }
//        else if(item == tree->item){
//            return item;
//        }
        else{
            tree = tree->left;
        }
    }
    if(is == true){
        return res;
    }
    else{
        return UNDEFINED;
    }
    
}

/**
 * Returns the ceiling of a given item in a set
 */
int SetCeiling(Set s, int item) {
    // TODO
    Node tree = s->tree;
    int res = 0;
    bool is = false;
    while (tree != NULL) {
        if(item <= tree->item){
            res = tree->item;
            is = true;
            tree = tree->left;
        }
//        else if(item == tree->item){
//            return item;
//        }
        else{
            tree = tree->right;
        }
    }
    if(is == true){
        return res;
    }
    else{
        return UNDEFINED;
    }
}

////////////////////////////////////////////////////////////////////////
// Cursor Operations

/**
 * Creates a new cursor positioned at the smallest element of the set
 */
SetCursor SetCursorNew(Set s) {
    // TODO
    return NULL;
}

/**
 * Frees all memory associated with the given cursor
 */
void SetCursorFree(SetCursor cur) {
    // TODO
}

/**
 * Returns the element at the cursor's current position, and then moves
 * the cursor to the next greatest element. If there is no next greatest
 * element, then all subsequent calls to SetCursorNext on this cursor
 * should return UNDEFINED.
 */
int SetCursorNext(SetCursor cur) {
    // TODO
    return UNDEFINED;
}

////////////////////////////////////////////////////////////////////////

