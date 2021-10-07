#define C_BASE64_IMPL
#include "../base64.h"
#include "utest.h"

#define COUNTOF(array) (sizeof(array) / sizeof(array[0]))

#define scoped_ptr __attribute__((cleanup(cleanup_free)))

static void
cleanup_free(void* p)
{
    free(*(void**)p);
}

UTEST(base64, b64_encode_into)
{
    const char* s = "hello";
    long s_len = strlen(s);
    char dst[256] = { 0 };

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

UTEST(base64, b64_encode)
{
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
        ASSERT_EQ(p, 0);
    }

    {
        scoped_ptr char* s_b64 = b64_encode(&p, s, s_len);
        ASSERT_TRUE(s_b64 != NULL);
        ASSERT_EQ(p, 16);
        ASSERT_STREQ(s_b64, "XzEwMjgrKiovLw==");
    }
}

UTEST(base64, b64_decode_into)
{
    // "example"
    const char* s = "ZXhhbXBsZQ==";
    long s_len = strlen(s);
    char dst[256] = { 0 };

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

UTEST(base64, b64_decode_invalid)
{
    const char* invalid_inputs[] = {
        ".", "a-", "ab+", "ab=12", "ab__12", "&>b__12", "&<>b__12",
    };
    for (int i = 0; i < (int)COUNTOF(invalid_inputs); i++) {
        const char* s = invalid_inputs[i];
        printf("testing: '%s'\n", s);
        ASSERT_TRUE(b64_decode(NULL, s, strlen(s)) == NULL);
    }
}

static int
check_base64(const char* p, const char* expected)
{
    scoped_ptr char* p_b64 = b64_encode(NULL, p, strlen(p));
    if (strcmp(expected, p_b64) != 0) {
        return 1;
    }
    scoped_ptr char* p_origin = b64_decode(NULL, p_b64, strlen(p_b64));
    if (strcmp(p_origin, p) != 0) {
        return 2;
    }
    return 0;
}

UTEST(base64, b64_test_vectors)
{
    // Generated with gen-testcases.py
    /* clang-format off */
    ASSERT_EQ(check_base64("\nt\xbf", "CnS/"), 0);
    ASSERT_EQ(check_base64("8", "OAE="), 0);
    ASSERT_EQ(check_base64("\xbaTEm\xd3", "ulRFbdM="), 0);
    ASSERT_EQ(check_base64("\xf3\x8dcp", "840YY3A="), 0);
    ASSERT_EQ(check_base64("mf\xc4\x9dg\xc8", "bWbEnRYGZxHI"), 0);
    ASSERT_EQ(check_base64(".'\xc0", "FS4PJ8AW"), 0);
    ASSERT_EQ(check_base64("\x8d\xcc\xcc", "jcwCzA=="), 0);
    ASSERT_EQ(check_base64("\xbbWxO", "u1d4Tx8="), 0);
    ASSERT_EQ(check_base64("\xfd", "/R4="), 0);
    ASSERT_EQ(check_base64("p\x92\x99\xdd\xfb\xd6kc\xb4", "cB+Smd371mtjtA=="), 0);
    ASSERT_EQ(check_base64("\x99-yN@g", "mS15TkBn"), 0);
    ASSERT_EQ(check_base64("RwW\xed", "UndX7Q=="), 0);
    ASSERT_EQ(check_base64("\xe7\x95\xe3\xe0\xc3", "55UPDOMe4MM="), 0);
    ASSERT_EQ(check_base64("\xde\xae\xe9v", "3gECrukcdg=="), 0);
    ASSERT_EQ(check_base64("|G\xf1 \x83\xff]", "fEfxIIP/XQ=="), 0);
    ASSERT_EQ(check_base64("gY\xeb\xc0Y\xb2\xa4-\x92hN", "HWdZ68BZsqQtkmhO"), 0);
    ASSERT_EQ(check_base64("lT:9\x85^<3~", "bFQ6OYVePDN+"), 0);
    ASSERT_EQ(check_base64("&\xf8\x8e{J", "JviOe0o="), 0);
    ASSERT_EQ(check_base64("v\xda\x97\xacHU\xca\xec\xc4", "GHbal6wMSFXK7AfE"), 0);
    ASSERT_EQ(check_base64("h\xfb", "aPs="), 0);
    ASSERT_EQ(check_base64("\xa7", "pw=="), 0);
    ASSERT_EQ(check_base64("\x8c\xc0\xe0\xef\xb6F0\x99\xa6", "jMDg77YYRjAQmaY="), 0);
    ASSERT_EQ(check_base64("w", "dw=="), 0);
    ASSERT_EQ(check_base64("\xf8\x8c\xb8\x9f\x83\x8fw\xf2\x80\xf6", "BviMuJ+Dj3cY8oD2"), 0);
    ASSERT_EQ(check_base64("\x9c", "nA=="), 0);
    ASSERT_EQ(check_base64("\x8d0\xb5\xc4?E\xc7I\xcbu", "jRQwtcQ/RcdJy3U="), 0);
    ASSERT_EQ(check_base64("\xbe\xc3\x98\xffO", "vsOY/08="), 0);
    ASSERT_EQ(check_base64("\xfe\xf8yn\xa0ks\xd9", "/vh5bqBrc9k="), 0);
    ASSERT_EQ(check_base64("Y!\xda=;\x90", "WSHaPQ47kA4="), 0);
    ASSERT_EQ(check_base64(",\xc6", "LMY="), 0);
    ASSERT_EQ(check_base64("\xb4\xde\xe7\xcb\x85\xfe", "tN7ny4X+"), 0);
    ASSERT_EQ(check_base64("(K", "KEs="), 0);
    ASSERT_EQ(check_base64("eP\xeaR\xad=z&X", "ZVDqUq09eiZY"), 0);
    ASSERT_EQ(check_base64("[\x82'%M", "W4InJU0="), 0);
    ASSERT_EQ(check_base64("L0N(\xfc\x94\xc8o\x85\xea&", "TDBOKPyUyG+F6iY="), 0);
    ASSERT_EQ(check_base64("Q\xe6!", "UeYh"), 0);
    /* clang-format on */
}

UTEST_MAIN()
