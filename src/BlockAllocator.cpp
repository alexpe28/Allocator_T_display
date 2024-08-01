#include "BlockAllocator.h"
#include <algorithm>
#include <cstring>

BlockAllocator::BlockAllocator(size_t blockSize , size_t poolSize)
    : blockSize(alignSize(blockSize)) , poolSize(poolSize) , freeCount(poolSize / this->blockSize)
{
    pool = reinterpret_cast<uint8_t*>(aligned_alloc(this->blockSize , poolSize));
    freeBlocks = new void* [freeCount];

#if defined(ESP32) || defined(ESP8266)
    mutex = xSemaphoreCreateMutex();
#endif

    initFreeBlocks();
}

BlockAllocator::~BlockAllocator()
{
    delete [ ] pool;
    delete [ ] freeBlocks;

#if defined(ESP32) || defined(ESP8266)
    vSemaphoreDelete(mutex);
#endif
}

void* BlockAllocator::allocate()
{
#if defined(ESP32) || defined(ESP8266)
    xSemaphoreTake(mutex , portMAX_DELAY);
#else
    std::lock_guard<std::mutex> lock(mutex);
#endif

    if (freeCount == 0)
    {
#if defined(ESP32) || defined(ESP8266)
        xSemaphoreGive(mutex);
#endif
        return nullptr;
    }

    void* block = freeBlocks[--freeCount];

#if defined(ESP32) || defined(ESP8266)
    xSemaphoreGive(mutex);
#endif

    return block;
}

void BlockAllocator::deallocate(void* ptr)
{
#if defined(ESP32) || defined(ESP8266)
    xSemaphoreTake(mutex , portMAX_DELAY);
#else
    std::lock_guard<std::mutex> lock(mutex);
#endif

    if (ptr >= pool && ptr < pool + poolSize)
    {
        freeBlocks[freeCount++] = ptr;
    }

#if defined(ESP32) || defined(ESP8266)
    xSemaphoreGive(mutex);
#endif
}

size_t BlockAllocator::getFreeCount() const
{
    return freeCount;
}

size_t BlockAllocator::getUsedCount() const
{
    return (poolSize / blockSize) - freeCount;
}

size_t BlockAllocator::getBlockSize() const
{
    return blockSize;
}

size_t BlockAllocator::getPoolSize() const
{
    return poolSize;
}

void BlockAllocator::reset()
{
#if defined(ESP32) || defined(ESP8266)
    xSemaphoreTake(mutex , portMAX_DELAY);
#else
    std::lock_guard<std::mutex> lock(mutex);
#endif

    initFreeBlocks();

#if defined(ESP32) || defined(ESP8266)
    xSemaphoreGive(mutex);
#endif
}

bool BlockAllocator::checkIntegrity() const
{
    for (size_t i = 0; i < freeCount; ++i)
    {
        if (freeBlocks[i] < pool || freeBlocks[i] >= pool + poolSize)
        {
            return false;
        }
    }
    return true;
}

size_t BlockAllocator::alignSize(size_t size) const
{
    size_t alignment = sizeof(void*);
    return (size + alignment - 1) & ~(alignment - 1);
}

void BlockAllocator::initFreeBlocks()
{
    freeCount = poolSize / blockSize;
    for (size_t i = 0; i < freeCount; ++i)
    {
        freeBlocks[i] = pool + i * blockSize;
    }
}

void BlockAllocator::defrag()
{
#if defined(ESP32) || defined(ESP8266)
    xSemaphoreTake(mutex , portMAX_DELAY);
#else
    std::lock_guard<std::mutex> lock(mutex);
#endif

    size_t usedBlockCount = (poolSize / blockSize) - freeCount;
    uint8_t* nextFreeBlock = pool + usedBlockCount * blockSize;

    for (size_t i = 0; i < freeCount; ++i)
    {
        uint8_t* block = reinterpret_cast<uint8_t*>(freeBlocks[i]);
        if (block >= nextFreeBlock)
        {
            std::swap(freeBlocks[i] , freeBlocks[--freeCount]);
            freeBlocks[freeCount++] = nextFreeBlock;
            nextFreeBlock += blockSize;
        }
    }

#if defined(ESP32) || defined(ESP8266)
    xSemaphoreGive(mutex);
#endif
}