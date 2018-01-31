template<class T,class Ref,class Ptr,size_t BufSiz>
struct __deque_iterator{ // 未继承 std::iterator

    //定义iterator类型
    typedef __deque_iterator<T,T&,T*,BufSiz> iterator;
    typedef __deque_iterator<T,const T &,const T *,BufSiz> const_iterator;
    //缓冲区尺寸
    static size_t buffer_size(){return(__deque_buf_size(BufSiz,sizeof(T)));}
    // 未继承std::iterator，所以必须自行撰写五个必要的迭代器相应型别（第3章）
    typedef random_access_iterator_tag iterator_category; // (1)
    typedef T value_type; // (2)
    typedef Ptr pointer; // (3)
    typedef Ref reference; // (4)
    typedef size_t size_type;
    typedef ptrdiff_t difference_type; //(5)
    typedef T** map_pointer;

    typedef __deque_iterator self;

    // 保持与容器的联结
    T *cur; // 此迭代器所指之缓冲区的现行（current）元素
    T *first; // 此迭代器所指之缓冲区的头
    T *last; // 此迭代器所指之缓冲区的尾（含备用空间）
    map_pointer node; //指向管控中心

    // ...

    // 更新当前迭代器中记录的节点
    void set_node(map_pointer new_node){
        node=new_node;
        first=*new_node;
        last=first+difference_type(buffer_size());
    }

    // 以下各个重载运算子是__deque_iterator<>成功运作的关键
    reference operator*() const { return *cur; }
    pointer operator->() const { return &(operator*()); }

    //当前迭代器与x迭代器的距离
    difference_type operator-(const self &x) const{
        //这里要注意指针加减的定义，不是按照字节定义的，而是按照指向元素类型定义的
        return difference_type(buffer_size())*(node-x.node-1)+(
            cur-first)+(x.last-x.cur);
    }

    // 参考 More Effective C++，item 6: Distinguish between prefix and
    // posix forms of increment and decrement operators
    self &operator++(){
        ++cur; //切换至下一个元素
        if(cur==last){ //如果已达所在缓冲区的尾端
            set_node(node+1); //就切换至下一节点（亦即缓冲区）
            cur=first; // 更新当前元素
        }
        return *this;
    }
    self operator++(int){//后置式，标准写法
        self tmp=*this;
        ++*this; //调用self &operator++函数
        return tmp;
    }
    self &operator--(){
        if(cur==first){// 如果已达所在缓冲区的头端，
            set_node(node-1); // 就切换至前一节点（亦即缓冲区）
            cur=last; // 更新当前元素    
        }
        --cur; //切换至前一个元素
        return *this;
    }
    self operator--(int){// 后置式，标准写法
        self tmp=*this;
        --*this; // 调用self &operator--()函数
        return tmp;
    }

    // 以下实现随机存取。迭代器可以直接跳跃n个距离
    self &operator+=(difference_type n){
        // 计算偏移
        difference_type offset=n+(cur-first);
        // 目标位置在同一缓冲区内
        if(offset>=0&&offset<difference_type(buffer_size)){
            cur+=n;
        }
        // 目标位置不再同一缓冲区内
        else{
            // 计算新节点在控制中心的偏移
            difference_type node_offset=
                offset>0?offset/difference_type(buffer_size())
                    :-difference_type((-offset-1)/buffer_size())-1;
            // 切换至正确的节点（亦即缓冲区）
            set_node(node+node_offset);
            // 切换至正确的元素
            cur=first+(offset-node_offset*difference_type(buffer_size()));
        }
        //
        return(*this);
    }

    // 参考 More Effective C++，item22: Consider using op=instead of
    // stand-alone op
    self operator+(difference_type n) const{
        self tmp=*this;
        return tmp+=n; //调用operator+=
    }

    //利用operator+=来完成operator-=
    self &operator-=(difference_type n){return *this+=-n;}

    // 参考 More Effective C++，item22: Consider usi g op= instead of
    // stand-alone op.
    self operator-(difference_type n) const{
        self tmp=*this;
        return tmp-=n; // 调用operator-=
    }

    // 以下实现随机存取。迭代器可以直接调与n个距离
    // 调用operator*,operator+
    reference operator[](difference_type n) const{ return *(*this+n); }

    bool operator==(const self &x) const{ return cur==x.cur; }
    bool operator!=(const self &x) const{ return !(*this==x); }
    bool operator<(const self &x) const{
        return(node==x.node)?(cur<x.cur):(node<x.node);
    }

    

};

// 如果n不为0，传回n，表示buffer size由用户自定义
// 如果n为0,表示buffer size使用默认值，那么
// 如果sz(元素太小，sizeof(value_type))小于512，
// 传回512/sz，如果sz不小于512，传回1
inline size_t __deque_buf_size(size_t n,size_t sz){
    return n!=0?n:(sz<512?size_t(512/sz):size_t(1));
}