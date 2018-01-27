template<class T,class Alloc=alloc> //缺省使用alloc为配置器
class list{
protected:
    typedef __list_node<T> list_node;
public:
    typedef list_node *link_type;
protected:
    link_type node; // 只要一个指针，便可表示整个环状双向链表
public:
    //开始元素
    iterator begin(){return((link_type)((*node).next));}
    //结束元素
    iterator end(){return(node);}
    //链表是否为空
    bool empty() const{return node->next==node;}
    //计算链表尺寸
    size_type size() const{
        size_type result=0;
        distance(begin(),end(),result);//全局函数，第3章
        return result;
    }
    //取头节点的内容（元素值）
    reference front(){return *begin();}
    //取尾节点的内容（元素值）
    reference back(){return *(--end());}
};