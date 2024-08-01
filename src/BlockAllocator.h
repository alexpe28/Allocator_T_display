#ifndef BLOCKALLOCATOR_H
#define BLOCKALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

#if defined(ESP32) || defined(ESP8266)
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#else
#include <mutex>
#endif

class BlockAllocator {
public:
    BlockAllocator(size_t blockSize, size_t poolSize);
    ~BlockAllocator();

    void* allocate();
    void deallocate(void* ptr);
    void reset();
    void defrag();

    size_t getFreeCount() const;
    size_t getUsedCount() const;
    size_t getBlockSize() const;
    size_t getPoolSize() const;
     
    bool checkIntegrity() const;

private:
    size_t blockSize;
    size_t poolSize;
    size_t freeCount;
    uint8_t* pool;
    void** freeBlocks;

#if defined(ESP32) || defined(ESP8266)
    SemaphoreHandle_t mutex;
#else
    std::mutex mutex;
#endif

    size_t alignSize(size_t size) const;
    void initFreeBlocks();
};

#endif // BLOCKALLOCATOR_H
