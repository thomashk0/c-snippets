#ifndef C_BASE64_H
#define C_BASE64_H

/// \section LICENSE
///
/// The c_base64 project uses a zlib/libpng license.
///
/// Copyright (c) 2021 Thomas Hiscock <thomashk000@gmail.com>
///
/// This software is provided 'as-is', without any express or implied warranty.
/// In no event will the authors be held liable for any damages arising from the
/// use of this software.
///
/// Permission is granted to anyone to use this software for any purpose,
/// including commercial applications, and to alter it and redistribute it
/// freely, subject to the following restrictions:
///
/// 1. The origin of this software must not be misrepresented; you must not
///    claim that you wrote the original software. If you use this software in a
///    product, an acknowledgment in the product documentation would be
///    appreciated but is not required.
///
/// 2. Altered source versions must be plainly marked as such, and must not be
///    misrepresented as being the original software.
///
/// 3. This notice may not be removed or altered from any source distribution.

#if defined _WIN32 || defined __CYGWIN__
#ifdef COMPILING_C_BASE64
#define C_BASE64_API __declspec(dllexport)
#else
#define C_BASE64_API __declspec(dllimport)
#endif
#else
#ifdef COMPILING_C_BASE64
#define C_BASE64_API __attribute__((visibility("default")))
#else
#define C_BASE64_API
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// Length of the base64 representation for a byte array of \p size bytes.
#define b64_repr_len(size) ((((size) + 2) / 3) * 4)

/// Length of the byte representation for a base64 array of \p size elements.
#define byte_repr_len(size) ((((size) + 3) / 4) * 3)

/// Convert bytes into base64 format.
///
/// \param dst Destination buffer, where to write base 64 representation.
/// \param dst_capacity
///     The maximum number of bytes that can be written
///     in \p dst.
/// \param src Source buffer (raw bytes).
/// \param src_len Length of the source buffer.
///
/// \return
///     If this function succeeds, it returns the a positive number
///     representing the number of bytes written in \p dst. If the destination
///     buffer is too small, this function does nothing and return -1.
C_BASE64_API long
b64_encode_into(char* dst, long dst_capacity, const char* src, long src_len);

C_BASE64_API char*
b64_encode(long* out_len, const char* src, long src_len);

/// Convert a base64 string to to raw bytes.
///
/// \param dst Destination buffer, where to write the byte representation.
/// \param dst_capacity
///     The maximum number of bytes that can be written in \p dst.
/// \param src Source buffer (base64 representation).
/// \param src_len Length of the source buffer.
///
/// \return
///     If this function succeeds, it returns the a positive number
///     representing the number of bytes written in \p dst. If the destination
///     buffer is too small, this function does nothing and return -1.
C_BASE64_API long
b64_decode_into(char* dst, long dst_capacity, const char* src, long src_len);

C_BASE64_API char*
b64_decode(long* out_len, const char* src, long src_len);

#ifdef __cplusplus
}
#endif

#endif // ifndef C_BASE64_H

#ifdef C_BASE64_IMPL

#ifndef C_BASE64_LOG_ERR
#define C_BASE64_LOG_ERR(...) struct __dummy_struct_for_semicolon__
#endif

#ifndef C_BASE64_ASSERT
#include <assert.h>
#define C_BASE64_ASSERT(prop) assert(prop)
#endif

#if !defined(C_BASE64_MALLOC) || !defined(C_BASE64_FREE)
#include <stdlib.h>
#endif

#ifndef C_BASE64_MALLOC
#define C_BASE64_MALLOC(size) malloc(size)
#endif

#ifndef C_BASE64_FREE
#define C_BASE64_FREE(ptr) free(ptr)
#endif

#define C_BASE64_CHECK_I(prop)                                                 \
    do {                                                                       \
        if (!(prop)) {                                                         \
            return -1;                                                         \
        }                                                                      \
    } while (0)

#define C_BASE64_CHECK_P(prop)                                                 \
    do {                                                                       \
        if (!(prop)) {                                                         \
            return NULL;                                                       \
        }                                                                      \
    } while (0)

/* clang-format off */
static const char b64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char b64_inv[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};
/* clang-format on */

typedef unsigned char u8;

static void
b64_encode_raw(char* dst, const char* src, long src_len)
{
    long remaining = src_len;
    const u8* u_src = (const u8*)src;
    while (remaining >= 3) {
        dst[0] = b64_table[u_src[0] >> 2];
        dst[1] = b64_table[((u_src[0] & 0x03) << 4) + (u_src[1] >> 4)];
        dst[2] = b64_table[((u_src[1] & 0x0f) << 2) + (u_src[2] >> 6)];
        dst[3] = b64_table[u_src[2] & 0x3f];
        remaining -= 3;
        u_src += 3;
        dst += 4;
    }
    if (remaining > 0) {
        dst[0] = b64_table[u_src[0] >> 2];
        if (remaining == 1) {
            dst[1] = b64_table[(u_src[0] & 0x03) << 4];
            dst[2] = '=';
            dst[3] = '=';
        } else { // remaining == 2
            dst[1] = b64_table[((u_src[0] & 0x03) << 4) + (u_src[1] >> 4)];
            dst[2] = b64_table[(u_src[1] & 0x0f) << 2];
            dst[3] = '=';
        }
    }
}

long
b64_encode_into(char* dst, long dst_capacity, const char* src, long src_len)
{
    C_BASE64_CHECK_I(dst != NULL);
    C_BASE64_CHECK_I(src != NULL);
    C_BASE64_CHECK_I(src_len >= 0);
    long dst_len = b64_repr_len(src_len);
    if (dst_len > dst_capacity) {
        C_BASE64_LOG_ERR(
            "dst_capacity is too small (%l, should be >= %l)",
            dst_capacity,
            dst_len);
        return -1;
    }
    b64_encode_raw(dst, src, src_len);
    return dst_len;
}

char*
b64_encode(long* out_len, const char* src, long src_len)
{
    C_BASE64_CHECK_P(src_len >= 0);
    // Add a trailing '\0' character.
    long b64_len = b64_repr_len(src_len) + 1;
    char* dst = (char*)C_BASE64_MALLOC(b64_len);
    if (dst == NULL) {
        return NULL;
    }
    long res = b64_encode_into(dst, b64_len, src, src_len);
    if (res == -1) {
        free(dst);
        return NULL;
    }
    dst[res] = 0;
    if (out_len) {
        *out_len = res;
    }
    return dst;
}

// Internal helper
static int
b64_inverse_block(u8* dst, size_t dst_len, const char* src)
{
    assert(dst_len <= 4);
    for (size_t i = 0; i < dst_len; i++) {
        char c = b64_inv[(u8)src[i]];
        if (c < 0) {
            return -1;
        }
        dst[i] = (u8)c;
    }
    return 0;
}

long
b64_decode_into(char* dst, long dst_capacity, const char* src, long src_len)
{
    C_BASE64_CHECK_I(dst != NULL);
    C_BASE64_CHECK_I(src != NULL);
    C_BASE64_CHECK_I(src_len >= 0);
    long dst_len_max = byte_repr_len(src_len);
    if (dst_len_max > dst_capacity) {
        C_BASE64_LOG_ERR(
            "dst_capacity is too small (%l, should be >= %l)",
            dst_capacity,
            dst_len);
        return -1;
    }

    long remaining = src_len;
    while (remaining > 4) {
        u8 isrc[4];
        if (b64_inverse_block(isrc, 4, src) == -1) {
            // error -> invalid base64 character.
            return -1;
        }

        dst[0] = (isrc[0] << 2) + (isrc[1] >> 4);
        dst[1] = (isrc[1] << 4) + (isrc[2] >> 2);
        dst[2] = (isrc[2] << 6) + isrc[3];

        dst += 3;
        src += 4;
        remaining -= 4;
    }

    // Ignore padding characters.
    long pad_len = 0;
    while (remaining > 2 && src[remaining - 1] == '=') {
        remaining -= 1;
        pad_len += 1;
    }

    // We zero-initalize the inverse (this is important).
    u8 isrc[4] = { 0 };
    if (b64_inverse_block(isrc, remaining, src) == -1) {
        // error -> invalid base64 character.
        return -1;
    }

    // We can have 2, 3 or 4 bytes remaining
    dst[0] = (isrc[0] << 2) + (isrc[1] >> 4);
    if (remaining > 2) {
        dst[1] = (isrc[1] << 4) + (isrc[2] >> 2);
    }
    if (remaining > 3) {
        dst[2] = (isrc[2] << 6) + isrc[3];
    }
    return dst_len_max - pad_len;
}

char*
b64_decode(long* out_len, const char* src, long src_len)
{
    long dst_len = byte_repr_len(src_len) + 1;
    char* dst = (char*)C_BASE64_MALLOC(dst_len);
    if (dst == NULL) {
        return NULL;
    }

    dst_len = b64_decode_into(dst, dst_len, src, src_len);
    if (dst_len == -1) {
        free(dst);
        return NULL;
    }

    dst[dst_len] = 0;
    if (out_len) {
        *out_len = dst_len;
    }
    return dst;
}
#endif // ifdef C_BASE64_IMPL
