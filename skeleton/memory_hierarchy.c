/*************************************************************************************|
|   1. YOU ARE NOT ALLOWED TO SHARE/PUBLISH YOUR CODE (e.g., post on piazza or online)|
|   2. Fill main.c and memory_hierarchy.c files                                       |
|   3. Do not use any other .c files neither alter main.h or parser.h                 |
|   4. Do not include any other library files                                         |
|*************************************************************************************/
#include "mipssim.h"

/// @students: declare cache-related structures and variables here
int address_length = 32;
int noOfOffsetBits = (2 + 2);  // 16 bytes in a block -> 2^4, so 4 bits for offset: 2 bits for block offset, 2 bits for byte offset
int noOfBlocks;
int noOfIndexBits;  // Index = log2(noOfBlocks)
int noOfTagBits; // Tag bits = address bit length - bits of index - bits of offset
                // Tag bits = 32 - log2(cache_size/16) - 4

typedef struct {
    int valid_bit;
    int tag;
    int index;
    int data[4];
} Block;

// Final cache structure
typedef struct {
    Block* blocks;
} Cache;

Cache myCache;

void memory_state_init(struct architectural_state* arch_state_ptr) {
    arch_state_ptr->memory = (uint32_t *) malloc(sizeof(uint32_t) * MEMORY_WORD_NUM);
    memset(arch_state_ptr->memory, 0, sizeof(uint32_t) * MEMORY_WORD_NUM);
    if(cache_size == 0){
        // CACHE DISABLED
        memory_stats_init(arch_state_ptr, 0); // WARNING: we initialize for no cache 0
    } else {
        // CACHE ENABLED
        //assert(0); /// @students: Remove assert(0); and initialize cache
        noOfBlocks = cache_size/16;
        noOfIndexBits = ceil(log2(noOfBlocks));  // Index = log2(noOfBlocks)
        noOfTagBits = address_length - noOfIndexBits - noOfOffsetBits; // Tag bits = address bit length - bits of index - bits of offset
                                                                          // Tag bits = 32 - log2(cache_size/16) - 4

        myCache.blocks = (Block *) malloc(noOfBlocks * sizeof(Block));

        for (int i = 0; i < noOfBlocks; i++) {
            myCache.blocks[i].valid_bit = 0;
            myCache.blocks[i].tag = 0;
            myCache.blocks[i].index = i;
            for (int j = 0; j < 4; j++){
                myCache.blocks[i].data[j] = 0;
            }
        }
        /// @students:
        memory_stats_init(arch_state_ptr, noOfTagBits); //<-- fill # of tag bits for cache 'X' correctly
    }
}

// returns data on memory[address / 4]
int memory_read(int address){
    arch_state.mem_stats.lw_total++;
    check_address_is_word_aligned(address);

    if(cache_size == 0){
        // CACHE DISABLED
        return (int) arch_state.memory[address / 4];
    } else {
        // CACHE ENABLED
        // EXAMPLE: 128 bytes for cache_size
        //printf("No of tag bits: %d\n", noOfTagBits);  // 25 bits
        //printf("No of index bits: %d\n", noOfIndexBits); // 3 bits
        //printf("No of offset bits: %d\n", noOfOffsetBits); // 4 bits

        int addressIndex = get_piece_of_a_word(address, noOfOffsetBits, noOfIndexBits);// Use index of address to move to the correct block number
        printf("Index: %d\n", addressIndex);
        int addressTag = get_piece_of_a_word(address, noOfOffsetBits+noOfIndexBits, noOfTagBits-1);
        printf("Tag: %d\n", addressTag);
        int addressBlockOffset = get_piece_of_a_word(address, noOfOffsetBits-2, noOfOffsetBits-2);
        printf("Block offset: %d\n", addressBlockOffset);
        printf("Valid bit of my cache at the index: %d\n",myCache.blocks[addressIndex].valid_bit);
        if (myCache.blocks[addressIndex].valid_bit == 1){

              if (myCache.blocks[addressIndex].tag == addressTag) {
                  arch_state.mem_stats.lw_cache_hits++;
                  return (int) myCache.blocks[addressIndex].data[addressBlockOffset];
              }

        } else {
                // go to main memory to find data and put this data in the cache
                int originalAddress = (int) arch_state.memory[address / 4];

                while (addressBlockOffset != 0){
                    address = address - 4;
                    addressBlockOffset = get_piece_of_a_word(address, noOfOffsetBits-2, noOfOffsetBits-2);
                }

                for (int i = 0; i < 4; i ++) {
                    myCache.blocks[addressIndex].data[i] = (int) arch_state.memory[(address + (i * 4)) / 4];
                    myCache.blocks[addressIndex].valid_bit = 1;
                    myCache.blocks[addressIndex].tag = addressTag;
                }

                return originalAddress;
        }

        //assert(0); /// @students: Remove assert(0); and implement Memory hierarchy w/ cache

        /// @students: your implementation must properly increment: arch_state_ptr->mem_stats.lw_cache_hits
    }
    return 0;
}

// writes data on memory[address / 4]
void memory_write(int address, int write_data){
    arch_state.mem_stats.sw_total++;
    check_address_is_word_aligned(address);

    if(cache_size == 0){
        // CACHE DISABLED
        arch_state.memory[address / 4] = (uint32_t) write_data;
    }else{
        // CACHE ENABLED

        int addressIndex = get_piece_of_a_word(address, noOfOffsetBits, noOfIndexBits);// Use index of address to move to the correct block number
        int addressTag = get_piece_of_a_word(address, noOfOffsetBits+noOfIndexBits, noOfTagBits);
        int addressBlockOffset = get_piece_of_a_word(address, noOfOffsetBits-2, noOfOffsetBits-2);

        if (myCache.blocks[addressIndex].valid_bit == 1) {
            if (myCache.blocks[addressIndex].tag == addressTag) {
                // WRITE-THROUGH POLICY: a hit; write to both cache and memory
                arch_state.mem_stats.sw_cache_hits++;
                myCache.blocks[addressIndex].data[addressBlockOffset] = write_data;
                arch_state.memory[address / 4] = (uint32_t) write_data;
            }
        } else {
              // WRITE-NO-ALLOCATE POLICY: do not allocate if a store does not find the target block in the cache
              arch_state.memory[address / 4] = (uint32_t) write_data;

        }
        //assert(0); /// @students: Remove assert(0); and implement Memory hierarchy w/ cache

        /// @students: your implementation must properly increment: arch_state_ptr->mem_stats.sw_cache_hits
    }
}
