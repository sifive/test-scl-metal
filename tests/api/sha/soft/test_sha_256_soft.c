#include "unity.h"
#include "unity_fixture.h"

#include <string.h>

#include <api/soft/hash/soft_sha256.h>

TEST_GROUP(test_sha_256_soft);

TEST_SETUP(test_sha_256_soft)
{
}

TEST_TEAR_DOWN(test_sha_256_soft)
{
}

TEST(test_sha_256_soft, msg_abc) {
    int32_t result = 0;
    sha256_ctx_t ctx;

    const uint8_t message[] = {
        0x61, 0x62, 0x63,
    };

    uint8_t digest[SHA256_BYTE_HASHSIZE];
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA256_BYTE_HASHSIZE] = {
        0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA,
        0x41, 0x41, 0x40, 0xDE, 0x5D, 0xAE, 0x22, 0x23,
        0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17, 0x7A, 0x9C,
        0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD
    };

    result = sha256_init_soft(&ctx, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = sha256_core_soft(&ctx, message, sizeof(message));
    TEST_ASSERT_TRUE(0 == result);

    result = sha256_finish_soft(&ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA256_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(0 == memcmp(expected_digest, digest, sizeof(expected_digest)));
}

TEST(test_sha_256_soft, msg_2_blocks) {
    int32_t result = 0;
    sha256_ctx_t ctx;

    const uint8_t message[] = "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

    uint8_t digest[SHA256_BYTE_HASHSIZE];
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA256_BYTE_HASHSIZE] = { 
        0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8,
        0xE5, 0xC0, 0x26, 0x93, 0x0C, 0x3E, 0x60, 0x39,
        0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF, 0x21, 0x67,
        0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1
    };

    result = sha256_init_soft(&ctx, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = sha256_core_soft(&ctx, message, sizeof(message) - 1);
    TEST_ASSERT_TRUE(0 == result);

    result = sha256_finish_soft(&ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);

    TEST_ASSERT_TRUE(SHA256_BYTE_HASHSIZE == digest_len);

    TEST_ASSERT_TRUE(0 == memcmp(expected_digest, digest, sizeof(expected_digest)));
}
