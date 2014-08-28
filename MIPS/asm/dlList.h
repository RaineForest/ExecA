// dlList.h
// $Id: dlList.h,v 1.1 2014/04/14 23:12:29 drw9888 Exp $
//
// double linked list with internal, tracking cursor mechanism.
//
// @author: bks ben k steele
//
// // // // // // // // // // // // // // // // // // // // // // // //

#ifndef _DLLIST_ADT_H_
#define _DLLIST_ADT_H_

#include <stdbool.h>

#ifndef _DLL_IMPL_

/// DlList_T points to a representation of a double-linked list 
/// of void pointers (to abstract data objects).
typedef struct { } * DlList_T;

#endif

/// dll_create constructs an instance of an empty double-linked list.
/// @return instance of double-linked list (this is a pointer).
DlList_T dll_create( void );

/// dll_destroy clears and frees all storage associated with
/// the linked list data structure.
/// The function handles memory management of all data payloads.
/// @param lst the linked list to destroy.
/// @post lst is no longer usable; the function freed all its memory.
void dll_destroy( DlList_T lst );

/// dll_clear clears the list content, making the list empty.
/// dll_clear assumes all data payloads are dynamic and frees their storage.
/// @param lst the linked list to clear.
/// @post lst is now in an empty state.
void dll_clear( DlList_T lst );

/// dll_move_to moves the cursor to the requested index, if index is valid.
/// @param lst the list whose cursor should move.
/// @param indx the 0-based index to which to move the cursor.
/// @return true if the cursor move was successful.
bool dll_move_to( DlList_T lst, int indx );

/// dll_has_next returns whether or not the cursor refers to a valid position.
/// @param lst the list to check.
/// @return non-zero if the cursor refers to a valid current position.
int dll_has_next( DlList_T lst );

/// dll_next returns the current item and advance forward to next item.
/// The list module retains ownership of the memory address returned.
/// The pointer returned refers to the actual data; the client does not own it.
/// @param lst the list to iterate over.
/// @return the data pointer of the current item.
/// @pre the cursor refers to a valid current position.
/// @post the cursor refers to the position after the current one, if any.
void * dll_next( DlList_T lst );

/// dll_prev returns the current item and advance backward to previous item.
/// The list module retains ownership of the memory address returned.
/// The pointer returned refers to the actual data; the client does not own it.
/// @param lst the list to iterate over.
/// @return the data pointer of the current item.
/// @pre the cursor refers to a valid current position.
/// @post the cursor refers to the position before the current one, if any.
void * dll_prev( DlList_T lst );

/// dll_size returns the size of the list.
/// @param lst the subject list.
/// @return the count of items in the list.
int dll_size( DlList_T lst );

/// dll_append appends an item to the end of the list.
/// The function assumes ownership of the memory of the data payload.
/// @param lst the subject list.
/// @param data a pointer to the item to append.
/// @post cursor moves to refer to the appended item. the lst size grows by 1.
void dll_append( DlList_T lst, void *data );

/// dll_insert_at inserts an item at the requested position, if index is valid.
/// The client is responsible for memory management of the item's storage.
/// The function assumes ownership of the memory of the data payload.
/// @param lst the subject list.
/// @param indx the 0-based position.
/// @param data a pointer to the item to append.
/// @pre indx must be in the range [0...dll_size(lst) ).
/// @post if successful, cursor moves to position of inserted item.
/// @post if successful, the lst size grows by 1.
void dll_insert_at( DlList_T lst, int indx, void *data );

/// dll_get returns a pointer to the item at index; item remains in list.
/// The list module retains ownership of the memory address returned.
/// The pointer returned refers to the actual data; the client does not own it.
/// @param lst the subject list.
/// @param indx the 0-based position.
/// @return pointer to the item requested.
/// @pre indx must be in the range [ 0...dll_size(lst) ).
/// @post cursor position does not change. lst size and content is unchanged.
void * dll_get( DlList_T lst, int indx );

/// dll_set replaces the pointer to the item with the data value.
/// The function transfers ownership of the old memory to the client.
/// The client is responsible for memory management of the old, returned data.
/// @param lst the subject list.
/// @param indx the 0-based position.
/// @param data a pointer to the item to set, replacing the existing entry.
/// @return pointer to the item that was replaced.
/// @pre indx must be in the range [ 0...dll_size(lst) ).
/// @post data is the value of the list at the position of the index.
void * dll_set( DlList_T lst, int indx, void *data );

/// dll_pop removes the item at the index and returns its data pointer.
/// The function transfers ownership of the old memory to the client.
/// The client is responsible for the memory management of the data removed.
/// The cursor moves to the next position, if present. Otherwise the cursor
/// moves to the previous position in the list, if present; Otherwise the
/// cursor is invalid since this function deleted the last line in the list.
/// @param lst the subject list.
/// @param indx the 0-based position.
/// @return pointer to the item removed.
/// @pre indx must be in the range [ 0...dll_size(lst) ).
/// @post cursor moves to adjacent position or is invalid if list is now empty.
void * dll_pop( DlList_T lst, int indx );

/// dll_index returns a 0-based index of the data in the list or -1 if absent.
/// @param lst the subject list.
/// @param data a pointer to the item to find.
/// @return 0-based index of the data in the list or -1 if absent.
/// @post cursor position does not change. lst size and content is unchanged.
int dll_index( DlList_T lst, void *data );

/// dll_empty checks for an empty list.
/// @param lst: the subject list.
/// @return true if the list is empty.
/// @post cursor position does not change. lst size and content is unchanged.
bool dll_empty( DlList_T lst );

#endif

// // // // // // // // // // // // // // // // // // // // // // // //
// Revisions:
// $Log: dlList.h,v $
// Revision 1.1  2014/04/14 23:12:29  drw9888
// Added dlList.h
//
// Revision 1.2  2014/03/24 17:42:27  csci243
// updated dll_clear docs.
//
// Revision 1.1  2014/03/14 20:01:33  csci243
// Initial revision
//

