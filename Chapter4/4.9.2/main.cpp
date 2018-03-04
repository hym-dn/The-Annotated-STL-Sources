// 单向链表的节点基本结构
struct __slist_node_base{
    __slist_node_base *next;
};

// 单向链表的节点结构
template<class T>
struct __slist_node : public __slist_node_base{
    T data;
};

// 全局函数：已知某一节点，输入新节点于其后
inline __slist_node_base *__slist_make_link(
    __slist_node_base *prev_node,
    __slist_node_base *new_node){
    // 令 new 节点的下一节点为 prev 节点的下一节点
    new_node->next=prev_node->next;
    prev_node->next=new_node; // 令 prev 节点的下一节点指向 new 节点
    return new_node;
};

// 全局函数：单向链表的大小（元素个数）
inline size_t __slist_size(__slist_node_base *node){
    size_t result=0;
    for(;node!=0;node=node->next){
        ++result; // 一个一个累计
    }
    return result;
}