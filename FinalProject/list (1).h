#ifndef LIST_H
#define LIST_H

/* element in the list */
typedef void* ELM;
/* the head of the list */
typedef void* HEAD;

/* create a linked list
create_elm - pointer to a function that creates
a new element in the list.
free_elm - pointer to a function that frees an element
returns the head of the list
*/
HEAD CreateList(ELM(*create_elm)(),
	void(*free_elm)(ELM));

/* destroy the linked list */
void DestroyList(HEAD head);

/* append a new element to the end of the list
and return it */
ELM AppendElementToList(HEAD head);

/* for iterations */

/* return the first element in the list */
ELM GetFirstFromList(HEAD head, ELM* _node);
/* return 1 if there are more elements in the list */
int HasMoreInList(ELM* _node);
/* return the next element in the list */
ELM GetNextFromList(ELM* _node);

#endif
