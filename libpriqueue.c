/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.
  
  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
	q->size = 0;
	q->root = NULL;
	q->comparer = comparer;
}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
	priqueue_node_t * node = (priqueue_node_t *) malloc(sizeof(priqueue_node_t));
	node->content = ptr;
	node->next = NULL;
	
	if(q->root == NULL)
	{
		q->root = node;
		q->size++;
		return 0;
	}
	else
	{
		int counter = 0;
		priqueue_node_t
			*now = q->root,
			*last = NULL;
		
		while(counter < q->size)
		{
			int result = q->comparer(now->content, node->content);
			if(result > 0)
			{
				if(last == NULL)
				{
					q->root = node;
					node->next = now;
				}
				else
				{
					last->next = node;
					node->next = now;
				}
				q->size++;
				return counter;
			}
			else
			{
				last = now;
				now = last->next;
				counter++;
			}
		}
		
		last->next = node;
		counter++;
		q->size++;
		return counter;
	}
	return -1;
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
	if(q->size ==0)
		return NULL;
	
	return q->root->content;
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
	if(q->size ==0)
		return NULL;
	
	priqueue_node_t* r = q->root;
	void* r_content = r->content;
	q->root = r->next;
	
	free(r);
	q->size--;
	return r_content;
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
	if(index >= q->size)
		return NULL;
	
	priqueue_node_t* now = q->root;
	for (int i = 0; i < index; i++) {
		now = now->next;
	}
	
	return now->content;
}


/**
  Removes all instances of ptr from the queue. 
  
  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
	priqueue_node_t* now = q->root;
	priqueue_node_t* last  = NULL;
	int counter = 0;
	for (int i = 0; i < q->size; i++) {
		if (now->content == ptr) {
			if (last == NULL)
				q->root = now->next;
			else
				last->next = now->next;
			priqueue_node_t* r = now;
			now = now->next;
			free(r);
			counter++;
		}
		else
		{
			last = now;
			now = last->next;
		}
	}
	
	q->size -= counter;
	return counter;
}


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
	if(index >= q->size)
		return NULL;
	
	priqueue_node_t* now = q->root;
	priqueue_node_t* last  = NULL;
	
	for (int i = 0; i < index; i++) {
		last = now;
		now = last->next;
	}
	
	void* r_content = now->content;
	last->next = now->next;
	
	free(now);
	q->size--;
	return r_content;
}


/**
  Returns the number of elements in the queue.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
	return q->size;
}


/**
  Destroys and frees all the memory associated with q.
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
	priqueue_node_t* now = q->root;
	while(now != NULL)
	{
		priqueue_node_t* r = now;
		now = now->next;
		free(r);
	}
}
