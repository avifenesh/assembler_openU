#ifndef LIST_H
#define LIST_H

/*elm = element in the list*/
typedef void* ELM;
/*head of the list*/
typedef void* HEAD;

/*create the linked-list
create_elm = pointer to a function that creates
a new elm in the list.
free_elm = pointer to a function that frees an elm
returns the head oflist*/
HEAD CreateList(ELM(*create_elm)(),
	void(*free_elm)(ELM));

/*destroy the linked-list*/
void FreeList(HEAD head);

/*append a new elm to end of list
and return it*/
ELM AppendElementToList(HEAD head);

/* for iterations */

/* return first elm of list */
ELM GetFirstFromList(HEAD head, ELM* _node);
/* return 1 if there is more node in list */
int HasMoreInList(ELM* _node);
/* return the next elm in list*/
ELM GetNextFromList(ELM* _node);

#endif
