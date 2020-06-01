/* * * * * * *
 * Deque module (i.e., double ended queue) for Assignment 1
 *
 * created for COMP20007 Design of Algorithms 2019
 * template by Tobias Edwards <tobias.edwards@unimelb.edu.au>
 * implementation by Yunfei Jing 987784
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "deque.h"


// Create a new empty Deque and return a pointer to it
Deque *new_deque() {
  // Allocate the memory for the deque pointer 
  Deque *deque = (Deque*) malloc(sizeof(deque));
  assert(deque);

  // Initialize the deque
  deque->head = NULL;
  deque->tail = NULL;
  deque->size = 0;

  return deque;
}

// Create a new node with given data
Node *new_node(void* data) {
  Node *node = (Node*) malloc(sizeof(Node));
  assert(node);

  node->data = data;
  node->prev = NULL;
  node->next = NULL;

  return node;
}


// Add a Point to the top of a Deque
// Runtime: O(1)
void deque_push(Deque *deque, void* data) {
  assert(deque!=NULL);
  
  // Make a new node
  Node *new = new_node(data);
  assert(new);
  
  if(deque->size>0) {
    // The new node will be linked to the previous head node
    deque->head->prev = new;
    new->next = deque->head;
  }

  // If deque is empty, the node is both first and last
  if(deque->size==0) {
    deque->tail = new;
  }

  deque->head = new;
  // Keep updating the size of the deque
  deque->size++;
}

// Add a Point to the bottom of a Deque
// Runtime: O(1)
void deque_insert(Deque *deque, void* data) {
  assert(deque!=NULL);

  Node *new = new_node(data);
  assert(new);

  // If deque is empty, both head and tail will be the new node
  if(deque->size==0) {
    deque->head = new;
  } else {
    // Otherwise the new node goes to the end of the deque
    deque->tail->next = new;
    new->prev = deque->tail;
  }
  
  // Update the tail node and the size of the deque
  deque->tail = new;
  deque->size++;
}


// Remove and return the top Point from a Deque
// Runtime: O(1)
void* deque_pop(Deque *deque) {
  assert(deque!=NULL);
  assert(deque->size>0);

  // Save the data which will be returned
  Node *node = deque->head;
  void* data = node->data;
  
  // Assign the head to its next
  // The second node in the original deque becomes the first
  if(deque->size>1) {
    deque->head = deque->head->next;
    deque->head->prev = NULL;
  }

  // If there is only one node in the deque, the tail will be NULL as well
  if(deque->size==1) {
    deque->head = NULL;
    deque->tail = NULL;
  }

  // Update the size of the deque and free the memory
  deque->size--;
  free_node(node);
  return data;
}

// Remove and return the bottom Point from a Deque
// Runtime: O(1)
void* deque_remove(Deque *deque) {
  assert(deque!=NULL);
  assert(deque->size>0);

  Node *node = deque->tail;
  void* data = node->data;

  // Make the second last node in the original deque the last
  // We have to use a doubly linked list in order to do this in O(1)
  if(deque->size>1) {
    deque->tail = node->prev;
    deque->tail->next = NULL;
  }
  
  // If there is only one node in the deque, the head will be NULL as well
  if(deque->size==1) {
    deque->tail = NULL;
    deque->head = NULL;
  }

  // Update the size of the deque and free the memory
  deque->size--;
  free_node(node);
  return data;
}

// Return the number of Points in a Deque
// Runtime: O(1)
long deque_size(Deque *deque) {
  assert(deque!=NULL);
  return deque->size;
}


// Free the memory associated with a Deque
void free_deque(Deque *deque) {
  assert(deque!=NULL);
  
  Node* node = deque->head;
  Node* next;
  while(node) {
    next = node->next;
    // Free each node in the deque
    free_node(node);
    node = next;
  }
  free(deque);
}

// Helper function to clear the memory of a node
void free_node(Node *node) {
  free(node->data);
  free(node);
}