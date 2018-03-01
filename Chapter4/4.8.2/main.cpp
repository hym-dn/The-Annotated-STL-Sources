template<class T,class Sequence=vector<T>,
    class Compare=less<typename Sequence::value_type>>
class priority_queue{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c; // 底层容器
    Compare comp; // 元素大小比较标准
public:
    priority_queue():c(){}
    explicit priority_queue(const Compare &x):c(),comp(x){}

    // 以下用到的make_heap(),push_heap(),pop_heap()都是泛型算法
    // 注意，任一个构造函数都立刻于底层容器内产生一个implicit representation heap
    template<class InputIterator>
    priority_queue(InputIterator first,InputIterator last,const Compare &x)
        :c(first,last),comp(x){
        make_heap(c.begin(),c.end(),comp);
    }
    template<class InputIterator>
    priority_queue(InputIterator first,InputIterator last)
        :c(first,last){
        make_heap(c.begin(),c.end(),comp);
    }

    bool empty() const{return c.empty();}
    size_type size() const{return c.size();}
    const_reference top() const{return c.front();}
    void push(const value_type &x){
        __STL_TRY{
            // push_heap 是泛型算法，先利用底层容器的push_back()将新元素
            // 推入末端，再重排heap,见C++ Primer p.1195
            c.push_back(x);
            push_heap(c.begin(),c.end(),comp); // push_heap 是泛型算法
        }
        __STL_UNWIND(c.clear());
    }
};