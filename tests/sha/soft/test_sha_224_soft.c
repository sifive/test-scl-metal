#include "unity.h"
#include "unity_fixture.h"

#include <string.h>

#include <api/soft/hash/soft_sha224.h>


TEST_GROUP(test_sha_224_soft);

TEST_SETUP(test_sha_224_soft)
{
}

TEST_TEAR_DOWN(test_sha_224_soft)
{
}

TEST(test_sha_224_soft, msg_abc) {
    int32_t result = 0;
    sha256_ctx_t ctx;

    const uint8_t message[] = {
        0x61, 0x62, 0x63,
    };

    uint8_t digest[SHA224_BYTE_HASHSIZE];
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA224_BYTE_HASHSIZE] = {  
        0x23, 0x09, 0x7D, 0x22, 0x34, 0x05, 0xD8, 0x22,
        0x86, 0x42, 0xA4, 0x77, 0xBD, 0xA2, 0x55, 0xB3,
        0x2A, 0xAD, 0xBC, 0xE4, 0xBD, 0xA0, 0xB3, 0xF7,
        0xE3, 0x6C, 0x9D, 0xA7
    };

    result = sha224_init_soft(&ctx, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = sha224_core_soft(&ctx, message, sizeof(message));
    TEST_ASSERT_TRUE(0 == result);

    result = sha224_finish_soft(&ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);

    TEST_ASSERT_TRUE(SHA224_BYTE_HASHSIZE == digest_len);

    TEST_ASSERT_TRUE(0 == memcmp(expected_digest, digest, sizeof(expected_digest)));
}

TEST(test_sha_224_soft, msg_2_blocks) {
    int32_t result = 0;
    sha256_ctx_t ctx;

    const uint8_t message[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

    uint8_t digest[SHA224_BYTE_HASHSIZE];
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA224_BYTE_HASHSIZE] = {  
        0x75, 0x38, 0x8B, 0x16, 0x51, 0x27, 0x76, 0xCC,
        0x5D, 0xBA, 0x5D, 0xA1, 0xFD, 0x89, 0x01, 0x50,
        0xB0, 0xC6, 0x45, 0x5C, 0xB4, 0xF5, 0x8B, 0x19,
        0x52, 0x52, 0x25, 0x25
    };

    result = sha224_init_soft(&ctx, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = sha224_core_soft(&ctx, message, sizeof(message) - 1);
    TEST_ASSERT_TRUE(0 == result);

    result = sha224_finish_soft(&ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);

    TEST_ASSERT_TRUE(SHA224_BYTE_HASHSIZE == digest_len);

    TEST_ASSERT_TRUE(0 == memcmp(expected_digest, digest, sizeof(expected_digest)));
}
