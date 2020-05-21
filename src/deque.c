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
#include "process.h"


// Create a new empty Deque and return a pointer to it
Deque *new_deque() {
  // Allocate the memory for the deque pointer 
  Deque *deque = malloc(sizeof(*deque));
  assert(deque);

  // Initialize the deque
  deque->head = NULL;
  deque->tail = NULL;
  deque->size = 0;

  return deque;
}

// Create a new empty node
Node *new_node() {
  Node *node = malloc(sizeof(*node));
  assert(node);

  return node;
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
  free(node);
}

// Add a Point to the top of a Deque
// Runtime: O(1)
void deque_push(Deque *deque, Process process) {
  assert(deque!=NULL);
  
  // Make a new node
  Node *new = new_node();
  // Assign the data that we want to store to the new node
  new->process = process;
  new->prev = NULL;
  
  if(deque->size>0) {
    // The new node will be linked to the previous head node
    deque->head->prev = new;
  }
    
  new->next = deque->head;
  deque->head = new;

  // If deque is empty, the node is both first and last
  if(deque->size==0) {
    deque->tail = new;
  }

  // Keep updating the size of the deque
  deque->size++;
}

// Add a Point to the bottom of a Deque
// Runtime: O(1)
void deque_insert(Deque *deque, Process process) {
  assert(deque!=NULL);

  Node *new = new_node();

  new->process = process;
  new->prev = deque->tail;
  new->next = NULL;

  // If deque is empty, both head and tail will be the new node
  if(deque->size==0) {
    deque->head = new;
  } else {
    // Otherwise the new node goes to the end of the deque
    deque->tail->next = new;
  }
  
  // Update the tail node and the size of the deque
  deque->tail = new;
  deque->size++;
}

// Remove and return the top Point from a Deque
// Runtime: O(1)
Process deque_pop(Deque *deque) {
  assert(deque!=NULL);
  assert(deque->size>0);

  // Save the data which will be returned
  Node *node = deque->head;
  Process process = node->process;
  
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
  return process;
}

// Remove and return the bottom Point from a Deque
// Runtime: O(1)
Process deque_remove(Deque *deque) {
  assert(deque!=NULL);
  assert(deque->size>0);

  Node *node = deque->tail;
  Process process = node->process;

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
  return process;
}

// Move the top process to the last available position
void move_to_last(Deque* deque, int clock) {

  assert(deque!=NULL);
  assert(deque->size>0);

  // If there is only one node ledt, nothing can be done
  if (deque->size==1) {
    return;
  }

  Node* first = deque->head;
  Node* last = deque->tail;

  Node* curr = deque->head;

  // After this loop, curr will be the next arriving process, so we will insert the first process before curr
  while (curr!=NULL && curr->process.arrival_time <= clock) {
    curr = curr->next;
  }

  // If no processes arrives, simply return
  if (curr==first) {
    return;
  }

  // If curr is NULL, means all processes have arrived and simply insert the first process to the end
  if (curr==NULL) {
    deque->head = first->next;
    first->next = NULL;
    last->next = first;
    deque->tail = last->next;
    return;
  }

  // Otherwise, move the first process before the curr (next arriving) process
  // Change the head pointer to point to second node now
  deque->head = first->next;

  first->prev = curr->prev;
  curr->prev = first;
  
  // Set the next of first as curr
  first->next = curr;
  first->prev->next = first;
}

// Return the number of Points in a Deque
// Runtime: O(1)
int deque_size(Deque *deque) {
  assert(deque!=NULL);
  return deque->size;
}

// Helper function to print the deque when debugging
void print_deque(Deque *deque) {
  assert(deque!=NULL);

  Node *curr = deque->head;
  printf("pid      arrival      memory      jobtime      timeleft\n");
  while (curr != NULL) { 
      printf("%-12d%-12d%-12d%-12d%-12d\n", \
      curr->process.pid, curr->process.arrival_time, curr->process.mem_size, curr->process.job_time, curr->process.remaining_time); 
      curr = curr->next; 
  } 
  printf("\n");
}