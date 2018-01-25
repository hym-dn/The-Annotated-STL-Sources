//alloc 是SGI STL的空间配置器，见第二章
template<class T,class Alloc=alloc>
class vector{
public:
    //vector 的嵌套型别定义
    typedef T value_type;
    typedef value_type *pointer;
    typedef value_type *iterator;
    typedef value_type &reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
protected:
    //以下，simple_alloc 是SGI STL的空间配置器，见2.2.4节
    typedef simple_alloc<value_type,Alloc> data_allocator
    iterator start; //表示目前使用的空间的头
    iterator finish; //表示目前使用空间的尾
    iterator end_of_storage; //表示目前可用空间的尾
    //在指定位置插入一个元素
    void insert_aux(iterator position,const T &x);
    //释放全部空间
    void deallocate(){
        if(start){
            data_allocator::deallocate(start,end_of_storage-start);
        }
    }
    //指定内容填充
    void fill_initialize(size_type n,const T &value){
        start=allocate_and_fill(n,value);
        finish=start+n;
        end_of_storage=finish;
    }
public:
    //起始迭代器
    iterator begin(){return start;}
    //结束迭代器
    iterator end(){return finish;}
    //容器尺寸
    size_type size() const{return(size_type(end()-begin()));}
    //容器容量
    size_type capacity() const{
        return(size_type(end_of_storage-begin()));
    }
    //容器是否为空
    bool empty() const{return(begin()==end());}
    //重载[]操作符，取下标
    reference operator[](size_type n){return(*(begin()+n));}
    //构造函数
    vector():start(0),finish(0),end_of_storage(0){}
    vector(size_type n,const T &value){fill_initialize(n,value);}
    vector(int n,const T &value){fill_initialize(n,value);}
    vector(long n,const T &value){fill_initialize(n,value);}
    explicit vector(size_type n){fill_initialize(n,T());}
    //析构函数
    ~vector(){
        destroy(start,finish); //全局函数，见2.2.3节
        deallocate(); //这是vector的一个member function
    }
    //获取首、尾
    reference front(){return(*begin());} //第一个元素
    reference back(){return(*(end()-1));} //最后一个元素
    //将元素插入尾部
    void push_back(const T &x){//将元素插入至最后尾端
        if(finish!=end_of_storage){//还有剩余空间
            //在指定空间中构造相应对象
            construct(finish,x);//全局函数，见2.2.3节
            //尾部递增
            ++finish;
        }
    }
    //弹出尾部
    void pop_back(){ //将最尾端元素取出
        //尾部自减
        --finish;
        //销毁指定元素
        destroy(finish); //全局函数，见2.2.3节
    }
    //擦除指定元素
    iterator erase(iterator position){ //清除某位置上的元素
        //并非最后一个元素
        if(position+1!=end()){
            copy(position+1,finish,position);//后续元素往前移动
        }
        //尾部递减
        --finish;
        //销毁元素
        destroy(finish); //全局函数，见2.2.3节
        //返回
        return(position);
    }
    //重新调整容器尺寸
    void resize(size_type new_size,const T &x){
        //如果新尺寸小于当前尺寸
        if(new_size<size()){
            erase(begin()+new_size,end());//擦除
        }else{
            insert(end(),new_size-size(),x);//插入
        }
    }
    void resize(size_type new_size){resize(new_size,T());}
    //清除
    void clear(){erase(begin(),end());}
protected:
    //配置空间并填充内容
    iterator allocate_and_fill(size_type n,const T &x){
        //分配内存
        iterator result=data_allocator::allocate(n);
        //初始化指定内存
        uninitialized_fill_n(result,n,x);//全局函数，见2.3节
        //返回
        return(result);
    }
};