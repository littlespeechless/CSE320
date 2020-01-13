/**
 * All functions you make for the assignment must be implemented in this file.
 * Do not submit your assignment with a main function in this file.
 * If you submit with a main function in this file, you will get a zero.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sfmm.h>
#include <errno.h>
#include "debug.h"
#include "sfmm.h"
int free_list_head_search(size_t size);
void initialize_free_list();
sf_block* free_list_fit_search(sf_block* head, size_t size);
void free_list_insert(sf_block* head, sf_block* item);
sf_block* split_free_block(sf_block* freeBlock, size_t split_block_size,size_t allocate_size);
sf_block* merge_free_block(sf_block* currentFreeBlock);
void free_list_block_search (sf_block* head, sf_block* item);
sf_block* grow_free_block(sf_block* freeBlock, int validGrow);
sf_block* allocate_free_block(sf_block* freeBlock, size_t size);
int valid_block (sf_block* freeBlock);

sf_prologue* prologue;
sf_epilogue* epilogue;
sf_block* lastBlock;

void *sf_malloc(size_t size) {
    //case of 0 size
    if (size == 0) {
        return NULL;
    }
    //case of uninitialized

    if (sf_mem_start() == sf_mem_end()) {
        initialize_free_list();
        //initialize
        sf_mem_grow();
        // start structure layout
        prologue = (sf_prologue *) sf_mem_start();
        prologue->padding1 = 0;
        prologue->header = 35;
        prologue->unused1 = NULL;
        prologue->unused2 = NULL;
        prologue->footer = prologue->header ^ sf_magic();
        sf_block *freeBlock = (sf_block *) (sf_mem_start() + 32);
        freeBlock->prev_footer = prologue->footer;
        //check  size
        if (size % 16 != 0) {
            size += (16 -(size % 16));
        }
        // page size - prologue - ep - block hear  x 2
        int totalPages = 0;
        size_t currentPageSZ = PAGE_SZ - 40 - 8 - 8 - 8;
        if (currentPageSZ < size) {
            //check how many we need
            size_t sizeNeeded = size - currentPageSZ;
            int pages;
            if (sizeNeeded % PAGE_SZ == 0){
                pages = sizeNeeded / PAGE_SZ;
            } else{
                pages = (int)(sizeNeeded / PAGE_SZ) +1;
            }
            int valid_growth = 0;
            //increment heap size
            for (int i = 0; i < pages; ++i) {
                if (sf_mem_grow() == NULL) {
                    freeBlock->header = currentPageSZ + 8 + 8 + (PAGE_SZ*valid_growth) + 1;
                    //epilogue = (void*) sf_mem_end() - 8;
                    //epilogue->header = 2;
                    lastBlock = (sf_block *)((void*) freeBlock + 16 + currentPageSZ + (PAGE_SZ*valid_growth));
                    lastBlock->prev_footer = freeBlock->header ^ sf_magic();
                    epilogue = (sf_epilogue*)((void *) lastBlock + 8);
                    epilogue->header = 2;
                    lastBlock->header = epilogue->header;
                    free_list_insert(&sf_free_list_heads[free_list_head_search(freeBlock->header & BLOCK_SIZE_MASK)]
                            ,freeBlock);
                    sf_errno = ENOMEM;
                    return NULL;
                }
                valid_growth++;
            }
            totalPages = pages;
        }
        // block header

        //check remaining size
        size_t remainSize = totalPages * PAGE_SZ + currentPageSZ - size;
        if (remainSize < 32) {
            size += remainSize;
            freeBlock->header = (size + 8 + 8) + 3;
            //no split
            lastBlock = (sf_block *) (((void *) freeBlock) + size + 16);
            lastBlock->prev_footer = freeBlock->header ^ sf_magic();
            //epilogue
            epilogue = (sf_epilogue *) ((void *) lastBlock + 8);
            epilogue->header = 2;
            lastBlock->header = epilogue->header;
            //sf_show_heap();
            return ((void *) freeBlock) + 16;
        }
        // case need split
        freeBlock->header = (size + 8 + 8) + 3;
        //split
        sf_block *split_block = (sf_block *) (((void *) freeBlock) + size + 16);
        split_block->prev_footer = freeBlock->header ^ sf_magic();
        split_block->header = remainSize + 1;
        // split footer
        lastBlock = (sf_block *) (((void *) split_block) + remainSize);
        lastBlock->prev_footer = split_block->header ^ sf_magic();

        //epilogue
        epilogue = (sf_epilogue *) ((void *) lastBlock + 8);
        epilogue->header = 2;
        lastBlock->header = epilogue->header;

        //put in free list
        int head_index = free_list_head_search(remainSize);
        // insert
        free_list_insert(&sf_free_list_heads[head_index], split_block);

        //sf_show_heap();
        return ((void *) freeBlock) + 16;
    }
    //case of already exist
    // check size
    if (size % 16 != 0) {
        size += (16 -(size % 16));
    }
    //search in free list
    sf_block *freeBlock = NULL;
    for (int j = free_list_head_search(size + 8 + 8); j < NUM_FREE_LISTS; ++j) {
        freeBlock = free_list_fit_search(&sf_free_list_heads[j], size + 8 + 8);
        if (freeBlock != NULL) {
            break;
        }
    }
    if (freeBlock == NULL) {
        //case no match
        //check last block
        size_t free_block_size = 0;
        if ((lastBlock->header & PREV_BLOCK_ALLOCATED)== 0) {
            free_block_size = (lastBlock->prev_footer ^ sf_magic()) & BLOCK_SIZE_MASK;
            //freeBlock = (sf_block*) ((void *) lastBlock - 16 - free_block_size);

        }
        // check how many size we still needs
        size_t sizeNeeded = size + 16 - free_block_size;
        // check how many page wee need to grow
        int pages;
        if (sizeNeeded % PAGE_SZ == 0){
            pages = sizeNeeded / PAGE_SZ;
        } else{
            pages = (int)(sizeNeeded / PAGE_SZ) +1;
        }
        int validGrow = 0;
        for (int i = 0; i < pages; ++i) {
            if (sf_mem_grow() == NULL){
                //case oversize
                freeBlock = grow_free_block(freeBlock, validGrow);
                //put in free list
                free_list_insert(
                        &sf_free_list_heads[free_list_head_search(freeBlock->header & BLOCK_SIZE_MASK)]
                        ,freeBlock);
                sf_errno = ENOMEM;
                return NULL;
            }
            validGrow ++;
        }
        //create new free block from last block
        freeBlock = grow_free_block(freeBlock, validGrow);
        // allocate it
        freeBlock = allocate_free_block(freeBlock,size);
        return ((void *) freeBlock) + 16;
    } else {
        //case find free block
        freeBlock = allocate_free_block(freeBlock,size);
        return ((void *) freeBlock) + 16;
    }
    return NULL;


}
sf_block* allocate_free_block(sf_block* freeBlock, size_t size){
    size_t block_size = freeBlock->header & BLOCK_SIZE_MASK;
    size_t remaining_size = block_size - size - 8 - 8;
    //check split or not
    if (remaining_size < 32) {
        size += remaining_size;
        freeBlock->header += 2;
        sf_block *footer_block = (sf_block *) ((void *) freeBlock + 16 + size);
        if (footer_block == lastBlock) {
            //case end of heap
            lastBlock->prev_footer = freeBlock->header ^ sf_magic();
            lastBlock->header = 3;
        } else {
            // not end
            footer_block->prev_footer = freeBlock->header ^ sf_magic();
        }
        return freeBlock;
    } else {
        //case of split
        freeBlock->header = size + 8 + 8 + (freeBlock->header & PREV_BLOCK_ALLOCATED) + 2;
        sf_block *split_block = split_free_block(freeBlock, remaining_size, size);
        split_block = merge_free_block(split_block);
        int free_list_head_index = free_list_head_search(remaining_size);
        free_list_insert(&sf_free_list_heads[free_list_head_index], split_block);
        return freeBlock;
    }

}
sf_block* grow_free_block(sf_block* freeBlock, int validGrow){
    //create new free block from last block
    freeBlock = lastBlock;
    freeBlock->header = PAGE_SZ * validGrow + (freeBlock->header&PREV_BLOCK_ALLOCATED);
    // update last block position
    lastBlock = (sf_block *)((void *) sf_mem_end() - 16);
    //update epilogue position
    epilogue = (sf_epilogue *)((void *) lastBlock + 8);
    // update epilogue header
    epilogue->header = 2;
    lastBlock->header = epilogue->header;
    //create footer for new free block
    lastBlock->prev_footer = freeBlock->header ^ sf_magic();
    //return merged block if any
    return merge_free_block(freeBlock);
}
sf_block* merge_free_block(sf_block* currentFreeBlock){
    //get prev block and next block
    size_t prevAllocate = currentFreeBlock->header & PREV_BLOCK_ALLOCATED;
    size_t block_size = currentFreeBlock->header & BLOCK_SIZE_MASK;
    sf_block* nextBlock = (sf_block *) ((void*) currentFreeBlock + block_size);
    size_t nextAllocate = nextBlock->header & THIS_BLOCK_ALLOCATED;
    size_t prevBlockSize = (currentFreeBlock->prev_footer ^ sf_magic()) & BLOCK_SIZE_MASK;
    size_t nextBlockSize = nextBlock->header & BLOCK_SIZE_MASK;
    sf_block* prevBlock = (sf_block *)((void *) currentFreeBlock - prevBlockSize);
    if (prevAllocate==0 && nextAllocate == 0){
        // remove blocks from free list
        free_list_block_search(&sf_free_list_heads[free_list_head_search(prevBlockSize)],
                prevBlock);
        free_list_block_search(&sf_free_list_heads[free_list_head_search(block_size)],
                currentFreeBlock);
        free_list_block_search(&sf_free_list_heads[free_list_head_search(nextBlockSize)],
                nextBlock);
        //merge blocks
        prevBlock->header += (block_size + nextBlockSize);
        sf_block* footer_block = (sf_block *)((void *) nextBlock + nextBlockSize);
        footer_block->prev_footer = prevBlock->header ^ sf_magic();
        //return merged block
        return prevBlock;
    } else if (prevAllocate==0){
        //remove block from free list
        free_list_block_search(&sf_free_list_heads[free_list_head_search(prevBlockSize)],
                               prevBlock);
        free_list_block_search(&sf_free_list_heads[free_list_head_search(block_size)],
                               currentFreeBlock);
        //merge blocks
        prevBlock->header += block_size;
        sf_block* footer_block = (sf_block *)((void *) currentFreeBlock + block_size);
        footer_block->prev_footer = prevBlock->header ^ sf_magic();
        //return merged block
        return prevBlock;
    } else if (nextAllocate==0){
        //remove block from free list
        free_list_block_search(&sf_free_list_heads[free_list_head_search(block_size)],
                               currentFreeBlock);
        free_list_block_search(&sf_free_list_heads[free_list_head_search(nextBlockSize)],
                               nextBlock);
        //merge blocks
        currentFreeBlock->header += nextBlockSize;
        sf_block* footer_block = (sf_block *)((void *) nextBlock + nextBlockSize);
        footer_block->prev_footer = currentFreeBlock->header ^ sf_magic();
        //return merged block
        return currentFreeBlock;
    } else{
//        currentFreeBlock->header = block_size + 1;
//        sf_block* footer_block = (sf_block *)((void *) currentFreeBlock + block_size);
//        footer_block->prev_footer = currentFreeBlock->header ^ sf_magic();
        return currentFreeBlock;
    }
}
sf_block* split_free_block(sf_block* freeBlock, size_t split_block_size, size_t allocate_size){
    //split block
    sf_block* split_block =(sf_block *) (((void *)freeBlock) + allocate_size + 16);
    split_block->prev_footer = freeBlock->header ^ sf_magic();
    split_block-> header = split_block_size + 1;
    // split footer
    sf_block* split_footer_block = (sf_block*)(((void*)split_block) + split_block_size);
    if (split_footer_block == lastBlock){
        lastBlock->prev_footer = split_block->header ^ sf_magic();
        lastBlock->header = 2;
    } else{
        split_footer_block->prev_footer = split_block->header ^ sf_magic();
        split_footer_block->header -= split_footer_block->header&PREV_BLOCK_ALLOCATED;
        sf_block* nextBlock = (sf_block*) ((void*) split_footer_block +
                (split_footer_block->header&BLOCK_SIZE_MASK));
        nextBlock->prev_footer = split_footer_block->header^sf_magic();
    }
    return split_block;
}
void initialize_free_list(){
    for (int i = 0; i <NUM_FREE_LISTS ; i++) {
        sf_free_list_heads[i].header = 0;
        sf_free_list_heads[i].body.links.next = &sf_free_list_heads[i];
        sf_free_list_heads[i].body.links.prev = &sf_free_list_heads[i];
    }
}
sf_block* free_list_fit_search (sf_block * head,size_t size ){
    if (head->body.links.next == head && head->body.links.prev == head){
        return  NULL;
    }
    sf_block* cursor = head;
    while (cursor->body.links.next!=head){
        cursor = cursor->body.links.next;
        if ((cursor->header&BLOCK_SIZE_MASK)>=size){
            cursor->body.links.prev->body.links.next = cursor->body.links.next;
            cursor->body.links.next->body.links.prev = cursor->body.links.prev;
            return cursor;
        }
    }
    return NULL;
}
void free_list_block_search (sf_block* head, sf_block* item){
    sf_block* cursor = head;
    while (cursor->body.links.next!=head){
        cursor = cursor->body.links.next;
        if (cursor == item){
            cursor->body.links.prev->body.links.next = cursor->body.links.next;
            cursor->body.links.next->body.links.prev = cursor->body.links.prev;
            break;
        }
    }
}
void free_list_insert(sf_block* head, sf_block* item){
    //case empty list
    if (head->body.links.prev == head
        && head->body.links.next == head){
        //case of empty list
        head->body.links.next = item;
        item->body.links.prev = head;
        head->body.links.prev = item;
        item->body.links.next = head;
    } else{
        head->body.links.next->body.links.prev = item;
        item->body.links.next = head->body.links.next;
        head->body.links.next = item;
        item->body.links.prev = head;

    }
}
int free_list_head_search(size_t size){
    if (size == 32){
        return 0 ;
    } else if (size>32 && size<=64){
        return 1;
    } else if (size>64 && size<=128){
        return 2;
    } else if (size>128 && size<=256){
        return 3;
    } else if (size>256 && size<=512){
        return 4;
    } else if (size>512 && size<=1024){
        return 5;
    } else if (size>1024 && size<=2048){
        return 6;
    } else if (size>2048 && size<=4096){
        return 7;
    } else if (size>4096){
        return 8;
    } else{
        return -1;
    }
}
void sf_free(void *block) {
    //case null
    if (block == NULL){
        abort();
    }
    sf_block* freeBlock = (sf_block *)(block - 16);
    if (valid_block(freeBlock)!=1){
        abort();
    }
    size_t freeBlockSize = freeBlock->header & BLOCK_SIZE_MASK;
    sf_block* footer_block = (sf_block*)((void *) freeBlock + freeBlockSize);
    //free the block
    freeBlock->header -= 2;
    footer_block->prev_footer = freeBlock->header ^ sf_magic();
    if (footer_block!=lastBlock){
        footer_block->header -=footer_block->header&PREV_BLOCK_ALLOCATED;
        sf_block* next_block = (sf_block*)((void*) footer_block + (footer_block->header & BLOCK_SIZE_MASK));
        next_block->prev_footer = footer_block->header ^ sf_magic();
    } else{
        footer_block->header -=footer_block->header&PREV_BLOCK_ALLOCATED;
    }
    freeBlock = merge_free_block(freeBlock);
    int head_index = free_list_head_search(freeBlock->header & BLOCK_SIZE_MASK);
    free_list_insert(&sf_free_list_heads[head_index],freeBlock);

    return;
}

void *sf_realloc(void *block, size_t rsize) {
    if (block == NULL){
        sf_errno = EINVAL;
        return NULL;
    }
    sf_block* freeBlock = (sf_block *) (block - 16);
    if (valid_block(freeBlock)!=1){
        sf_errno = EINVAL;
        return NULL;
    }
    if (rsize == 0){
        sf_free(block);
        return NULL;
    }
    size_t freeBlockSize = freeBlock->header & BLOCK_SIZE_MASK;
    //sf_block* footer_block = (sf_block*)((void *) freeBlock + freeBlockSize);
    //check rsize
    if (rsize % 16 != 0) {
        rsize += (16 -(rsize % 16));
    }
    // big or small re-allocate
    if (rsize+16 <= freeBlockSize){
        //check remaining size
        size_t remainSize = freeBlockSize - rsize - 8 - 8;
        if (remainSize<32){
            rsize+= remainSize;
            return block;
        }
        //case split
        freeBlock = allocate_free_block(freeBlock,rsize);
        return ((void *) freeBlock) + 16;
    } else {
        void* newBlock = sf_malloc(rsize);
        //case NULL
        if (newBlock == NULL){
            return  NULL;
        }
        memcpy(newBlock,block,freeBlockSize-16);
        sf_free(block);
        return  newBlock;
    }
}
int valid_block (sf_block* freeBlock){
    //case header is 0
    if ((freeBlock->header&THIS_BLOCK_ALLOCATED) == 0){
        return -1;
    }
    size_t freeBlockSize = freeBlock->header & BLOCK_SIZE_MASK;
    // small size
    if (freeBlockSize<32){
        return -1;
    }
    //case of out of boundary
    if ((((void *) freeBlock + 8 )<((void *) prologue + 40))||
        (((void *) freeBlock + freeBlockSize)>(void*) epilogue-8)){
        return -1;
    }
    // invalid header
    size_t  prevAllocate = (freeBlock->prev_footer ^ sf_magic()) & THIS_BLOCK_ALLOCATED;
    size_t  allocate = freeBlock->header & PREV_BLOCK_ALLOCATED;
    if ((prevAllocate == 2 && allocate!= 1)||(prevAllocate == 0 && allocate !=0)){
        return -1;
    }
    //case size not aligned
    if (freeBlockSize%16 != 0){
        return -1;
    }
    //case miss match footer
    sf_block* footer_block = (sf_block*)((void *) freeBlock + freeBlockSize);
    if (freeBlock->header != (footer_block->prev_footer ^ sf_magic())){
        return -1;
    }
    return 1;
}

