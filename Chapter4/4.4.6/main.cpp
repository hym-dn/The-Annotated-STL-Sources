void pop_back(){
    if(finish.cur!=finish.first){
        // 最后缓冲区有一个（或更多）元素
        --finish.cur; // 调整指针，相当于排除了最后元素
        destroy(finish.cur); // 将最后元素析构
    }else{
        // 最后缓冲区没有任何元素
        pop_back_aux(); // 这里将进行缓冲区的释放工作
    }
}

// 只有当finish.cur==finish.first时才会被调用
template<class T,class Alloc,size_t BufSize>
void deque<T,Alloc,BufSize>::pop_back_aux(){
    deallocate_node(finish.first); // 释放最后一个缓冲区
    finish.set_node(finish.node-1); // 调整finish的状态，是指向
    finish.cur=finish.last-1; // 上一个缓冲区的最后一个元素
    destroy(finish.cur); // 将该元素析构
}

void pop_front(){
    if(start.cur!=start.last-1){
        // 第一缓冲区有两个（或更多）元素
        destroy(start.cur); // 将第一个元素析构
        ++start.cur; // 调整指针，相当于排除了第一元素
    }else{
        // 第一缓冲区仅有一个元素
        pop_front_aux(); // 这里将进行缓冲区的释放工作
    }
}

// 只有当start.cur==start.last-1时才会被调用
template<class T,class Alloc,size_t BufSize>
void deque<T,Alloc,BufSize>::pop_front_aux(){
    destroy(start.cur); // 将第一缓冲区的第一个（也是最后一个、
                        // 唯一一个元素析构）
    deallocate_node(start.first); // 释放第一缓冲区
    start.set_node(start.node+1); // 调整start的状态，使指向
    start.cur=start.first;        // 下一个缓冲区的第一个元素
}

// 注意，最终需要保留一个缓冲区。这是deque的策略，也是deque的初始状态
template<class T,class Alloc,size_t BufSize>
void deque<T,Alloc,BufSize>::clear(){
    // 以下针对头尾以外的每一个缓冲区（它们一定都是饱满的）
    for(map_pointer node=start.node+1;node<finish.node;++node){
        // 将缓冲区内的所有元素析构。注意，调用的是destroy()第二版本，
        // 见2.2.3节
        destroy(*node,*node+buffer_size());
        // 释放缓冲区内存
        data_allocator::deallocate(*node,buffer_size());
    }
    // 至少有头尾两个缓冲区
    if(start.node!=finish.node){
        // 将头缓冲区的目前所有元素析构
        destroy(start.cur,start.last);
        // 将尾缓冲区的目前所有元素析构
        destroy(finish.first,finish.cur);
        // 以下释放尾缓冲区。注意，头缓冲区保留
        data_allocator::deallocate(finish.first,buffer_size());
    }
    // 只是一个缓冲区
    else{
        destroy(start.cur,finish.cur); // 将此唯一缓冲区内的所有元素析构
        // 注意，并不释放缓冲区空间。这唯一的缓冲区保留。
    }
    finish=start; // 调整状态
}

// 清除pos所指的元素。pos为清除点
iterator erase(iterator pos){
    iterator next=pos;
    ++next;
    difference_type index=pos-start; // 清除点之前的元素个数
    if(index<(size()>>1)){ // 如果清除点之前的元素比较少,
        copy_backward(start,pos,next); // 就移动清除点之前的元素
        pop_front(); // 移动完毕，最前一个元素冗余，去除之。
    }else{ // 清除点之后的元素比较少
        copy(next,finish,pos); // 就移动清除点之后的元素
        pop_back(); // 移动完毕，最后一个元素冗余，去除之
    }
    return(start+index);
}

template<class T,class Alloc,size_t BufSize>
deque<T,Alloc,BufSize>::iterator 
    deque<T,Alloc,BufSize>::erase(iterator first,iterator last){
    if(first==start&&last==finish){ // 如果清除区间就是整个deque
        clear(); // 直接调用clear()即可
        return finish;
    }else{
        difference_type n=last-first; // 清除区间的长度
        difference_type elems_before=first-start; // 清除区间前方的元素个数
        if(elems_before<(size()-n)/2){ // 如果前方的元素比较少
            copy_backward(start,first,last); // 向后移动前方元素（覆盖清除区间）
            iterator new_start=start+n; // 标记deque的新起点
            destroy(start,new_start); // 移动完毕，将冗余的元素析构
            // 以下将冗余的缓冲区释放
            for(map_pointer cur=start.node;cur<new_start.node;++cur)
                data_allocator::deallocate(*cur,buffer_size());
            // 设定 deque 的新起点
            start=new_start; // 设定 deque 的新起点
        }else{ // 如果清除区间后方的元素比较少
            copy(last,finish,first); // 向前移动后方元素（覆盖清除区间）
            iterator new_finish=finish-n; // 标记deque的新尾点
            destroy(new_finish,finish); // 移动完毕，将冗余的元素析构
            // 以下将冗余的缓冲区释放
            for(map_pointer cur=new_finish.node+1;cur<=finish.node;++cur)
                data_allocator::deallocate(*cur,buffer_size());
            // 设定 deque 的新尾点
            finish=new_finish;
        }
        return start+elems_before;
    }
}

// 在 position 处插入一个元素，其值为x
iterator insert(iterator position,const value_type &x){
    // 如果插入点是 deque 最前端
    if(position.cur==start.cur){ // 如果插入点是 deque 最前端
        push_front(x); // 交给 push_front 去做
        return start;
    }else if(position.cur==finish.cur){ // 如果插入点是 deque 最尾端
        push_back(x); // 交给 push_back 去做
        iterator temp=finish;
        --tmp;
        return tmp;
    }else{
        return insert_aux(position,x); // 交给 insert_aux 去做
    }
}

template<class T,class Alloc,size_t BufSize>
typename deque<T,Alloc,BufSize>::iterator 
deque<T,Alloc,BufSize>::insert_aux(iterator pos,const value_type &x){
    diference_type index=pos-start; // 插入点之前的元素个数
    value_type x_copy=x;
    if(index<size()/2){ // 如果插入点之前的元素个数比较多s
        push_back(front()); // 在最前端加入与第一元素同值的元素
        iterator front1=start; // 以下标记记号，然后进行元素移动
        ++front1;
        iterator front2=front1;
        ++front2;
        pos=start+index;
        iterator pos1=pos;
        ++pos1;
        copy(front2,pos1,front1); // 元素移动
    }else{ // 插入点之后的元素个数比较少
        push_back(back()); // 在最尾端加入与最后元素同值的元素
        iterator back1=finish; // 以下标记记号，然后进行元素移动
        --back1;
        iterator back2=back1;
        --back2;
        pos=start+index;
        copy_backward(pos,back2,back1); // 元素移动
    }
    *pos=x_copy; // 在插入点上设定新值
    return pos;
}