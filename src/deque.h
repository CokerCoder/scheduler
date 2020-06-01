/* * * * * * *
 * Deque module
 *
 * created for COMP20007 Design of Algorithms 2019
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Yunfei Jing 987784
 * Modified for COMP30023 Computer Systems
 */


#ifndef DEQUE_H
#define DEQUE_H


typedef struct deque Deque;

typedef struct node Node;

struct deque {
  Node *head;
  Node *tail;
  __int64_t size;
};

struct node {

  // Since we need three doubly linked list overall, one for processes and one for memory addresses and one for pages
  // So a linked list can store generic data type is needed, which can be achived by storing void* data types
  void* data;

  Node *prev;
  Node *next;
};

// Initalize new deque and node and return a pointer to it
Deque *new_deque();
Node *new_node(void* data);


void free_deque(Deque *deque);
void free_node(Node *node);


// Add a node to the front
void deque_push(Deque *deque, void* data);
// Add a node to the back
void deque_insert(Deque *deque, void* data);
// Delete the first node
void* deque_pop(Deque *deque);
// Delete the last node
void* deque_remove(Deque *deque);
// Size
__int64_t deque_size(Deque *deque);


#endif
