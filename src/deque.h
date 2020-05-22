/* * * * * * *
 * Deque module (i.e., double ended queue) for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2019
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by <Insert Name Here>
 */

// You must not change any of the code already provided in this file, such as
// type definitions, constants or functions.
//
// You may, however, add additional functions and/or types which you may need
// while implementing your algorithms and data structures.

#ifndef DEQUE_H
#define DEQUE_H

// You may change the definition of Deque but DO NOT change the name
typedef struct deque Deque;

typedef struct node Node;

struct deque {
  Node *head;
  Node *tail;
  int size;
};

struct node {

  // Since we need two doubly linked list overall, one for processes and one for memory addresses
  // So a linked list can store generic data type is needed, which can be achived by storing void* data types
  void* data;

  Node *prev;
  Node *next;
};

// Create a new empty Deque and return a pointer to it
//
// DO NOT CHANGE THIS FUNCTION SIGNATURE
Deque *new_deque();


void free_deque(Deque *deque);
void free_node(Node *node);


void deque_push(Deque *deque, void* data);

void deque_insert(Deque *deque, void* data);

void* deque_pop(Deque *deque);

void* deque_remove(Deque *deque);



int deque_size(Deque *deque);


#endif
