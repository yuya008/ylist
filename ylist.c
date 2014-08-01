#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ylist.h"

static inline ylist_node *new_list_node(void *ptr)
{
    ylist_node *node = malloc(sizeof(ylist_node));
    if (!node) {
        fprintf(stderr, "%s\n", "init ylist_node oom");
        exit(1);
    }
    memset(node, 0, sizeof(ylist_node));
    node->data_ptr = ptr;
    return node;
}

static inline ylist_node *find_node_index(ylist_t *l, uint64_t index)
{
	ylist_node *node = NULL;
	uint64_t i = 0;
	for (node = l->head; node; node = node->next, i++) {
		if (i == index) {
			break;
		}
	}
	return node;
}

static inline ylist_node *find_node(ylist_t *l, void *d)
{
	ylist_node *node = NULL;
	for (node = l->head; node; node = node->next) {
		if (node->data_ptr == d) {
			break;
		}
	}
	return node;
}

static inline int remove_node(ylist_t *l, ylist_node *node)
{
	if (!node) {
		return 1;
	}
    if (!node->pre && !node->next) {
        l->tail = l->head = NULL;
        goto ret;
    }
    if (!node->pre && node->next) {
        node->next->pre = NULL;
        l->head = node->next;
        goto ret;
    }
    if (node->pre && !node->next) {
        node->pre->next = NULL;
        l->tail = node->pre;
        goto ret;
    }
    node->next->pre = node->pre;
    node->pre->next = node->next;
    ret:
    free(node);
    l->count--;
	return 0;
}

ylist_t *ylist_init()
{
    ylist_t *list = malloc(sizeof(ylist_t));
    if (!list) {
        fprintf(stderr, "%s\n", "init ylist_t oom");
        exit(1);
    }
    memset(list, 0, sizeof(ylist_t));
    return list;
}

uint64_t ylist_size(ylist_t *list)
{
    return list->count;
}

int ylist_add(ylist_t *list, void *e)
{
    if (!e) {
        return 1;
    }
    ylist_node *node = new_list_node(e);
    if (!list->head && !list->tail) {
    	list->cur = list->head = list->tail = node;
    } else {
        list->tail->next = node;
        node->pre = list->tail;
        list->tail = node;
    }
    list->count++;
    return 0;
}

int ylist_clear(ylist_t *l)
{
	ylist_node *free_ptr = NULL, *node = NULL;
	if (!l->head) {
		return 1;
	}
	for (node = l->head; node;) {
		free_ptr = node;
		node = node->next;
		free(free_ptr);
	}
	l->head = l->tail = l->cur = NULL;
	l->count = 0;
	return 0;
}

inline void *ylist_next(ylist_t *l)
{
	if (!l->cur) {
		return NULL;
	}
	ylist_node *node = l->cur;
	l->cur = l->cur->next;
	return node->data_ptr;
}

inline void ylist_reset(ylist_t *l)
{
	l->cur = l->head;
}

int ylist_insert_before(ylist_t *l, uint64_t index, void *e)
{
	if (!l->head && !l->tail) {
		return ylist_add(l, e);
	}

	ylist_node *node = find_node_index(l, index);
	if (!node) {
		return 1;
	}

	ylist_node *new_node = new_list_node(e);
	if (!node->pre) {
		new_node->next = node;
		node->pre = new_node;
		l->head = new_node;
	} else {
		node->pre->next = new_node;
		new_node->next = node;
		new_node->pre = node->pre;
		node->pre = new_node;
	}
	l->count++;
	return 0;
}

int ylist_insert_after(ylist_t *l, uint64_t index, void *e)
{
	if (!l->head && !l->tail) {
		return ylist_add(l, e);
	}
	ylist_node *node = find_node_index(l, index);
	if (!node) {
		return 1;
	}
	ylist_node *new_node = new_list_node(e);
	if (!node->next) {
		node->next = new_node;
		new_node->pre = node;
		l->tail = new_node;
	} else {
		node->next->pre = new_node;
		new_node->next = node;
		new_node->pre = node->next;
		node->next = new_node;
	}
	l->count++;
	return 0;
}

ylist_t *ylist_clone(ylist_t *l)
{
	ylist_t *newlist = ylist_init();
	ylist_node *node = NULL;

	for (node = l->head; node; node = node->next) {
		if (ylist_add(newlist, node->data_ptr)) {
			return NULL;
		}
	}
	return newlist;
}

int64_t ylist_index_of(ylist_t *l, void *d)
{
	ylist_node *node = NULL;
	int64_t i = 0;
	for (node = l->head; node; node = node->next,i++) {
		if (node->data_ptr == d) {
			return i;
		}
	}
	return -1;
}

int ylist_remove_index(ylist_t *l, uint64_t index)
{
	ylist_node *node = find_node_index(l, index);
	if (!node) {
		return 1;
	}
	return remove_node(l, node);
}

int ylist_remove(ylist_t *l, void *d)
{
	ylist_node *node = find_node(l, d);
	if (!node) {
		return 1;
	}
	return remove_node(l, node);
}

int ylist_set(ylist_t *l, uint64_t index, void *d)
{
	ylist_node *node = find_node_index(l, index);
	if (!node) {
		return 1;
	}
	node->data_ptr = d;
	return 0;
}

ylist_t *ylist_sublist(ylist_t *l, uint64_t start, uint64_t end)
{
	if (start > end) {
		return NULL;
	}
	ylist_node *node = find_node_index(l, start);
	if (!node) {
		return NULL;
	}
	ylist_t *newlist = ylist_init();
	uint64_t i = start;
	for (; node && i <= end; node = node->next,i++) {
		if (ylist_add(newlist, node->data_ptr)) {
			return NULL;
		}
	}
	return newlist;
}

void ylist_sort(int (*comparator)(void *, void *))
{

}

int main(void)
{
	return 0;
}
