#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define C_BASE64_IMPL
#include "../base64.h"

#define scoped_ptr __attribute__((cleanup(cleanup_free)))

static void
cleanup_free(void* p)
{
    free(*(void**)p);
}

int
LLVMFuzzerTestOneInput(const uint8_t* payload, size_t size)
{
    const char* p = (const char*)payload;
    // Make the input null-terminated
    long p_b64_len = 0;
    scoped_ptr char* p_b64 = b64_encode(&p_b64_len, p, size);
    if (p_b64 == NULL) {
        return 1;
    }

    // Try to decode the input bytes "as this", most of the time, this should
    // fail.
    scoped_ptr char* p_chk = b64_decode(NULL, p, size);
    (void)p_chk;

    long p_base_len = 0;
    scoped_ptr char* p_base = b64_decode(&p_base_len, p_b64, p_b64_len);
    if (p_base == NULL) {
        return 1;
    }

    return 0;
}
