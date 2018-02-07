#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>

struct node_t {
	size_t size;
	void *data;
	struct node_t *next;
};

struct list_t {
	size_t size;
	struct node_t *head;
	struct node_t *tail;
};

inline struct node_t * __attribute__ ((always_inline))
node_mem_alloc(size_t _sz)
{
	struct node_t *_new =
		malloc(sizeof(struct node_t));

	if (_new)
		_new->next = NULL;

	_new->data = calloc(_sz, sizeof(char));
	_new->size = _sz;

	return _new;
}

inline void __attribute__ ((always_inline))
list_mem_zero(struct list_t *_list)
{
	_list->size = 0;
	_list->head = NULL;
	_list->tail = NULL;
}

inline void __attribute__ ((always_inline))
list_mem_free(struct list_t *_list)
{
	struct node_t *_del = _list->head;

	while (_del) {
		_list->head = _del->next;
		free(_del->data);
		free(_del);
		_del = _list->head;
	}

	list_mem_zero(_list);
}

inline void __attribute__ ((always_inline))
list_init(struct list_t *_list)
{
	list_mem_zero(_list);
}

inline void __attribute__ ((always_inline))
list_add_data(struct list_t *_list, void *_data, size_t _sz)
{
	struct node_t *_add = node_mem_alloc(_sz);

	if (!_add) return;

	_add->size = _sz;
	memcpy(_add->data, _data, _sz);

	if (!_list->head) _list->tail = _add;

	_add->next = _list->head;
	_list->head = _add;
	_list->size++;
}

#endif
