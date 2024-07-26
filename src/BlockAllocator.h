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

    size_t getFreeCount() const;
    size_t getUsedCount() const;
    size_t getBlockSize() const;
    size_t getPoolSize() const;

    void reset();
    bool checkIntegrity() const;

private:
    size_t blockSize;
    size_t poolSize;
    size_t freeCount;
    uint8_t* pool;
    void** freeBlocks;
};

#endif // BLOCKALLOCATOR_H
