// #include <Arduino.h>
// #include <TFT_eSPI.h>
// #include "config.h"
// #include "BlockAllocator.h"


// TFT_eSPI tft = TFT_eSPI();
// BlockAllocator allocator(BLOCK_SIZE, POOL_SIZE);

// // Arr for pointers to allo blocks
// void* allocatedBlocks[POOL_SIZE / BLOCK_SIZE] = {nullptr};

// TaskHandle_t display_task_handle;
// TaskHandle_t allo_task_handle;
// TaskHandle_t deallo_task_handle;

// int getRandom(int min, int max) {
//     return rand() % (max - min + 1) + min;
// }

// // Draw grig. Called once per setup.
// void drawGrid() {
//     tft.fillScreen(TFT_BLACK);
    
//     size_t numBlocks = allocator.getPoolSize() / allocator.getBlockSize();
//     size_t cols = sqrt(numBlocks);
//     size_t rows = numBlocks / cols;
//     size_t cellWidth = tft.width() / cols;
//     size_t cellHeight = tft.height() / rows;

//     for (size_t i = 0; i < rows; ++i) {
//         for (size_t j = 0; j < cols; ++j) {
//             tft.drawRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, TFT_LIGHTGREY);
//         }
//     }
// }

// // Function to paint all cells green. Called once per setup.
// void fillAllCellsGreen() {
//     size_t numBlocks = allocator.getPoolSize() / allocator.getBlockSize();
//     size_t cols = sqrt(numBlocks);
//     size_t rows = numBlocks / cols;
//     size_t cellWidth = tft.width() / cols;
//     size_t cellHeight = tft.height() / rows;

//     for (size_t i = 0; i < rows; ++i) {
//         for (size_t j = 0; j < cols; ++j) {
//             tft.fillRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, TFT_DARKGREEN);
//         }
//     }
// }

// // function of coloring the specified cell in the desired color
// void updateCell(size_t index, uint16_t color) {
//     size_t numBlocks = allocator.getPoolSize() / allocator.getBlockSize();
//     size_t cols = sqrt(numBlocks);
//     size_t rows = numBlocks / cols;
//     size_t cellWidth = tft.width() / cols;
//     size_t cellHeight = tft.height() / rows;

//     size_t i = index / cols;
//     size_t j = index % cols;

//     tft.fillRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, color);
//     tft.drawRect(j * cellWidth, i * cellHeight, cellWidth, cellHeight, TFT_LIGHTGREY);
// }

// void display_task(void* pvParameters) {
//     while (1) {
//         size_t numBlocks = allocator.getPoolSize() / allocator.getBlockSize();
//         for (size_t i = 0; i < numBlocks; ++i) {
//             uint16_t color = (allocatedBlocks[i] != nullptr) ? TFT_ORANGE : TFT_DARKGREEN;
//             updateCell(i, color);
//         }

//         vTaskDelay(pdMS_TO_TICKS(500));
//     }
// }

// void displayGameOver() {
//     const char* message = "GAME OVER";
//     size_t len = strlen(message);
//     tft.setTextSize(4);
//     int16_t x = (tft.width() - tft.textWidth(message)) / 2;
//     int16_t y = (tft.height() - tft.fontHeight()) / 2;
//     uint16_t colors[] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_YELLOW, TFT_CYAN, TFT_MAGENTA};
//     size_t colorCount = sizeof(colors) / sizeof(colors[0]);
    
//     for (size_t i = 0; i < 10; ++i) {
//         tft.setTextColor(colors[i % colorCount]);
//         tft.drawString(message, x, y);
//         delay(500);
//     }
// }

// // allo_task & deallo_task: These tasks work in turn.

// void allo_task(void* pvParameters) {
//     while (1) {
//         int numBlocks = getRandom(MIN_BLOCKS, MAX_BLOCKS);
//         if (allocator.getFreeCount() < numBlocks) {
//             displayGameOver();
//             vTaskSuspend(NULL);
//         }
//         for (int i = 0; i < numBlocks; ++i) {
//             void* block = allocator.allocate();
//             if (block != nullptr) {
//                 for (int j = 0; j < POOL_SIZE / BLOCK_SIZE; ++j) {
//                     if (allocatedBlocks[j] == nullptr) {
//                         allocatedBlocks[j] = block;
//                         break;
//                     }
//                 }
//             }
//         }
//         vTaskDelay(pdMS_TO_TICKS(1000));
//         vTaskResume(deallo_task_handle);
//         vTaskSuspend(NULL);
//     }
// }

// void deallo_task(void* pvParameters) {
//     while (1) {
//         for (int i = 0; i < DE_BLOCKS; ++i) {
//             for (int j = 0; j < POOL_SIZE / BLOCK_SIZE; ++j) {
//                 if (allocatedBlocks[j] != nullptr) {
//                     allocator.deallocate(allocatedBlocks[j]);
//                     allocatedBlocks[j] = nullptr;
//                     break;
//                 }
//             }
//         }
//         vTaskDelay(pdMS_TO_TICKS(1000));
//         vTaskResume(allo_task_handle);
//         vTaskSuspend(NULL);
//     }
// }

// void setup() {
//     tft.begin();
//     tft.setRotation(3);

//     drawGrid();
//     fillAllCellsGreen();

//     xTaskCreate(display_task, "Display Task", 2048, NULL, 1, &display_task_handle);
//     xTaskCreate(allo_task, "Allocation Task", 2048, NULL, 1, &allo_task_handle);
//     xTaskCreate(deallo_task, "Deallocation Task", 2048, NULL, 1, &deallo_task_handle);

//     vTaskSuspend(deallo_task_handle);
// }

// void loop() {
// }
