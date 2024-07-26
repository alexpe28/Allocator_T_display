#include <unity.h>
#include "BlockAllocator.h"

void test_allocation_and_deallocation() {
    BlockAllocator allocator(16, 256);

    void* block1 = allocator.allocate();
    void* block2 = allocator.allocate();
    TEST_ASSERT_NOT_NULL(block1);
    TEST_ASSERT_NOT_NULL(block2);
    TEST_ASSERT_EQUAL(14, allocator.getFreeCount());

    allocator.deallocate(block1);
    allocator.deallocate(block2);
    TEST_ASSERT_EQUAL(16, allocator.getFreeCount());
}

void test_overflow() {
    BlockAllocator allocator(16, 256);

    for (int i = 0; i < 16; ++i) {
        TEST_ASSERT_NOT_NULL(allocator.allocate());
    }
    TEST_ASSERT_NULL(allocator.allocate());
}

void test_used_and_free_count() {
    BlockAllocator allocator(16, 256);

    TEST_ASSERT_EQUAL(0, allocator.getUsedCount());
    TEST_ASSERT_EQUAL(16, allocator.getFreeCount());

    void* block = allocator.allocate();
    TEST_ASSERT_EQUAL(1, allocator.getUsedCount());
    TEST_ASSERT_EQUAL(15, allocator.getFreeCount());

    allocator.deallocate(block);
    TEST_ASSERT_EQUAL(0, allocator.getUsedCount());
    TEST_ASSERT_EQUAL(16, allocator.getFreeCount());
}

void test_reset_allocator() {
    BlockAllocator allocator(16, 256);

    allocator.allocate();
    allocator.allocate();
    TEST_ASSERT_EQUAL(14, allocator.getFreeCount());

    allocator.reset();
    TEST_ASSERT_EQUAL(16, allocator.getFreeCount());
    TEST_ASSERT_EQUAL(0, allocator.getUsedCount());
}

void test_integrity() {
    BlockAllocator allocator(16, 256);

    TEST_ASSERT_TRUE(allocator.checkIntegrity());
    void* block = allocator.allocate();
    TEST_ASSERT_TRUE(allocator.checkIntegrity());

    allocator.deallocate(block);
    TEST_ASSERT_TRUE(allocator.checkIntegrity());
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_allocation_and_deallocation);
    RUN_TEST(test_overflow);
    RUN_TEST(test_used_and_free_count);
    RUN_TEST(test_reset_allocator);
    RUN_TEST(test_integrity);
    UNITY_END();
}

void loop() {
}
