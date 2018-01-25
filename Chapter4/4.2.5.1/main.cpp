
template<class T,class Alloc=alloc>
class vector{
protected:
    // simple_alloc<> 见2.2.4节
    typedef simple_alloc<value_type,Alloc> data_allocator;
    // ...
};

//构造函数，允许指定vector大小n的初值value
vector(size_type n,const T &value){fill_initialize(n,value);}

//填充并予初始化
void fill_initialize(size_type n,const T &value){
    start=allocate_and_fill(n,value);
    finish=start+n;
    end_of_storage=finish;
}

//配置而后填充
iterator allocate_and_fill(size_type n,const T &x){
    iterator result=data_allocator::allocate(n);//配置n个元素空间
    uninitialized_fill_n(result,n,x);//全局函数，见2.3节
    return(result);
}

void push_back(const T &x){
    if(finish!=end_of_storage){//还有备用空间
        construct(finish,x);//全局函数，见2.2.3节
        ++finish; //调整水位高度
    }else{//无备用空间
        insert_aux(end(),x);//vector member function,见一下列表
    }
}

template<class T,class Alloc>
void vector<T,Alloc>::insert_aux(iterator position,const T &x){
    if(finish!=end_of_storage){//还有备用空间
        //在备用空间起始处构造一个元素，并以vector最后一个元素值为其初值
        construct(finish,*(finish-1));
        //调整水位
        ++finish;
        T x_copy=x;
        copy_backward(position,finish-2,finish-1);
        *position=x_copy;
    }else{//已无备用空间
        const size_type old_size=size();
        const size_type len=old_size!=0?2*old_size:1;
        //以上配置原则：如果远大小为0,则配置1（个元素大小）;
        //如果原大小不为0,则配置原大小的两倍，
        //前半段用来放置原数据，后半段准备用来放置新数据
        iterator new_start=data_allocator::allocate(len);//实际配置
        iterator new_finish=new_start;
        try{
            //将原vector的内容拷贝到新vector
            new_finish=uninitialized_copy(start,position,new_start);
            //为新元素设定初值x
            construct(new_finish,x);
            //调整水位
            ++new_finish;
            //将安插点的原内容也拷贝过来（提示：本函数也可能被insert(p,x)调用）
            new_finish=uninitialized_copy(position,finish,new_finish);
        }catch(...){
            // "commit or rollback" semantics.
            destroy(new_start,new_finish);
            data_allocator::deallocate(new_start,len);
            throw;
        }
        //析构并释放原vector
        destroy(begin(),end());
        deallocate();
        //调整迭代器，指向新vector
        start=new_start;
        finish=new_finish;
        end_of_storage=new_start+len;
    }
}