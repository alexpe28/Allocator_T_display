#include <unity.h>
#include "BlockAllocator.h"

void test_allocation_and_deallocation() {
    const size_t blockSize = 32;
    const size_t poolSize = 320;
    BlockAllocator allocator(blockSize, poolSize);
    
    void* blocks[10];
    for (int i = 0; i < 10; ++i) {
        blocks[i] = allocator.allocate();
        TEST_ASSERT_NOT_NULL(blocks[i]);
    }

    void* nullBlock = allocator.allocate();
    TEST_ASSERT_NULL(nullBlock);

    for (int i = 0; i < 10; ++i) {
        allocator.deallocate(blocks[i]);
    }

    for (int i = 0; i < 10; ++i) {
        blocks[i] = allocator.allocate();
        TEST_ASSERT_NOT_NULL(blocks[i]);
    }
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_allocation_and_deallocation);
    UNITY_END();
}

void loop() {
}