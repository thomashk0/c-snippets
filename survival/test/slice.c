#include "slice.h"

#include <stdlib.h>

#include <survival.h>

// Constructor
int
slice_init(slice_t* dst, size_t size)
{
    if (!dst) {
        return -1;
    }
    *dst = (slice_t){ .ptr = NULL, .size = 0 };
    void* p = calloc(sizeof(int), size);
    if (p == NULL) {
        return -1;
    }
    *dst = (slice_t){ .ptr = p, .size = size };
    return 0;
}

// Destructor.
int
slice_release(slice_t* dst)
{
    if (!dst) {
        return -1;
    }
    // NOTE: freeing a NULL pointer is OK.
    free(dst->ptr);
    dst->ptr = NULL;
    dst->size = 0;
    DPRINTF("call to slice_release\n");
    return 0;
}
