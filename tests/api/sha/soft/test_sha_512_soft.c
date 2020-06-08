#include "unity.h"
#include "unity_fixture.h"

#include <api/scl_api.h>
#include <string.h>

#include <api/software/hash/soft_sha.h>

TEST_GROUP(soft_test_sha_512);

TEST_SETUP(soft_test_sha_512) {}

TEST_TEAR_DOWN(soft_test_sha_512) {}

TEST(soft_test_sha_512, msg_abc)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] = {
        0x61,
        0x62,
        0x63,
    };

    uint8_t digest[SHA512_BYTE_HASHSIZE];
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA512_BYTE_HASHSIZE] = {
        0xDD, 0xAF, 0x35, 0xA1, 0x93, 0x61, 0x7A, 0xBA, 0xCC, 0x41, 0x73,
        0x49, 0xAE, 0x20, 0x41, 0x31, 0x12, 0xE6, 0xFA, 0x4E, 0x89, 0xA9,
        0x7E, 0xA2, 0x0A, 0x9E, 0xEE, 0xE6, 0x4B, 0x55, 0xD3, 0x9A, 0x21,
        0x92, 0x99, 0x2A, 0x27, 0x4F, 0xC1, 0xA8, 0x36, 0xBA, 0x3C, 0x23,
        0xA3, 0xFE, 0xEB, 0xBD, 0x45, 0x4D, 0x44, 0x23, 0x64, 0x3C, 0xE8,
        0x0E, 0x2A, 0x9A, 0xC9, 0x4F, 0xA5, 0x4C, 0xA4, 0x9F};

    result = soft_sha_init(NULL, &ctx, SCL_HASH_SHA512, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = soft_sha_core(NULL, &ctx, message, sizeof(message));
    TEST_ASSERT_TRUE(0 == result);

    result = soft_sha_finish(NULL, &ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA512_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(0 ==
                     memcmp(expected_digest, digest, sizeof(expected_digest)));
}

TEST(soft_test_sha_512, msg_2_blocks)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] =
        "abcdefghbcdefghicdefghijdefghijkefghijklfghijklmghijklmnhijklmnoijklmn"
        "opjklmnopqklmnopqrlmnopqrsmnopqrstnopqrstu";

    uint8_t digest[SHA512_BYTE_HASHSIZE];
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA512_BYTE_HASHSIZE] = {
        0x8E, 0x95, 0x9B, 0x75, 0xDA, 0xE3, 0x13, 0xDA, 0x8C, 0xF4, 0xF7,
        0x28, 0x14, 0xFC, 0x14, 0x3F, 0x8F, 0x77, 0x79, 0xC6, 0xEB, 0x9F,
        0x7F, 0xA1, 0x72, 0x99, 0xAE, 0xAD, 0xB6, 0x88, 0x90, 0x18, 0x50,
        0x1D, 0x28, 0x9E, 0x49, 0x00, 0xF7, 0xE4, 0x33, 0x1B, 0x99, 0xDE,
        0xC4, 0xB5, 0x43, 0x3A, 0xC7, 0xD3, 0x29, 0xEE, 0xB6, 0xDD, 0x26,
        0x54, 0x5E, 0x96, 0xE5, 0x5B, 0x87, 0x4B, 0xE9, 0x09};

    result = soft_sha_init(NULL, &ctx, SCL_HASH_SHA512, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = soft_sha_core(NULL, &ctx, message, sizeof(message) - 1);
    TEST_ASSERT_TRUE(0 == result);

    result = soft_sha_finish(NULL, &ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA512_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(0 ==
                     memcmp(expected_digest, digest, sizeof(expected_digest)));
}
