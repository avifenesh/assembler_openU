#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/*struct for node in list*/
typedef struct node
{
	/*the value stored in the node*/
	ELM val;
	/*pointer to the next node*/
	struct node* next;
} node;

/*struct for the root of the list */
typedef struct list_root
{
	/* pointer to the first node in list */
	node* head;
	/* pointer to a function that creates a new elm in the list */
	ELM(*create_elm)();
	/* pointer to a function that frees an elm */
	void(*free_elm)(ELM);
} list_root;

ELM GetFirstFromList(HEAD head, ELM* _node)
{
	list_root* root = (list_root*)head;
	if ((root == NULL) || (root->head == NULL))
		return NULL;
	*_node = root->head;
	return root->head->val;
}

int HasMoreInList(ELM* _node)
{
	struct node* tmp;
	if ((_node == NULL) || (*_node == NULL))
		return 0;
	tmp = *_node;
	return tmp->next != NULL;
}

ELM GetNextFromList(ELM* _node)
{
	struct node* tmp;
	if ((_node == NULL) || (*_node == NULL))
		return NULL;
	tmp = *_node;
	tmp = tmp->next;
	*_node = tmp;
	if (tmp == NULL)
		return NULL;
	return tmp->val;
}

HEAD CreateList(ELM(*create_elm)(),
	void(*free_elm)(ELM))
{
	struct list_root* root;
	root = (list_root*)malloc(sizeof(list_root));
	if (root == NULL)
		return NULL;
	root->create_elm = create_elm;
	root->free_elm = free_elm;
	root->head = NULL;
	return root;
}

void FreeList(HEAD head)
{
	list_root* root = (list_root*)head;
	node* n = root->head;
	node* prev;
	while (n != NULL)
	{
		prev = n;
		n = n->next;
		root->free_elm(prev->val);
		free(prev);
	}
	free(head);
}

ELM AppendElementToList(HEAD head)
{
	list_root* root = (list_root*)head;
	node* tmp_node = root->head;
	node* new_node;
	node* prev_node = NULL;
	ELM elm = root->create_elm();
	if (elm == NULL)
		return NULL;
	new_node = (node*)malloc(sizeof(node));
	if (new_node == NULL)
		return NULL;
	new_node->next = NULL;
	new_node->val = elm;
	/* add a node right after the root */
	if (root->head == NULL)
	{
		root->head = new_node;
		return elm;
	}
	prev_node = tmp_node;
	while (tmp_node != NULL)
	{
		prev_node = tmp_node;
		tmp_node = tmp_node->next;
	}
	if (prev_node == NULL)
		return NULL;
	/*add in the end of list*/
	prev_node->next = new_node;
	return elm;
}
