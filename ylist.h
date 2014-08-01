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
    ylist_node *tail;
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
inline void *ylist_next(ylist_t *);

/* ylist reset cur指针 */
inline void ylist_reset(ylist_t *);

/* ylist加入一个元素 */
int ylist_add(ylist_t *, void *);

/* ylist加入一个元素到某个索引的元素之前 */
int ylist_insert_before(ylist_t *, uint64_t, void *);

/* ylist加入一个元素到某个索引的元素之后 */
int ylist_insert_after(ylist_t *, uint64_t, void *);

/* 完全克隆一个新的ylist对象 */
ylist_t *ylist_clone(ylist_t *);

/* 通过索引得到元素 */
void *ylist_get_index(uint64_t);

/* 获得元素的索引值 */
int64_t ylist_index_of(ylist_t *, void *);

/* 按索引删除元素 */
int ylist_remove_index(ylist_t *, uint64_t);

/* 按元素删除元素 */
int ylist_remove(ylist_t *, void *);

/* 修改元素 */
int ylist_set(ylist_t *, uint64_t, void *);

/* 排序 */
void ylist_sort(int (*comparator)(void *, void *));

/* 返回一个全新的子list */
ylist_t *ylist_sublist(ylist_t *, uint64_t, uint64_t);


#endif
