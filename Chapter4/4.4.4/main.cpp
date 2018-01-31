// 见 __deque_buf_size()。BufSize默认值为0的唯一理由是为了闪避
// 某些编译器在处理常数算式（constant expressions）时的臭虫
// 缺省使用alloc为配置器
template<class T,class Alloc=alloc,size_t BufSiz=0>
class deque{

public: // Basic types
    typedef T value_type;
    typedef value_type *pointer;
    typedef size_t size_type;
public: // Iterators
    typedef __deque_iterator<T,T&,T*,BufSiz> iterator;
protected: // Internal typedefs
    // 元素的指针的指针（pointer of pointer of T）
    typedef pointer* map_pointer;
protected: // Data members
    iterator start; // 表现第一个节点
    iterator finish; // 表现最后一个节点
    
    map_pointer map; // 指向map,map是块连续空间，
                     // 其每个元素都是个指针，指向一个节点（缓冲区）
    size_type map_size; // map内有所少指针

    // ...

public: // Basic accessors
    iterator begin() { return start; }
    iterator end() { return finish; }

    reference operator[](size_type n){
        return start[difference_type(n)]; // 调用__deque_iterator<>::operator[]
    }

    reference front(){ return *start; } // 调用 __deque_iterator<>::operator*

    reference back(){
        iterator tmp=finish;
        --tmp; // 调用 __deque_iterator<>::operator--
        return *tmp; // 调用 __deque_iterator<>::operator *
        // 以上三个何不改为： return *(finish-1);
        // 因为 __deque_iterator<> 没有为(finish-1)定义运算子？！
    }

    // 以下最后有两个';'，虽奇怪但合乎语法
    size_type size() const{ return finish-start;; }
    // 以上调用iterator::operator-
    size_type max_size() const{ return size_type(-1); }
    bool empty() const{ return finish=start; }

};