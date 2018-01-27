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
        link_type p=get_node();
        construct(&p->data,x); //全局函数，构造/析构基本工具
        return(p);
    }
    //销毁（析构并释放）一个节点
    void destroy_node(link_type p){
        destroy(&p->data); // 全局函数，构造/析构基本工具
        put_node(p);
    }
}