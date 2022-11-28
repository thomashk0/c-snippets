#ifndef _SURVIVOR_SLICE_H_
#define _SURVIVOR_SLICE_H_

#include <stddef.h>

typedef struct slice
{
    int* ptr;
    size_t size;
} slice_t;

int
slice_init(slice_t* dst, size_t size);

int
slice_release(slice_t* dst);

#endif
