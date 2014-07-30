#include <stdio.h>
#include <stdlib.h>

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
    if (!list->head && !list->cur) {
        list->head = list->cur = node;
    } else {
        list->cur->next = node;
        node->pre = list->cur;
        list->cur = node;
    }
    list->count++;
    return 0;
}
