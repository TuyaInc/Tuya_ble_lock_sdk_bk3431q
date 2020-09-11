#include "sf_mem.h"




/*********************************************************************
 * LOCAL CONSTANT
 */

/*********************************************************************
 * LOCAL STRUCT
 */
typedef struct mem                /* << Memory Pool management struct >>     */
{
    struct mem *next;               /* Next Memory Block in the list           */
    u32         len;                /* Length of data block                    */
} MEMP;

/*********************************************************************
 * LOCAL VARIABLE
 */

/*********************************************************************
 * VARIABLE
 */
u32       os_stack_mem2[MEM_SIZE];
u32 const os_stack_sz2 = sizeof(os_stack_mem2);

/*********************************************************************
 * LOCAL FUNCTION
 */




// Initialize Dynamic Memory pool
//   Parameters:
//     pool:    Pointer to memory pool
//     size:    Size of memory pool in bytes
//   Return:    0 - OK, 1 - Error
static u32 init_mem (void *pool, u32 size)
{
    MEMP *ptr;

    if ((pool == NULL) || (size < sizeof(MEMP)))
    {
        return (1U);
    }

    ptr = (MEMP *)pool;
    ptr->next = (MEMP *)((u32)pool + size - sizeof(MEMP *));
    ptr->next->next = NULL;
    ptr->len = 0U;

    return (0U);
}

// Allocate Memory from Memory pool
//   Parameters:
//     pool:    Pointer to memory pool
//     size:    Size of memory in bytes to allocate
//   Return:    Pointer to allocated memory
static void *alloc_mem (void *pool, u32 size)
{
    MEMP *p, *p_search, *p_new;
    u32   hole_size;

    if ((pool == NULL) || (size == 0U))
    {
        return NULL;
    }

    /* Add header offset to 'size' */
    size += sizeof(MEMP);
    /* Make sure that block is 4-byte aligned  */
    size = (size + 3U) & ~(u32)3U;

    p_search = (MEMP *)pool;
    while (1)
    {
        hole_size  = (u32)p_search->next - (u32)p_search;
        hole_size -= p_search->len;
        /* Check if hole size is big enough */
        if (hole_size >= size)
        {
            break;
        }
        p_search = p_search->next;
        if (p_search->next == NULL)
        {
            /* Failed, we are at the end of the list */
            return NULL;
        }
    }

    if (p_search->len == 0U)
    {
        /* No block is allocated, set the Length of the first element */
        p_search->len = size;
        p = (MEMP *)(((u32)p_search) + sizeof(MEMP));
    }
    else
    {
        /* Insert new list element into the memory list */
        // Create a new node
        p_new       = (MEMP *)((u32)p_search + p_search->len);
        p_new->len  = size;
        // Insert the new node in pSearch and pSearch->next
        p_new->next = p_search->next;
        p_search->next = p_new;

        p = (MEMP *)(((u32)p_new) + sizeof(MEMP));
    }

    memset( p, 0, size - sizeof(MEMP) );
    return (p);
}

// Free Memory and return it to Memory pool
//   Parameters:
//     pool:    Pointer to memory pool
//     mem:     Pointer to memory to free
//   Return:    0 - OK, 1 - Error
static u32 free_mem (void *pool, void *mem)
{
    MEMP *p_search, *p_prev, *p_return;

    if ((pool == NULL) || (mem == NULL))
    {
        return (1U);
    }

    p_return = (MEMP *)((u32)mem - sizeof(MEMP));

    /* Set list header */
    p_prev = NULL;
    p_search = (MEMP *)pool;
    while (p_search != p_return)
    {
        p_prev   = p_search;
        p_search = p_search->next;
        if (p_search == NULL)
        {
            /* Valid Memory block not found */
            return (1U);
        }
    }

    if (p_prev == NULL)
    {
        /* First block to be released, only set length to 0 */
        p_search->len = 0U;
    }
    else
    {
        /* Discard block from chain list */
        p_prev->next = p_search->next;
    }

    return (0U);
}


bool sd_mem_init( void )
{
    return !(init_mem( os_stack_mem2, os_stack_sz2 ));
}

void* sd_malloc( u32 size )
{
    return alloc_mem( os_stack_mem2, size );
}

bool sd_free( void* mem )
{
    return !(free_mem( os_stack_mem2, mem ));
}



