#include <string.h>

/*
 * mm-implicit.c - The best malloc package EVAR!
 *
 * TODO (bug): mm_realloc and mm_calloc don't seem to be working...
 * TODO (bug): The allocator doesn't re-use space very well...
 */

#include <stdint.h>

#include "mm.h"
#include "memlib.h"

/** The required alignment of heap payloads */
const size_t ALIGNMENT = 2 * sizeof(size_t);

/** The layout of each block allocated on the heap */
typedef struct {
    /** The size of the block and whether it is allocated (stored in the low bit) */
    size_t header;
    /**
     * We don't know what the size of the payload will be, so we will
     * declare it as a zero-length array.  This allow us to obtain a
     * pointer to the start of the payload.
     */
    uint8_t payload[];
} block_t;

/** The first and last blocks on the heap */
static block_t *mm_heap_first = NULL;
static block_t *mm_heap_last = NULL;

/** Rounds up `size` to the nearest multiple of `n` */
static size_t round_up(size_t size, size_t n) {
    return (size + (n - 1)) / n * n;
}

/** Set's a block's header with the given size and allocation state */
static void set_header(block_t *block, size_t size, bool is_allocated) {
    block->header = size | is_allocated;
}

/** Extracts a block's size from its header */
static size_t get_size(block_t *block) {
    return block->header & ~1;
}

/** Extracts a block's allocation state from its header */
static bool is_allocated(block_t *block) {
    return block->header & 1;
}

/**
 * Finds the first free block in the heap with at least the given size.
 * If no block is large enough, returns NULL.
 */
static block_t *find_fit(size_t size) {
    // Traverse the blocks in the heap using the implicit list
    for (
        block_t *curr = mm_heap_first;
        mm_heap_last != NULL && curr <= mm_heap_last;
        curr = (void *) curr + get_size(curr)
    ) {
        // If the block is free and large enough for the allocation, return it
        if (!is_allocated(curr) && get_size(curr) >= size) {
            return curr;
        }
    }
    return NULL;
}

/** Gets the header corresponding to a given payload pointer */
static block_t *block_from_payload(void *ptr) {
    return ptr - offsetof(block_t, payload);
}


/**
 * mm_init - Initializes the allocator state
 */
bool mm_init(void) {
    // We want the first payload to start at ALIGNMENT bytes from the start of the heap
    void *padding = mem_sbrk(ALIGNMENT - sizeof(block_t));
    if (padding == (void *) -1) {
        return false;
    }

    // Initialize the heap with no blocks
    mm_heap_first = NULL;
    mm_heap_last = NULL;
    return true;
}

/**
 * mm_malloc - Allocates a block with the given size
 */
void *mm_malloc(size_t size) {
    // The block must have enough space for a header and be 16-byte aligned
    size = round_up(sizeof(block_t) + size, ALIGNMENT);

    block_t *curr = mm_heap_first;
    while (curr != NULL && curr != mm_heap_last) {
        block_t *next = (block_t *)((char *)curr + get_size(curr));
        if (!is_allocated(curr) && !is_allocated(next)) {
            set_header(curr, get_size(curr) + get_size(next), false);
            if (next == mm_heap_last) {
                mm_heap_last = curr;
            }
            continue;
        }
        curr = (block_t *)((char *)curr + get_size(curr));
    }

    // If there is a large enough free block, use it
    block_t *block = find_fit(size);
    if (block != NULL) {
        size_t block_size = get_size(block);
        size_t min_block_size = sizeof(block_t) + ALIGNMENT;
        if (block_size >= size + min_block_size) {
            set_header(block, size, true);
            block_t *remainder = (block_t *)((char *)block + size);
            set_header(remainder, block_size - size, false);
            if (block == mm_heap_last) {
                mm_heap_last = remainder;
            }
        } else {
            set_header(block, block_size, true);
        }
        return block->payload;
    }

    // Otherwise, a new block needs to be allocated at the end of the heap
    block = mem_sbrk(size);
    if (block == (void *) -1) {
        return NULL;
    }

    // Update mm_heap_first and mm_heap_last since we extended the heap
    if (mm_heap_first == NULL) {
        mm_heap_first = block;
    }
    mm_heap_last = block;

    // Initialize the block with the allocated size
    set_header(block, size, true);
    return block->payload;
}

/**
 * mm_free - Releases a block to be reused for future allocations
 */
void mm_free(void *ptr) {
    // mm_free(NULL) does nothing
    if (ptr == NULL) {
        return;
    }

    // Mark the block as unallocated
    block_t *block = block_from_payload(ptr);
    set_header(block, get_size(block), false);
}

/**
 * mm_realloc - Change the size of the block by mm_mallocing a new block,
 *      copying its data, and mm_freeing the old block.
 */
void *mm_realloc(void *oldptr, size_t size) {
    if (oldptr == NULL) {
        return mm_malloc(size);
    }
    if (size == 0) {
        mm_free(oldptr);
        return NULL;
    }
    block_t *old_block = block_from_payload(oldptr);
    size_t old_size = get_size(old_block) - sizeof(size_t);
    void *newptr = mm_malloc(size);
    if (newptr == NULL) {
        return NULL;
    }
    size_t copy_size = (old_size < size) ? old_size : size;
    memcpy(newptr, oldptr, copy_size);
    mm_free(oldptr);
    return newptr;
}

/**
 * mm_calloc - Allocate the block and set it to zero.
 */
void *mm_calloc(size_t nmemb, size_t size) {
    if (nmemb == 0 || size == 0) {
        return NULL;
    }
    size_t total = nmemb * size;
    void *ptr = mm_malloc(total);
    if (ptr == NULL) {
        return NULL;
    }
    memset(ptr, 0, total);
    return ptr;
}

/**
 * mm_checkheap - So simple, it doesn't need a checker!
 */
void mm_checkheap(void) {

}
