#include "linkedlist.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct node *head = NULL;
struct node *current = NULL;

/* Function: isEmpty
   Arguments: None
   Return Value: int
   Purpose: Returns integer value representing whether
            the circular doubly linked list is empty or not
*/
int isEmpty()
{
	return head == NULL;
}

/* Function: insertHead
   Arguments: char identifier[], int x_solution, int y_solution
   Return Value: void
   Purpose: Instantiates a new node struct, populates it with the
            properties passed to the function, and inserts the
            node in the head position of the circular doubly linked list
*/ 
void insertHead(char identifier[5], int x_solution, int y_solution)
{
	struct node *new_node = (struct node *)malloc(sizeof(struct node));
	strncpy(new_node->identifier, identifier, 4);
	new_node->x_solution = x_solution;
	new_node->y_solution = y_solution;

	if (isEmpty()) {
		head = new_node;
		head->next = head;
		head->last = head;
	} else {
		// Insert new node in head's previous position and assign
		// new node as new head
		new_node->next = head;
		head->last->next = new_node;
		new_node->last = head->last;
		head->last = new_node;
		head = new_node;
	}
}

/* Function: deleteNode
   Arguments: struct node *to_delete
   Return Value: void
   Purpose: Takes a pointer to the node that should
            be deleted, and removes it from the
            circular doubly linked list
*/
void deleteNode(struct node *to_delete)
{
    if (to_delete == head && to_delete->next == NULL) {
        free(head);
    } else if (to_delete == head && to_delete->next->next == head) {
        head = to_delete->next;
        head->next = NULL;
        head->last = NULL;
        free(to_delete);
    } else if (to_delete == head) {
        head = to_delete->next;
	    to_delete->last->next = to_delete->next;
	    to_delete->next->last = to_delete->last;
	    free(to_delete);
    } else {
        to_delete->last->next = to_delete->next;
        to_delete->next->last = to_delete->last;
        free(to_delete);
    }
}

/* Function: printList
   Arguments: None
   Return Value: void
   Purpose: Displays the contents of the nodes
            currently present in the circular
            doubly linked list
*/
void printList()
{
	struct node *iter = head;
	printf("(Identifier: %s, Solution x value: %d, Solution y value: %d\n) ",
	       iter->identifier, iter->x_solution, iter->y_solution);

	while (iter->next != head) {
		printf
		    ("(Identifier: %s, Solution x value: %d, Solution y value: %d\n) ",
		     iter->identifier, iter->x_solution, iter->y_solution);
		iter = iter->next;
	}
}
