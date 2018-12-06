/********************************************************************
 * PROGRAM: yafm
 * FILE: list.h
 * PURPOSE: simple linked list impl
 * AUTHOR: 5aboteur <5aboteur@protonmail.com>
 *******************************************************************/

#ifndef __LIST_H__
#define __LIST_H__

#include <assert.h>
#include <stdlib.h>

/* Node structure */
struct node_t {
	int index;				// index in the list
	size_t size;			// size of data
	void *data;				// payload
	struct node_t *next;	// next node
};

/* List structure */
struct list_t {
	size_t size;			// list length
	struct node_t *head;	// first elem
	struct node_t *tail;	// last elem
};

/*
 * Function: node_mem_alloc
 * ------------------------
 * Description:
 *  Node memory allocation.
 *
 * Arguments:
 *  '_sz' - size of data to be alloc'd.
 *
 * Asserts:
 *  'malloc' won`t return NULL.
 *
 * Returns:
 *  Newly allocated node.
 */

inline struct node_t *
__attribute__ ((always_inline))
node_mem_alloc(size_t _sz)
{
	struct node_t *_new =
		malloc(sizeof(struct node_t));

	assert(_new != NULL);

	_new->size = _sz;
	_new->data = calloc(_sz + 1, sizeof(char));
	_new->next = NULL;

	return _new;
}

/*
 * Function: list_mem_zero
 * -----------------------
 * Description:
 *  Resets the list.
 *
 * Arguments:
 *  '_list' - list.
 *
 * Asserts:
 *  '_list' isn`t NULL.
 */

inline void __attribute__ ((always_inline))
list_mem_zero(struct list_t *_list)
{
	assert(_list != NULL);
	_list->size = 0;
	_list->head = NULL;
	_list->tail = NULL;
}

/*
 * Function: list_mem_free
 * -----------------------
 * Description:
 *  Frees list memory.
 *
 * Arguments:
 *  '_list' - list.
 *
 * Asserts:
 *  '_list' isn`t NULL.
 */

inline void __attribute__ ((always_inline))
list_mem_free(struct list_t *_list)
{
	assert(_list != NULL);
	struct node_t *_nxt, *_del = _list->head;

	while (_del) {
		_nxt = _del->next;
		free(_del->data);
		free(_del);
		_del = _nxt;
	}

	list_mem_zero(_list);
}

/*
 * Function: list_init
 * -------------------
 * Description:
 *  List initialization.
 *
 * Arguments:
 *  '_list' - list.
 */

inline void __attribute__ ((always_inline))
list_init(struct list_t *_list)
{
	list_mem_zero(_list);
}

/*
 * Function: list_add_data
 * -----------------------
 * Description:
 *  Inserts data into the list.
 *
 * Arguments:
 *  '_list' - list;
 *  '_idx' - position in list;
 *  '_data' - payload;
 *  '_sz' - size of payload.
 *
 * Asserts:
 *  '_list' isn`t NULL.
 */

inline void __attribute__ ((always_inline))
list_add_data(struct list_t *_list, int _idx,
	void *_data, size_t _sz)
{
	assert(_list != NULL);
	struct node_t *_add = node_mem_alloc(_sz);

	_add->index = _idx;
	memcpy(_add->data, _data, sizeof(char) * (_sz + 1));
	_add->next = NULL;

	if (!_list->head) {
		_list->head = _add;
		_list->size++;
		return;
	}

	struct node_t *_end = _list->head;
	for (; _end->next; _end = _end->next);
	_end->next = _add;
	_list->size++;
}

/*
 * Function: list_find_data
 * ------------------------
 * Description:
 *  Searches data by index in the list.
 *
 * Arguments:
 *  '_list' - list;
 *  '_idx' - position in list;
 *
 * Asserts:
 *  '_list' isn`t NULL.
 *
 * Returns:
 *  Data payload if it was found, NULL otherwise.
 */

inline void * __attribute__ ((always_inline))
list_find_data(struct list_t *_list, int _idx)
{
	assert(_list != NULL);
	struct node_t *_find = _list->head;
	for (; _find && _find->index != _idx;
		_find = _find->next);
	return _find ? _find->data : NULL;
}

#endif
