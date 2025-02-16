#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include <stdint.h>
#include <stddef.h>
#include <cmsis_compiler.h>

#define MAX_NODES 128  // 链表中最大节点数量

/*
在单片机环境中，由于无法使用动态内存分配（如 malloc），
我们可以通过预先分配固定大小的内存池来实现链表。每当需要添加新节点时，
我们从这个内存池中获取一个空闲节点；当删除节点时，我们将节点返回到内存池中。
*/

typedef struct Node 
{
		void *data;    //通用数据指针，使用的时候做强制类型转换
    struct Node *next;
} Node;

typedef struct 
{
    Node *head;
    Node *tail;
}LinkedList;

void Linked_List_Init(LinkedList *list);
int Linked_List_Add(LinkedList *list, void *data);
int Linked_List_Remove(LinkedList *list, void *data);
int Linked_List_Is_Empty(LinkedList *list);
void Linked_List_Clear(LinkedList *list);
void Linked_List_Test(void);

#endif
