template<class T,class Sequence=deque<T>>
class stack{
    // 以下的 __STL_NULL_TMPL_ARGS 会开展为<>，见1.9.1节
    friend bool operator==__STL_NULL_TMPL_ARGS(const stack &,const stack &);
    friend bool operator<__STL_NULL_TMPL_ARGS(const stack &,const stack &);
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c; // 底层容器
public:
    // 以下完全利用 Sequence c 的操作，完成 stack 的操作
    bool empty() const{ return c.empty(); }
    size_type size() const{ return c.size(); }
    reference top(){ return c.back(); }
    const_reference top() const{ return c.back(); }
    // deque 是两头可进出， stack 是末端进，末端出（所以后进者先出）。
    void push(const value_type &x) { c.push_back(x); }
    void pop() { c.pop_back(); }
};

template<class T,class Sequence>
bool operator==(const stack<T,Sequence> &x,const stack<T,Sequence> &y){
    return x.c==y.c;
}

template<class T,class Sequence>
bool operator<(const stack<T,Sequence> &x,const stack<T,Sequence> &y){
    return x.c<y.c;
}