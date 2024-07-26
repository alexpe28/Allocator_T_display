#ifndef CONFIG_H
#define CONFIG_H

#define BLOCK_SIZE 8    // Memory block size
#define POOL_SIZE 320   // Total size of the memory pool
#define MIN_BLOCKS 1    // Minimum number of blocks to allocate (used by randomizer)
#define MAX_BLOCKS 15   // Maximum number of blocks to allocate (used by randomizer)
#define DE_BLOCKS  5    // Fixed number of blocks to free. Used in deallo_task to show interesting pictures of memory fragmentation on the display.

#endif // CONFIG_H