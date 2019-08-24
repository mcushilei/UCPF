/*******************************************************************************
 *  Copyright(C)2019 by Dreistein<mcu_shilei@hotmail.com>                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify it   *
 *  under the terms of the GNU Lesser General Public License as published     *
 *  by the Free Software Foundation; either version 3 of the License, or      *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful, but       *
 *  WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU          *
 *  General Public License for more details.                                  *
 *                                                                            *
 *  You should have received a copy of the GNU Lesser General Public License  *
 *  along with this program; if not, see http://www.gnu.org/licenses/.        *
*******************************************************************************/




/*============================ INCLUDES ======================================*/
#include ".\app_cfg.h"
#include ".\heap_first_fit.h"
#include <string.h>

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct heap_block_link_t heap_block_link_t;
struct heap_block_link_t {
    /* The next free block in the list. */
	heap_block_link_t  *NextFreeBlock;
    /* The size of the free block. */
	size_t              BlockSize;
};

/*============================ PRIVATE PROTOTYPES ============================*/
#ifndef HEAP_MEM_ALIGNMENT
#define HEAP_MEM_ALIGNMENT               (3)             //! in power of 2.
#endif

#ifndef HEAP_MEM_CRITICAL_SECTION_BEGIN
#define HEAP_MEM_CRITICAL_SECTION_BEGIN()
#endif


#ifndef HEAP_MEM_CRITICAL_SECTION_END
#define HEAP_MEM_CRITICAL_SECTION_END()
#endif


#define HEAP_ALIGNMENT_OF_BLOCK_LINK    (( (sizeof(void *) - 1u) | (sizeof(size_t) - 1u) ) + 1u)
#define HEAP_ALIGNMENT_OF_CFG           (1u << HEAP_MEM_ALIGNMENT)
/* The alignemtn used of heap is the greater one between HEAP_ALIGNMENT_OF_BLOCK_LINK 
and HEAP_ALIGNMENT_OF_CFG. */
#define HEAP_ALIGNMENT                  (( (HEAP_ALIGNMENT_OF_BLOCK_LINK - 1u) | (HEAP_ALIGNMENT_OF_CFG - 1u) ) + 1u)
#define HEAP_ALIGNMENT_MASK             ((size_t)HEAP_ALIGNMENT - 1u)

/* Block sizes must not get too small and must be kept aligned. */
#define HEAP_MINIMUM_BLOCK_SIZE	        (heapStructSize + HEAP_ALIGNMENT)


/*============================ PRIVATE VARIABLES =============================*/

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void heap_insert_block_into_freelist( heap_block_link_t *pBlockToInsert );


/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
static const size_t heapStructSize = (sizeof(heap_block_link_t) + HEAP_ALIGNMENT_MASK) & ~HEAP_ALIGNMENT_MASK;

static const heap_block_link_t heapListZeroValue = { .NextFreeBlock = NULL, .BlockSize = 0 };
/* Create a couple of list links to mark the start and end of the list. */
static       heap_block_link_t heapListStart    = { NULL };

/* Keeps track of the number of free bytes remaining, but says nothing about
fragmentation. */
static size_t heapFreeBytesRemaining               = 0U;
static size_t heapMinimumEverFreeBytesRemaining    = 0U;

/*============================ PUBLIC VARIABLES ==============================*/
/*============================ IMPLEMENTATION ================================*/

static void heap_insert_block_into_freelist( heap_block_link_t *pBlockToInsert )
{
    heap_block_link_t *pIterator, *pNextBlock;
    char *puc;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for (pIterator = &heapListStart;
        (pIterator->NextFreeBlock != NULL) && (pIterator->NextFreeBlock < pBlockToInsert);
        pIterator = pIterator->NextFreeBlock) {
		/* Nothing to do here, just iterate to the right position. */
	}
    /* pBlockToInsert will be inserted between pIterator and pNextBlock. */
    pNextBlock = pIterator->NextFreeBlock;

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	puc = (char *)pIterator;
	if( (puc + pIterator->BlockSize) == (char *)pBlockToInsert ) {
		pIterator->BlockSize += pBlockToInsert->BlockSize;
		pBlockToInsert = pIterator;
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	puc = (char *)pBlockToInsert;
	if( (puc + pBlockToInsert->BlockSize) == (char *)pNextBlock ) {
		if( pNextBlock == NULL ) {
			pBlockToInsert->NextFreeBlock = NULL;
		} else {
			/* Form one big block from the two blocks. */
			pBlockToInsert->BlockSize    += pNextBlock->BlockSize;
			pBlockToInsert->NextFreeBlock = pNextBlock->NextFreeBlock;
		}
	} else {
		pBlockToInsert->NextFreeBlock = pNextBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's NextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	if( pIterator != pBlockToInsert ) {
		pIterator->NextFreeBlock = pBlockToInsert;
	}
}

void heap_init( const heap_memory_cfg_t cfg[], size_t length )
{
    heap_block_link_t *freeBlock;
    size_t address;
    size_t size;

	HEAP_MEM_CRITICAL_SECTION_BEGIN();
    heapListStart = heapListZeroValue;
	heapMinimumEverFreeBytesRemaining  = 0;
	heapFreeBytesRemaining             = 0;

    for (size_t i = 0; i < length; i++) {

        address = (size_t)cfg[i].Address;
        size    = cfg[i].Size;

        if (size < HEAP_MINIMUM_BLOCK_SIZE) {
            continue;
        }

	    /* Ensure the heap starts on a correctly aligned boundary. */
	    address +=  HEAP_ALIGNMENT_MASK;
	    address &= ~HEAP_ALIGNMENT_MASK;
	    size -= address - (size_t)cfg[i].Address;

        freeBlock = (void *)address;

	    /* Ensure the heap end on a correctly aligned boundary. */
	    address += size;
		size -= address & HEAP_ALIGNMENT_MASK;

        /* second check on the block size after pruning. */
        if (size < HEAP_MINIMUM_BLOCK_SIZE) {
            continue;
        }

	    freeBlock->BlockSize        = size;
	    freeBlock->NextFreeBlock    = NULL;

        heap_insert_block_into_freelist(freeBlock);

	    heapMinimumEverFreeBytesRemaining  += size;
	    heapFreeBytesRemaining             += size;
    }

    heapListStart.BlockSize = heapFreeBytesRemaining;
	HEAP_MEM_CRITICAL_SECTION_END();
}

void heap_deinit(void)
{
	HEAP_MEM_CRITICAL_SECTION_BEGIN();
    heapListStart = heapListZeroValue;
	heapMinimumEverFreeBytesRemaining  = 0;
	heapFreeBytesRemaining             = 0;
	HEAP_MEM_CRITICAL_SECTION_END();
}

void *heap_malloc( size_t requiredSize )
{
    heap_block_link_t *pBlock, *pPreviousBlock, *pNewBlockLink;
    void *mem = NULL;

    if (0u == requiredSize) {
        return NULL;
    }

	/* If this is the first call to malloc then the heap will require
	initialisation to setup the list of free blocks. */
	if( heapListStart.BlockSize == 0u ) {
		return NULL;
	}

	/* The wanted size is increased so it can contain a heap_block_link_t
	structure in addition to the requested amount of bytes. */
	requiredSize += heapStructSize;

    /* Increase wanted size to HEAP_MINIMUM_BLOCK_SIZE. */
    if (requiredSize < HEAP_MINIMUM_BLOCK_SIZE) {
        requiredSize = HEAP_MINIMUM_BLOCK_SIZE;
    }

	/* Ensure that blocks are always aligned to the required number
	of bytes. */
    requiredSize +=  HEAP_ALIGNMENT_MASK;
    requiredSize &= ~HEAP_ALIGNMENT_MASK;

	HEAP_MEM_CRITICAL_SECTION_BEGIN();
	do {
        if (requiredSize > heapFreeBytesRemaining) {
            break;
        }

		/* Traverse the list from the start	(lowest address) block until
		one	of adequate size is found. */
		pPreviousBlock  = &heapListStart;
		pBlock          = heapListStart.NextFreeBlock;
		while( (pBlock != NULL) && (pBlock->BlockSize < requiredSize) ) {
			pPreviousBlock  = pBlock;
			pBlock          = pBlock->NextFreeBlock;
		}
		/* If the end marker was reached then a block of adequate size
		was	not found. */
        if (pBlock == NULL) {
            break;
        }

		/* This block is being taken out of the list of free blocks. */
		pPreviousBlock->NextFreeBlock = pBlock->NextFreeBlock;

		/* If the block is larger than required and large enough to be split
			into two. */
		if( (pBlock->BlockSize - requiredSize) > HEAP_MINIMUM_BLOCK_SIZE ) {
			/* This block is to be split into two.  Create a new
			block following the number of bytes requested. The void
			cast is used to prevent byte alignment warnings from the
			compiler. */
			pNewBlockLink = (void *)( ((char *)pBlock) + requiredSize );

			/* Calculate the sizes of two blocks split from the
			single block. */
			pNewBlockLink->BlockSize    = pBlock->BlockSize - requiredSize;
			pBlock->BlockSize           = requiredSize;

			/* Insert the new block into the list of free blocks. */
            pNewBlockLink->NextFreeBlock    = pBlock->NextFreeBlock;
            pPreviousBlock->NextFreeBlock   = pNewBlockLink;
		}

		heapFreeBytesRemaining -= pBlock->BlockSize;
		if( heapFreeBytesRemaining < heapMinimumEverFreeBytesRemaining ) {
			heapMinimumEverFreeBytesRemaining = heapFreeBytesRemaining;
		}

		/* The block is being returned - it is allocated and owned
		by the application and has no "next" block. */
		pBlock->NextFreeBlock = NULL;

        mem = (void *)((char *)pBlock + heapStructSize);
    } while (0);
	HEAP_MEM_CRITICAL_SECTION_END();

    return mem;
}

void heap_free( void *memory )
{
    heap_block_link_t *pBlock;

    if (memory == NULL) {
        return;
    }

    /* If this is called before we have initialised it.
	   This should not happen. */
	if( heapListStart.BlockSize == 0u ) {
        return;
	}

    /* The memory we allacte is always alligned. 
	   This should not happen. */
    if (HEAP_ALIGNMENT_MASK & (size_t)memory) {
        return;
    }

	/* The memory being freed will have an heap_block_link_t structure immediately
	before it. */
	pBlock = (heap_block_link_t *)((char *)memory - heapStructSize);

	HEAP_MEM_CRITICAL_SECTION_BEGIN();
	do {
        /* Make sure that this block is valid. 
	       This should not happen. */
        if (pBlock->NextFreeBlock != NULL) {
            break;
        }
        if (pBlock->BlockSize < HEAP_MINIMUM_BLOCK_SIZE) {
            break;
        }

		heapFreeBytesRemaining += pBlock->BlockSize;

		/* Add this block to the list of free blocks. */
		heap_insert_block_into_freelist( pBlock );
    } while (0);
	HEAP_MEM_CRITICAL_SECTION_END();
}

void *heap_realloc(void *memory, size_t newSize)
{
    heap_block_link_t *pBlock, *pNextBlock, *pNewBlockLink, *pIterator;
    size_t oldSize, growSize, address;
    void *newMemory = memory;

    if (memory == NULL) {
        return heap_malloc(newSize);
    }

    if (newSize == 0) {
        heap_free(memory);
        return NULL;
    }

    /* If this is called before we have initialised it.
	   This should not happen. */
	if( heapListStart.BlockSize == 0u ) {
        return NULL;
	}

    /* The memory we allacte is always alligned. 
	   This should not happen. */
    if (HEAP_ALIGNMENT_MASK & (size_t)memory) {
        return NULL;
    }

	/* The memory being freed will have an heap_block_link_t structure immediately
	before it. */
	pBlock = (heap_block_link_t *)((char *)memory - heapStructSize);

    /* Make sure that this block is valid. 
	    This should not happen. */
    if (pBlock->NextFreeBlock != NULL) {
        return NULL;
    }
    if (pBlock->BlockSize < HEAP_MINIMUM_BLOCK_SIZE) {
        return NULL;
    }

	oldSize = pBlock->BlockSize;

	/* The wanted size is increased so it can contain a heap_block_link_t
	structure in addition to the requested amount of bytes. */
	newSize += heapStructSize;

    /* Increase wanted size to HEAP_MINIMUM_BLOCK_SIZE. */
    if (newSize < HEAP_MINIMUM_BLOCK_SIZE) {
        newSize = HEAP_MINIMUM_BLOCK_SIZE;
    }

	/* Ensure that blocks are always aligned to the required number
	of bytes. */
    newSize +=  HEAP_ALIGNMENT_MASK;
    newSize &= ~HEAP_ALIGNMENT_MASK;

	HEAP_MEM_CRITICAL_SECTION_BEGIN();
	do {
        if (oldSize == newSize) {
            /* nothing to do. */
            break;

        } else if (oldSize > newSize) {
            /* if freed memory size is smaller than HEAP_MINIMUM_BLOCK_SIZE, then nothing to do. */
            if ((oldSize - newSize) < HEAP_MINIMUM_BLOCK_SIZE) {
                break;
            }

            /* caculate the address of block to free. */
            address = (size_t)memory + newSize;
            pNewBlockLink = (void *)address;
            pNewBlockLink->BlockSize        = oldSize - newSize;
            pNewBlockLink->NextFreeBlock    = NULL;

            pBlock->BlockSize  = newSize;
            heapFreeBytesRemaining += pNewBlockLink->BlockSize;

            /* Add this block to the list of free blocks. */
		    heap_insert_block_into_freelist( pNewBlockLink );

        } else {
            pNextBlock = (void *)((char *)pBlock + pBlock->BlockSize);
            growSize = newSize - oldSize;

            if (pNextBlock != NULL) {
            
                /* if the block next to it has not been allocated. */
                if (pNextBlock->NextFreeBlock != NULL) {

                    /* and it's large enough. */
                    if (growSize <= pNextBlock->BlockSize) {

	                    for( pIterator = &heapListStart; pIterator->NextFreeBlock != pNextBlock; pIterator = pIterator->NextFreeBlock ) {
		                    /* Nothing to do here, just iterate to the right position. */
	                    }

				        /* If the block is larger enough to be split into two. */
				        if( (pNextBlock->BlockSize - growSize) > HEAP_MINIMUM_BLOCK_SIZE ) {
                            heap_block_link_t tmp;

                            /* let pNewBlockLink to the new position of pNextBlock. */
                            pNewBlockLink = (void *)((size_t)pNextBlock + growSize);

                            /* clear the memory pNextBlock used. */
                            tmp = *pNextBlock;
                            *pNextBlock = heapListZeroValue;

                            /* set the values of pNewBlockLink. */
                            tmp.BlockSize -= growSize;
                            *pNewBlockLink = tmp;

                            /* adjust the list to new node. */
                            pIterator->NextFreeBlock = pNewBlockLink;

                        /* otherwise the entire of the next block will be allocated. */
                        } else {
                            growSize = pNextBlock->BlockSize;

                            /* take the pNextBlock off the list. */
                            pIterator->NextFreeBlock = pNextBlock->NextFreeBlock;

                            /* clear the memory pNextBlock used. */
					        *pNextBlock = heapListZeroValue;
                        }

				        pBlock->BlockSize += growSize;

                        heapFreeBytesRemaining -= growSize;
				        if( heapFreeBytesRemaining < heapMinimumEverFreeBytesRemaining ) {
					        heapMinimumEverFreeBytesRemaining = heapFreeBytesRemaining;
				        }

                        break;
                    }
                }
            }

            /* allocate a new block, copy data to it and then free the old one. */
            newMemory = heap_malloc(newSize);
            if (NULL == newMemory) {
                /* there is no space to allocate the memory, so nothing to do. */
                break;
            }

            memcpy(newMemory, memory, oldSize);
            heap_free(memory);
        }
    } while (0);
	HEAP_MEM_CRITICAL_SECTION_END();

    return newMemory;
}

size_t heap_remaining( void )
{
    size_t value;

	HEAP_MEM_CRITICAL_SECTION_BEGIN();
    value = heapFreeBytesRemaining;
	HEAP_MEM_CRITICAL_SECTION_END();
	return value;
}

size_t heap_minimum_remaining( void )
{
    size_t value;

	HEAP_MEM_CRITICAL_SECTION_BEGIN();
    value = heapMinimumEverFreeBytesRemaining;
	HEAP_MEM_CRITICAL_SECTION_END();
	return value;
}

