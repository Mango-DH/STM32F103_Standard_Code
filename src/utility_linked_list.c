#include "utility_linked_list.h"
#include <string.h>

static Node nodes_pool[MAX_NODES]; // 静态内存池,实际上是一个Node类型的数组
static int nodes_pool_used[MAX_NODES] = {0}; // 判断节点是否被使用

/*
以下代码实现了一个简单的内存管理机制，
用于在没有动态内存分配（如 malloc）的环境中（如单片机）管理链表节点
具体来说，这部分代码负责分配和释放链表节点，通过静态内存池来实现
*/
static Node* get_free_node()          //寻找可用节点，static静态化函数
{
    for (int i = 0; i < MAX_NODES; i++) 
		{
        if (!nodes_pool_used[i]) 
				{
            nodes_pool_used[i] = 1;
            return &nodes_pool[i];    //一旦找到可用节点就跳出循环
        }
    }
    return NULL; // 没有可用的节点
}

static void release_node(Node *node)    //释放节点
{
    int index = node - nodes_pool;
	
/*  
	int index = node - nodes_pool;
	nodes_pool 指针指向的是什么呢？？？？？？？？？？？？？？？？？？？
	nodes_pool 指针指向的是整个内存池的起始位置，也就是数组 nodes_pool 的第一个元素
	
	计算需要释放的节点在内存池中的索引。
	这里利用指针减法的特性，计算 node 指针与 nodes_pool 指针的差值，
	得到该节点在 nodes_pool 数组中的位置（索引）
	例如，如果 node 是 nodes_pool 中第 3 个节点的地址，那么 index 的值就是 2
*/
	  if (index >= 0 && index < MAX_NODES) 
		{
        nodes_pool_used[index] = 0;//该节点被释放，后续可以分配给其他数据
    }
}


/**
* @name:  Linked_List_Init 
* @brief: 链表初始化
* @param: list  传入操作的链表
* @reval: None
*/
void Linked_List_Init(LinkedList *list) 
{
    list->head = list->tail = NULL;
}

/**
* @name:  Linked_List_Add 
* @brief: 从内存池中分配新节点，并将其添加到链表尾部
* @param: list   传入操作的链表
* @param: data   要追加的数据节点
* @reval: 操作是否成功,成功返回1,否则返回-1 
*/
int Linked_List_Add(LinkedList *list, void* data) 
{
    Node *new_node = get_free_node();   //得到一个可用的节点
    if(!new_node) 
		{
        return -1; // 没有可用节点
    }

    new_node->data = data;
    new_node->next = NULL;	

    if(list->tail)    //当前链表的尾部不是空指针
		{
        list->tail->next = new_node;
    }
		else   //如果链表尾部是个空指针，即链表为空
		{
				list->head = new_node;
    }				
		list->tail = new_node;
    return 1;
}

/**
* @name:  Linked_List_Remove 
* @brief: 找到并删除链表中匹配的数据节点，将其释放回内存池
* @param: list   传入操作的链表
* @param: data   要删除的数据节点
* @reval: 操作是否成功,成功返回1,否则返回-1 
*/
int Linked_List_Remove(LinkedList *list, void *data) 
{
    if (!list || !list->head) 
    {
        return -1;  // 空链表或无效链表
    }

    Node *prev = NULL;
    Node *curr = list->head;

    while (curr) 
    {
        if (curr->data == data) 
        {
            if (prev) 
            {
                // 当前节点不是头节点
                prev->next = curr->next;
            } 
            else 
            {
                // 当前节点是头节点
                list->head = curr->next;
            }

            if (curr == list->tail) 
            {
                // 当前节点是尾节点
                list->tail = prev;
            }

            // 释放节点内存
            release_node(curr); 

            // 如果链表变为空，确保尾指针也被设置为 NULL
            if (list->head == NULL) 
            {
                list->tail = NULL;
            }

            return 1;  // 成功删除节点
        }
        prev = curr;        // 更新前一个节点为当前节点
        curr = curr->next; // 移动到下一个节点
    }
    return -1;  // 未找到匹配节点

}

/**
* @name:  Linked_List_Is_Empty 
* @brief: 判断传入链表是否为空
* @param: list   传入操作的链表
* @reval: 链表为空返回1 ，否则返回0
*/
int Linked_List_Is_Empty(LinkedList *list) 
{
    return list->head == NULL;
	/*
		list->head == NULL 判断链表的头指针是否为 NULL，即链表是否为空
		如果 head 为 NULL，则链表为空，函数返回 1（表示真），否则返回0
		这竟然是个条件判断语句!
	*/
}


/**
* @name:  Linked_List_Clear 
* @brief: 清除传入的链表
* @param: list   传入操作的链表
* @reval: None
*/
void Linked_List_Clear(LinkedList *list) 
{
    Node *curr = list->head;
    while (curr)
		{
        Node *next = curr->next;
        release_node(curr);
        curr = next;
    }
    list->head = list->tail = NULL;
}


/**
* @name:  Linked_List_Test 
* @brief: 测试本文档代码
* @reval: None
*/
void Linked_List_Test(void)
{
	LinkedList list;
  int data1 = 10;
  float data2 = 3.1415;
  char data3 = 'a';

	volatile int node_data;          //存储节点的数据，方便调试

	
	Linked_List_Init(&list);
	int is_empty = Linked_List_Is_Empty(&list);   //定义一个变量检测
	__NOP();	
	
	Linked_List_Add(&list, &data1);
	Linked_List_Add(&list, &data2);
	Linked_List_Add(&list, &data3);
	is_empty = Linked_List_Is_Empty(&list);   
	__NOP();
	
	Node *node = list.head;	          //每次进入循环前保证节点在链表头处
  while (node)                      //遍历链表的方法
	{
    node_data = *(int *)node->data ;
		__NOP();     //检查node_data的值
    node = node->next;
  }
	
	
  Linked_List_Remove(&list, &data2);
	
	node = list.head;	//每次进入循环前保证节点在链表头处
  while (node)                      //遍历链表的方法
	{
    node_data = *(int *)node->data ;
		__NOP();     //检查node_data的值
    node = node->next;
  }

  Linked_List_Clear(&list);        //清空列表
	is_empty = Linked_List_Is_Empty(&list);   
	__NOP();    			                //检查is_empty的值

}

