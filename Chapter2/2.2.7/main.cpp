//n must be >0
static void *allocate(size_t n){
    obj *volatile *my_free_list;
    obj *result;
    //大于128就调用第一级配置器
    if(n>(size_t)__MAX_BYTES){
        return(malloc_alloc::allocate(n));
    }
    //选找16个freelists中适当的一个
    my_free_list=free_list+FREELIST_INDEX(n);
    result=*my_free_list;
    if(result==0){
        //没找到可用的free list，准备重新填充free list
        void *r=refill(ROUND_UP(n));//下节详述
        return(r);
    }
    //调整free list
    *my_free_list=result->free_list_link;
    return(result);
}
