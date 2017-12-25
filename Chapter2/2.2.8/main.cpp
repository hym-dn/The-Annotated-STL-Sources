//p不可以是0
static void dealocate(void *p,size_t n){
    obj *q=(obj*)p;
    obj *volatile *my_free_list;
    //大于128就用第一级配置器
    if(n>(size_t)__MAX_BYTES){
        malloc_alloc::deallocate(p,n);
    }
    //寻找对应的free list
    my_free_list=free_list+FREELIST_INDEX(n);
    //调整free list
    q->free_list_link=*my_free_list;
    *my_free_list=q;
}
