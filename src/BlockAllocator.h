#ifndef BLOCKALLOCATOR_H
#define BLOCKALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

class BlockAllocator {
public:
    BlockAllocator(size_t blockSize, size_t poolSize);
    ~BlockAllocator();
    
    void* allocate();
    void deallocate(void* ptr);

    size_t getBlockSize() const { return blockSize; }
    size_t getPoolSize() const { return poolSize; }
    size_t getFreeCount() const { return freeCount; }
    size_t getUsedCount() const { return (poolSize / blockSize) - freeCount; }

private:
    size_t blockSize;
    size_t poolSize;
    uint8_t* pool;
    uint8_t** freeList;
    size_t freeCount;
};

#endif // BLOCKALLOCATOR_H
