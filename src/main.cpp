#include <Arduino.h>
#include <TFT_eSPI.h>
#include "config.h"
#include "BlockAllocator.h"

TFT_eSPI tft = TFT_eSPI();
BlockAllocator allocator(BLOCK_SIZE, POOL_SIZE);

void* allocatedBlocks[POOL_SIZE / BLOCK_SIZE] = {nullptr};

TaskHandle_t display_task_handle;
TaskHandle_t allo_task_handle;
TaskHandle_t deallo_task_handle;

int getRandom(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void drawGrid() {
    tft.fillScreen(TFT_BLACK);
    
    size_t numBlocks = allocator.getPoolSize() / allocator.getBlockSize();
    size_t cols = sqrt(numBlocks);
    size_t rows = numBlocks / cols;
    size_t cellWidth = tft.width() / cols;
    size_t cellHeight = tft.height() / rows;

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            tft.drawRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, TFT_LIGHTGREY);
        }
    }
}

void fillAllCellsGreen() {
    size_t numBlocks = allocator.getPoolSize() / allocator.getBlockSize();
    size_t cols = sqrt(numBlocks);
    size_t rows = numBlocks / cols;
    size_t cellWidth = tft.width() / cols;
    size_t cellHeight = tft.height() / rows;

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            tft.fillRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, TFT_DARKGREEN);
        }
    }
}

void updateCell(size_t index, uint16_t color) {
    size_t numBlocks = allocator.getPoolSize() / allocator.getBlockSize();
    size_t cols = sqrt(numBlocks);
    size_t rows = numBlocks / cols;
    size_t cellWidth = tft.width() / cols;
    size_t cellHeight = tft.height() / rows;

    size_t i = index / cols;
    size_t j = index % cols;

    tft.fillRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, color);
    tft.drawRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, TFT_LIGHTGREY);
}

void display_task(void* pvParameters) {
    while (1) {
        size_t numBlocks = allocator.getPoolSize() / allocator.getBlockSize();
        for (size_t i = 0; i < numBlocks; ++i) {
            uint16_t color = (allocatedBlocks[i] != nullptr) ? TFT_ORANGE : TFT_DARKGREEN;
            updateCell(i, color);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void allo_task(void* pvParameters) {
    while (1) {
        int numBlocks = getRandom(MIN_BLOCKS, MAX_BLOCKS);
        for (int i = 0; i < numBlocks; ++i) {
            void* block = allocator.allocate();
            if (block != nullptr) {
                for (int j = 0; j < POOL_SIZE / BLOCK_SIZE; ++j) {
                    if (allocatedBlocks[j] == nullptr) {
                        allocatedBlocks[j] = block;
                        break;
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        vTaskResume(deallo_task_handle);
        vTaskSuspend(NULL);
    }
}

void deallo_task(void* pvParameters) {
    while (1) {
        // int numBlocks = getRandom(MIN_BLOCKS, MAX_BLOCKS);
        int numBlocks = 14;
        for (int i = 0; i < numBlocks; ++i) {
            for (int j = 0; j < POOL_SIZE / BLOCK_SIZE; ++j) {
                if (allocatedBlocks[j] != nullptr) {
                    allocator.deallocate(allocatedBlocks[j]);
                    allocatedBlocks[j] = nullptr;
                    break;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(400));
        vTaskResume(allo_task_handle);
        vTaskSuspend(NULL);
    }
}

void setup() {
    tft.begin();
    tft.setRotation(1);

    drawGrid();
    fillAllCellsGreen();

    xTaskCreate(display_task, "Display Task", 2048, NULL, 1, &display_task_handle);
    xTaskCreate(allo_task, "Allocation Task", 2048, NULL, 1, &allo_task_handle);
    xTaskCreate(deallo_task, "Deallocation Task", 2048, NULL, 1, &deallo_task_handle);

    vTaskSuspend(deallo_task_handle);
}

void loop() {
}
