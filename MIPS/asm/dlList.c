/**
* $Log: dlList.c,v $
* Revision 1.11  2014/04/17 01:41:40  drw9888
* Testing complete. Commit before submission
*
* Revision 1.10  2014/04/17 00:33:09  drw9888
* Runs through example transcript with no diff
*
* Revision 1.9  2014/04/16 23:03:39  drw9888
* Back to prev version
*
* Revision 1.7  2014/04/16 22:58:09  drw9888
* Before I mess things up...
*
* Revision 1.6  2014/04/16 14:23:40  drw9888
* testdllist runs without error
*
* Revision 1.5  2014/04/16 14:18:40  drw9888
* Clean compile
*
* Revision 1.4  2014/04/16 14:09:28  drw9888
* Fixed it
*
* Revision 1.2  2014/04/15 00:46:26  drw9888
* Implemeted dlList.c
*
* Revision 1.1  2014/04/14 23:16:44  drw9888
* Added .c files
*
*
* $Id: dlList.c,v 1.11 2014/04/17 01:41:40 drw9888 Exp $
*
* @author Dakota Williams 
*/

#include <stdlib.h>
#include <stdbool.h>

#define _DLL_IMPL_

typedef struct DlNode {
	struct DlNode* next;
	struct DlNode* prev;
	void* data;
} * DlNode_T;
	
typedef struct DlList { 
	struct DlNode* start;
	struct DlNode* end;
	struct DlNode* cursor;
	int size;
} * DlList_T;

#include "dlList.h"

DlList_T dll_create( void ) {
	DlList_T new;
	new = malloc(sizeof(*new));
	new->start = NULL;
	new->end = NULL;
	new->cursor = NULL;
	new->size = 0;
	return new;
}

void dll_destroy( DlList_T lst ) {
	dll_clear(lst);
	free(lst);
}

void dll_clear( DlList_T lst ) {
	if(dll_empty(lst)) {
		return;
	}
	
	DlNode_T iter = lst->start;
	while(iter != NULL) {
		DlNode_T temp = iter->next;
		free(iter);
		iter = temp;
	}

	lst->start = NULL;
	lst->end = NULL;
	lst->cursor = NULL;
	lst->size = 0;
}

bool dll_move_to( DlList_T lst, int indx ) {
	if(indx < 0 || indx >= lst->size) {
		return false;
	}

	DlNode_T iter = lst->start;
	for(int i = 0; i < lst->size; i++) {
		if(i == indx) {
			lst->cursor = iter;
			break;
		}
		iter = iter->next;
	}
	return true;
}

int dll_has_next( DlList_T lst ) {
	if(lst->cursor->next == NULL) {
		return false;
	}
	return true;
}

void * dll_next( DlList_T lst ) {
	if(dll_has_next(lst)) {
		lst->cursor = lst->cursor->next;
		return lst->cursor->data;
	}
	return NULL;
}

void * dll_prev( DlList_T lst ) {
	if(lst->cursor->prev == NULL) {
		return NULL;
	}

	lst->cursor = lst->cursor->prev;
	return lst->cursor->data;
}

int dll_size( DlList_T lst ) {
	return lst->size;
}

void dll_append( DlList_T lst, void *data ) {
	DlNode_T new;
	new = malloc(sizeof(*new));
	if(new == NULL) {
		return;
	}
	new->data = data;

	if(lst->size == 0) {
		lst->start = new;
		lst->end = new;
		new->next = NULL;
		new->prev = NULL;
	} else {
		new->prev = lst->end;
		new->next = NULL;
		lst->end->next = new;
		lst->end = new;
	}
	
	lst->size++;
	lst->cursor = new;
}

void dll_insert_at( DlList_T lst, int indx, void *data ) {
	DlNode_T new;
	new = malloc(sizeof(*new));
	if(new == NULL) {
		return;
	}
	new->data = data;

	if(lst->size == 0) {
		lst->start = new;
		lst->end = new;
		new->next = NULL;
		new->prev = NULL;
		lst->size++;
		lst->cursor = new;
		return;
	}

	if(indx < 0 || indx > lst->size) {
		free(new);
		return;
	}
	
	if(indx == 0) {
		lst->start->prev = new;
		new->next = lst->start;
		new->prev = NULL;
		lst->start = new;
		lst->size++;
		lst->cursor = new;
		return;
	}

	if(indx == dll_size(lst)) {
		dll_append(lst, data);
		return;
	}

	DlNode_T iter = lst->start;
	for(int i = 0; i < indx; i++) {
		iter = iter->next;
	}

	new->next = iter;
	new->prev = iter->prev;
	iter->prev->next = new;
	iter->prev = new;

	lst->size++;
	lst->cursor = new;
}

void * dll_get( DlList_T lst, int indx ) {
	if(indx < 0 || indx >= lst->size) {
		return NULL;
	}

	DlNode_T iter = lst->start;
	for(int i = 0; i < lst->size; i++) {
		if(indx == i) {
			return iter->data;
		}
		iter = iter->next;
	}

	return NULL; //really should never happen, but it'll make the compiler happy
}

void * dll_set( DlList_T lst, int indx, void *data ) {
	if(indx < 0 || indx >= lst->size) {
		return NULL;
	}

	DlNode_T iter = lst->start;
	for(int i = 0; i < lst->size; i++) {
		if(indx == i) {
			iter->data = data;
		}
		iter = iter->next;
	}
	return NULL;
}

void * dll_pop( DlList_T lst, int indx ) {
	if(indx <= 0 || indx >= lst->size) {
		return NULL;
	}

	if(lst->size == 1) {
		void* temp = lst->start->data;
		free(lst->start);
		lst->start = NULL;
		lst->end = NULL;
		lst->cursor = NULL;
		lst->size = 0;
		return temp;
	}

	if(indx == 0) {
		DlNode_T temp = lst->start;
		lst->start = temp->next;
		temp->next->prev = NULL;
		void* tempdata = temp->data;
		free(temp);
		lst->cursor = lst->start;
		return tempdata;
	}

	DlNode_T iter = lst->start;
	for(int i = 0; i < lst->size; i++) {
		if(indx-1 == i) {
			DlNode_T temp = iter->next;
			
			iter->next = iter->next->next;
			if(iter->next != NULL) {
				iter->next->prev = iter;
				lst->cursor = iter->next;
			} else {
				lst->cursor = iter;
			}
			
			void* tempdata = temp->data;
			free(temp);
			lst->size--;
			return tempdata;
		}
		iter = iter->next;
	}

	return NULL; //should never happen, but it makes the compiler happy
}

int dll_index( DlList_T lst, void *data ) {
	if(dll_empty(lst)) {
		return -1;
	}

	DlNode_T iter = lst->start;
	for(int i = 0; i < lst->size; i++) {
		if(iter->data == data) {
			return i;
		}
		iter = iter->next;
	}

	return -1;
}

bool dll_empty( DlList_T lst ) {
	if(lst->size == 0) {
		return true;
	}
	return false;
}

