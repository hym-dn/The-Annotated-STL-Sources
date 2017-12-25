//返回一个大小为n的对象，并且有时候会为适当的free list增加节点
//假设n已经适当上调至8的倍数
template<bool threads,int inst>
void *__default_alloc_template<threads,inst>::refill(size_t n){
    int nobjs=20;
    //调用chunk_alloc()，尝试取得nobjs个区块作为free list的新节点
    //注意参数nobjs是pass by reference
    char *chunk=chunk_alloc(n,nobjs);//下节详述
    obj *volatile *my_free_list;
    obj *result;
    obj *current_obj,*next_obj;
    int i;
    //如果只获得一个区块，这个区块就分配给调用者用，free list无新节点
    if(1==nobjs){
        return(chunk);
    }
    //否则准备调整free list,纳入新节点
    my_free_list=free_list+FREELIST_INDEX(n);
    //以下在chunk空间内建立free list
    result=(obj*)chunk;//这一块准备返回给客户端
    //以下导引free list的各节点转接起来。
    *my_free_list=next_obj=(obj *)(chunk+n);
    //以下将free list的各节点串接起来
    for(i=1;;i++){//从1开始，因为第0个将返回给客户端。
        current_obj=next_obj;
        next_obj=(obj*)((char*)next_obj+n);
        if(nobjs-1==i){
            current_obj->free_list_link=0;
            break;
        }else{
            current_obj->free_list_link=next_obj;
        }
    }
    return(result);
}
