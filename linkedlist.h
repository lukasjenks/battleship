#ifndef LINKEDLIST_DOT_H	// Include guard that prevents
#define LINKEDLIST_DOT_H	// file from being included twice

struct node {
	char identifier[5];
	int x_solution;
	int y_solution;

	struct node *next;
	struct node *last;
};

/* Function: isEmpty
   Arguments: None
   Return Value: int
   Purpose: Returns integer value representing whether
            the circular doubly linked list is empty or not
*/
int isEmpty();

/* Function: insertHead
   Arguments: char identifier[], int x_solution, int y_solution
   Return Value: void
   Purpose: Instantiates a new node struct, populates it with the
            properties passed to the function, and inserts the
            node in the head position of the circular doubly linked list
*/ 
void insertHead(char identifier[], int x_solution, int y_solution);

/* Function: deleteNode
   Arguments: struct node *to_delete
   Return Value: void
   Purpose: Takes a pointer to the node that should
            be deleted, and removes it from the
            circular doubly linked list
*/
void deleteNode(struct node *to_delete);

/* Function: printList
   Arguments: None
   Return Value: void
   Purpose: Displays the contents of the nodes
            currently present in the circular
            doubly linked list
*/
void printList();

#endif				/* LINKEDLIST_DOT_H */
