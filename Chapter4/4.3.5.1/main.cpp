template<class T,class Alloc=alloc> // 缺省使用alloc为配置器
class list{
protected:
    typedef __list_node<T> list_node;
    // 专属之空间配置器，每次配置一个节点大小
    typedef simple_alloc<list_node,Alloc> list_node_allocator;
    // ...
protected:
    //配置一个节点并传回
    link_type get_node() {return list_node_allocator::allocate();}
    //释放一个节点
    void put_node(link_type p){list_node_allocator::deallocate(p);}

    //产生（配置并构造）一个节点，带有元素值
    link_type create_node(const T &x){
        link_type p=get_node(); //分配一个节点
        construct(&p->data,x); //全局函数，构造/析构基本工具
        return(p);
    }
    //销毁（析构并释放）一个节点
    void destroy_node(link_type p){
        destroy(&p->data); // 全局函数，构造/析构基本工具
        put_node(p); // 释放一个节点
    }
public:
    list(){empty_initialize();} //产生一个空链表
    void push_back(const T &x){insert(end(),x);}
protected:
    void empty_initialize(){
        node=get_node(); // 配置一个节点空间，令node指向它
        node->next=node; // 令node头尾指向自己，不设元素值
        node->prev=node; 
    }
    //函数目的：在迭代器position所指位置插入一个节点，内容为x
    iterator insert(iterator position,const T &x){
        link_type tmp=create_node(x); // 产生一个节点（设妥内容为x）
        //调整双向指针，使tmp插入进去
        tmp->next=position.node;
        tmp->prev=position.node->prev;
        (link_type(position.node->prev))->next=tmp;
        position.node->prev=tmp;
        return tmp;
    }
}