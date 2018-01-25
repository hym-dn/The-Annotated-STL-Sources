//将尾端元素拿掉,并调整大小
void pop_back(){
    --finish; //将尾端标记往前移一格，表示将放弃尾端元素
    //调用析构函数
    destroy(finish); //destroy 是全局函数，见第2章
}

//清除[first,last)中的所有元素
iterator erase(iterator first,iterator last){
    //拷贝last到finish的内容到first上
    iterator i=copy(last,finish,first);// copy是全局函数，第6章
    //调用i~finish的析构函数
    destroy(i,finish); // destroy是全局函数，第2章。
    //更新结尾
    finish=finish-(last-first);
    //返回
    return(first);
}

//清除某个位置上的元素
iterator erase(iterator position){
    //如果不是最后一个元素
    if(position+1!=end()){
        //拷贝position+1到finish之间的数据，到position上
        copy(position+1,finish,position);// copy是全局函数，第6章
    }
    //更新结尾
    --finish;
    //调用析构函数
    destroy(finish); // destroy是全局函数,2.2.3节
    //返回
    return(position);
}

//清除全部内容
void clear(){ 
    erase(begin(),end()); // erase()就定义在上面
} 

//从position开始，插入n个元素，元素初值为x
template<class T,class Alloc>
void vector<T,Alloc>::insert(iterator position,size_type n,const T &x){
    if(n!=0){//当n!=0才进行以下所有操作
        if(size_type(end_of_storage-finish)>=n){
            //备用空间大于等于“新增元素个数”
            T x_copy=x;
            //以下计算插入点之后的现有元素个数
            const size_type elems_after=finish-position;
            iterator old_finish=finish;
            if(elems_after>n){
                //"插入点之后的现有元素个数"大于“新增元素个数”
                uninitialized_copy(finish-n,finish,finish);//将finish-n挪动到finish
                finish+=n;//将vector尾端标记后移
                copy_backward(position,old_finish-n,old_finish);//将position拷贝到old_finish
                fill(position,position+n,x_copy);//从插入点开始填入新值
            }else{
                //"插入点之后的现有元素个数"小于等于“新增元素个数”
                uninitialized_fill_n(finish,n-elems_after,x_copy);//在尾端插入新元素
                //更新尾端
                finish+=n-elems_after;
                //将position挪到finish
                uninitialized_copy(position,old_finish,finish);
                //更新尾端
                finish+=elems_after;
                //填充新值
                fill(position,old_finish,x_copy);
            }
        }else{
            //备用空间小于“新增元素个数”（那就必须配置额外的内存）
            //首先决定新长度：旧长度的两倍，或旧长度+新元素个数
            const size_type old_size=size();
            const size_type len=old_size+max(old_size,n);
            //以下配置新的vector空间
            iterator new_start=data_allocator::allocate(len);
            iterator new_finish=new_start;
            __STL_TRY{
                //以下首先将旧vector的插入点之前的元素复制到新空间
                new_finish=uninitialized_copy(start,position,new_start);
                //以下再将新增元素（初值皆为n）填入新空间
                new_finish=uninitialized_fill_n(new_finish,n,x);
                //以下再将旧vector的插入点之后的元素复制到新空间
                new_finish=uninitialized_copy(position,finish,new_finish);
            }
            #ifdef __STL_USE_EXCEPTIONS
            catch(...){
                //如有异常发生，实现“commit or rollback” semantics
                destroy(new_start,new_finish);
                data_allocator::deallocate(new_start,len);
                throw;
            }
            #endif /* __STL_USE_EXCEPTIONS */
            // 以下清除并释放旧的vector
            destroy(start,finish);
            deallocate();
            //以下调整水位标记
            start=new_start;
            finish=new_finish;
            end_of_storage=new_start+len;
        }
    }
}