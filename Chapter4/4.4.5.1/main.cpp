protected: // Internal typedef

    // 专属之空间配置器，每次配置一个元素大小
    typedef simple_alloc<value_type,Alloc> data_allocator;
    // 专属之空间配置其，每次配置一个指针大小
    typedef simple_alloc<pointer,Alloc> map_allocator;

    // 构造函数
    deque(int n,const value_type &value)
        :start(),finish(),map(0),map_size(0){
        fill_initialize(n,value);
    }

    // 初始化填充
    template<class T,class Alloc,size_t BufSize>
    void deque<T,Alloc,BufSize>::fill_initialize(
        size_type n,const value_type &value){
        // 把deque的结构都产生并安排好
        create_map_and_nodes(n);
        // 填充数据
        map_pointer cur;
        __STL_TRY{
            // 为每个节点的缓冲区设定初值
            for(cur=start.node;cur<finish.node;++cur) //遍历控制中心
                uninitialized_fill(*cur,*cur+buffer_size(),value); //初始化控制中心所指向内容
            // 最后一个节点的设定稍有不同（因为尾端可能有备用空间，不必设初值）
            uninitialized_fill(finish.first,finish.cur,value);
        }catch(...){
            // ...
        }  
    }

    // 负责产生并安排好deque的结构
    template<class T,class Alloc,size_t BufSize>
    void deque<T,Alloc,BufSize>::create_map_and_nodes(
        size_t num_elements){
        // 需要节点数=（元素个数/每个缓冲区可容纳的元素个数）+1
        // 如果刚好整除，会分配一个节点
        size_type num_nodes=num_elements/buffer_size()+1;

        // 一个map要管理几个节点。最少8个，最多是“所需节点数加2”
        //（前后个预留一个，扩充时可用）
        map_size=max(initial_map_size(),num_nodes+2);
        // 配置出一个“具有map_size个节点”的map
        map=map_allocator::allocate(map_size);

        // 以下令nstart和nfinish指向map所拥有之全部节点的最中央区段
        // 保持在最中央，可是头尾两端的扩充能量一样大。每个节点可对应一个缓冲区。
        map_pointer nstart=map+(map_size-num_nodes)/2; // 使得有效节点保持在中央
        map_pointer nfinish=nstart+num_nodes-1;

        map_pointer cur;
        __STL_TRY{
            // 为map内的每个现用节点配置缓冲区。所有缓冲区加起来就是deque的
            // 可用空间（最后一个缓冲区可能留有一些余裕）
            for(cur=nstart;cur<=nfinish;++cur)
                *cur=allocate_node();
        }catch(...){
            // "commit or rollback"语意：若非全部成功，就一个不留
            // ...
        }

        // 为deque内的两个迭代器start和end设定正确内容
        start.set_node(nstart);
        finish.set_node(nfinish);
        start.cur=start.first; // first,cur都是public
        finish.cur=finish.first+num_elements%buffer_size();
        // 前面说过，如果刚好整除，会多配置一个节点
        // 此时即令cur指向这多配的一个节点（所对应之缓冲区）的起始处
    }

public: // push_* and pop_*

    void push_back(const value_type &t){
        if(finish.cur!=finish.last-1){
            // 最后缓冲区尚有两个（含）以上的元素备用空间
            construct(finish.cur,t); // 直接在备用空间上构造元素
            // 调整最后缓冲区的使用状态
            ++finish.cur;
        }else{ // 最后缓冲区只剩一个元素备用空间
            push_back_aux(t);
        }
    }

    // 只有当 finish.cur==finish.last-1时才会被调用
    // 也就是说，只有当最后一个缓冲区只剩一个备用元素空间时才会被调用
    template<class T,class Alloc,size_t BufSize>
    void deque<T,Alloc,BufSize>::push_back_aux(const value_type &t){
        value_type t_copy=t;
        reserve_map_at_back(); // 若符合某种条件则必须重换一个map
        *(finish.node+1)=allocate_node(); // 配置一个新节点（缓冲区）
        __STL_TRY{
            construct(finish.cur,t_copy); // 针对标的元素设值
            finish.set_node(finish.node+1); // 改变finish，令其指向新节点
            finish.cur=finish.first; // 设定finish的状态
        }
        __STL_UNWIND(deallocate_node(*(finish.node+1)));
    }

public:

    void push_front(const value_type &t){
        if(start.cur!=start.first){ // 第一缓冲区尚有备用空间
            construct(start.cur-1,t); // 直接在备用空间上构造元素
            --start.cur; // 调整第一缓冲区的使用状态
        }
        // 第一缓冲区已无备用空间
        else{
            push_front_aux(t);
        }
    }

    // 只有当start.cur==start.first时才会被调用
    // 也就是说，只有当第一个缓冲区没有任何备用元素时才会被调用
    template<class T,class Alloc,size_t BufSize>
    void deque<T,Alloc,BufSize>::push_front_aux(const value_type &t){
        value_type t_copy=t;
        reserve_map_at_front(); // 若符合某种条件则必须重换一个map
        *(start.node-1)=allocate_node(); // 配置一个新节点（缓冲区）
        __STL_TRY{
            start.set_node(start.node-1); // 改变start，令其指向新节点
            start.cur=start.last-1; // 设定start的状态
            construct(start.cur,t_copy); // 针对标的元素设置
        }catch(...){
            // "commit or rollback"语意：若非全部成功，就一个不留。
            start.set_node(start.node+1);
            start.cur=start.first;
            deallocate_node(*(start.node-1));
            throw;
        }
    }

    void reserve_map_at_back(size_type nodes_to_add=1){
        if(nodes_to_add+1>map_size-(finish.node-map))
            // 如果 map 尾端节点备用空间不足
            // 符合以上条件则必须重换一个map(配置更大的，拷贝原来的，释放原来的)
            reallocate_map(nodes_to_add,false);
    }

    void reserve_map_at_front(size_type nodes_to_add=1){
        if(nodes_to_add>start.node-map)
            // 如果map前端的节点备用空间不足
            // 符合以上条件则必须重换一个map(配置更大的，拷贝原来的，释放原来的)
            reallocate_map(nodes_to_add,true);
    }
    
    template<class T,class Alloc,size_t BufSize>
    void deque<T,Alloc,BufSize>::reallocate_map(
        size_type nodes_to_add,bool add_at_front){

        size_type old_num_nodes=finish.node-start.node+1;
        size_type new_num_nodes=old_num_nodes+nodes_to_add;

        map_pointer new_nstart;
        // 如果当前map尺寸大于2倍的节点数量
        if(map_size>2*new_num_nodes){
            new_nstart=map+(map_size-new_num_nodes)/2+(
                add_at_front?nodes_to_add:0);
            if(new_nstart<start.node)
                copy(start.node,finish.node+1,new_nstart);
            else
                copy_backward(start.node,finish.node+1,new_nstart+old_num_nodes);
        }
        // 如果当前map尺寸不大于2倍的节点数量
        else{
            size_type new_map_size=map_size+max(map_size,nodes_to_add)+2;
            // 配置一块空间，准备给新map使用
            map_pointer new_map=map_allocator::allocate(new_map_nodes);
            new_nstart=new_map+(new_map_size-new_num_nodes)/2+
                (add_at_front?nodes_to_add:0);
            // 把原map内容拷贝过来
            copy(start.node,finish.node+1,new_nstart);
            // 释放原map
            map_allocator::deallocate(map,map_size);
            // 设定新map的起始地址与大小
            map=new_map;
            map_size=new_map_size;
        }
        // 重新设定迭代器start和finish
        start.set_node(new_nstart);
        finish.set_node(new_nstart+old_num_nodes-1);

    }