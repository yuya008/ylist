#ifndef __ylist_h__
#define __ylist_h__

#include <stdint.h>

typedef struct ylist_t    ylist_t;
typedef struct ylist_node ylist_node;

/* ylist主结构 */
struct ylist_t
{
    uint64_t count;
    ylist_node *head;
    ylist_node *cur;
};

/* ylist节点 */
struct ylist_node
{
    ylist_node *pre;
    ylist_node *next;
    void *data_ptr;
};

/* ylist主结构初始化 */
ylist_t *ylist_init();
/* ylist元素总数量 */
uint64_t ylist_size(ylist_t *);
/* ylist清除所有 */
int ylist_clear(ylist_t *);
/* ylist下一个元素 */
void *ylist_next(ylist_t *);
/* ylist加入一个元素 */
int ylist_add(ylist_t *，void *);

int ylist_insert(ylist_t *，void *);
#endif