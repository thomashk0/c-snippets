#define C_BASE64_IMPL
#include "../base64.h"
#include "utest.h"

#define scoped_ptr __attribute__((cleanup(cleanup_free)))

static void cleanup_free(void* p) { free(*(void**)p); }

UTEST(base64, b64_encode_into) {
    const char* s = "hello";
    long s_len = strlen(s);
    char dst[256] = {0};

    // Destination pointer is NULL
    ASSERT_EQ(b64_encode_into(NULL, 1000, s, s_len), -1);
    // Destination size too small
    ASSERT_EQ(b64_encode_into(dst, 5, s, s_len), -1);
    // Source pointer is invalid
    ASSERT_EQ(b64_encode_into(dst, 256, NULL, s_len), -1);

    // Empty string
    ASSERT_EQ(b64_encode_into(dst, 256, s, 0), 0);

    ASSERT_EQ(b64_encode_into(dst, 256, s, s_len), 8);
    ASSERT_STREQ(dst, "aGVsbG8=");
}

UTEST(base64, b64_encode) {
    const char* s = "_1028+**//";
    long s_len = strlen(s);
    long p = 0;

    // Negative size.
    ASSERT_TRUE(b64_encode(NULL, s, -1) == NULL);
    ASSERT_TRUE(b64_encode(&p, s, -1) == NULL);
    ASSERT_EQ(p, 0);

    {
        scoped_ptr char* s_b64 = b64_encode(&p, s, 0);
        ASSERT_TRUE(s_b64 != NULL);
        ASSERT_STREQ(s_b64, "");
        // FIXME: should be 0. => number of actual data bytes.
        ASSERT_EQ(p, 1);
    }

    {
        scoped_ptr char* s_b64 = b64_encode(&p, s, s_len);
        ASSERT_TRUE(s_b64 != NULL);
        ASSERT_EQ(p, 17);
        ASSERT_STREQ(s_b64, "XzEwMjgrKiovLw==");
    }
}

UTEST(base64, b64_decode_into) {
    // "example"
    const char* s = "ZXhhbXBsZQ==";
    long s_len = strlen(s);
    char dst[256] = {0};

    // Destination pointer is NULL
    ASSERT_EQ(b64_decode_into(NULL, 1000, s, s_len), -1);
    // Destination size too small
    ASSERT_EQ(b64_decode_into(dst, 5, s, s_len), -1);
    ASSERT_EQ(b64_decode_into(dst, 6, s, s_len), -1);
    ASSERT_EQ(b64_decode_into(dst, 7, s, s_len), -1);

    // Source pointer is invalid
    ASSERT_EQ(b64_decode_into(dst, 256, NULL, s_len), -1);

    // Empty string
    ASSERT_EQ(b64_decode_into(dst, 256, s, 0), 0);

    ASSERT_EQ(b64_decode_into(dst, 256, s, s_len), 7);
    ASSERT_STREQ(dst, "example");
}

UTEST(base64, b64_decode) 
{
    const char* s = "ZXhhbXBsZQ==";
    long s_len = strlen(s);

    long len = 0;
    // Negative size.
    ASSERT_TRUE(b64_decode(NULL, s, -1) == NULL);
    ASSERT_TRUE(b64_decode(&len, s, -1) == NULL);
    ASSERT_EQ(len, 0);

    {
        scoped_ptr char* p = b64_decode(&len, s, 0);
        ASSERT_TRUE(p != NULL);
        ASSERT_STREQ(p, "");
        ASSERT_EQ(len, 0);
    }

    {
        scoped_ptr char* s_origin = b64_decode(&len, s, s_len);
        ASSERT_TRUE(s_origin != NULL);
        ASSERT_EQ(len, 7);
        ASSERT_STREQ(s_origin, "example");
    }

}

// static void check_base64(const char* p, const char* expected)
// {
//     scoped_ptr char* p_b64 = b64_encode(NULL, p, strlen(p));
//     ASSERT_STREQ(p_b64, expected);
//     scoped_ptr char* p_origin = b64_decode(NULL, p_b64, strlen(p_b64));
// }
// 
// UTEST(base64, b64_test_vectors) 
// {
// }

UTEST_MAIN()
