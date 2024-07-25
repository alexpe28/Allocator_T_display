#include "BlockAllocator.h"
#include <stdlib.h>
#include <string.h>

BlockAllocator::BlockAllocator(size_t blockSize, size_t poolSize)
    : blockSize(blockSize), poolSize(poolSize), freeCount(poolSize / blockSize) {
    pool = (uint8_t*)malloc(poolSize);
    freeList = (uint8_t**)malloc(freeCount * sizeof(uint8_t*));
    
    for (size_t i = 0; i < freeCount; ++i) {
        freeList[i] = pool + i * blockSize;
    }
}

BlockAllocator::~BlockAllocator() {
    free(pool);
    free(freeList);
}

void* BlockAllocator::allocate() {
    if (freeCount == 0) return nullptr;
    return freeList[--freeCount];
}

void BlockAllocator::deallocate(void* ptr) {
    if (freeCount < poolSize / blockSize) {
        freeList[freeCount++] = (uint8_t*)ptr;
    }
}
