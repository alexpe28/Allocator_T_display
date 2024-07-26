#include "BlockAllocator.h"
#include <cstring>

BlockAllocator::BlockAllocator(size_t blockSize, size_t poolSize)
    : blockSize(blockSize), poolSize(poolSize), freeCount(poolSize / blockSize) {
    pool = new uint8_t[poolSize];
    freeBlocks = new void*[freeCount];
    for (size_t i = 0; i < freeCount; ++i) {
        freeBlocks[i] = pool + i * blockSize;
    }
}

BlockAllocator::~BlockAllocator() {
    delete[] pool;
    delete[] freeBlocks;
}

void* BlockAllocator::allocate() {
    if (freeCount == 0) return nullptr;
    return freeBlocks[--freeCount];
}

void BlockAllocator::deallocate(void* ptr) {
    if (ptr >= pool && ptr < pool + poolSize) {
        freeBlocks[freeCount++] = ptr;
    }
}

size_t BlockAllocator::getFreeCount() const {
    return freeCount;
}

size_t BlockAllocator::getUsedCount() const {
    return (poolSize / blockSize) - freeCount;
}

size_t BlockAllocator::getBlockSize() const {
    return blockSize;
}

size_t BlockAllocator::getPoolSize() const {
    return poolSize;
}

void BlockAllocator::reset() {
    freeCount = poolSize / blockSize;
    for (size_t i = 0; i < freeCount; ++i) {
        freeBlocks[i] = pool + i * blockSize;
    }
}

bool BlockAllocator::checkIntegrity() const {
    for (size_t i = 0; i < freeCount; ++i) {
        if (freeBlocks[i] < pool || freeBlocks[i] >= pool + poolSize) {
            return false;
        }
    }
    return true;
}
