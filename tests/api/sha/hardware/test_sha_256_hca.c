#include "unity.h"
#include "unity_fixture.h"

#include <string.h>

#include <api/hardware/scl_hca.h>
#include <api/scl_api.h>

static const struct __metal_scl scl = {
    .hca_base = 0x20000,
};

TEST_GROUP(hca_test_sha_256);

TEST_SETUP(hca_test_sha_256) {}

TEST_TEAR_DOWN(hca_test_sha_256) {}

TEST(hca_test_sha_256, msg_abc_all_aligned)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] __attribute__((aligned(8))) = {
        0x61,
        0x62,
        0x63,
    };

    uint8_t digest[SHA256_BYTE_HASHSIZE] __attribute__((aligned(8)));
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA256_BYTE_HASHSIZE] = {
        0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40,
        0xDE, 0x5D, 0xAE, 0x22, 0x23, 0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17,
        0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD};

    result = hca_sha_init(&scl, &ctx, SCL_HASH_SHA256, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_core(&scl, &ctx, message, sizeof(message));
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_finish(&scl, &ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA256_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(0 ==
                     memcmp(expected_digest, digest, sizeof(expected_digest)));
}

TEST(hca_test_sha_256, msg_2_blocks_all_aligned)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] __attribute__((aligned(8))) =
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

    uint8_t digest[SHA256_BYTE_HASHSIZE] __attribute__((aligned(8)));
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA256_BYTE_HASHSIZE] = {
        0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26,
        0x93, 0x0C, 0x3E, 0x60, 0x39, 0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF,
        0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1};

    result = hca_sha_init(&scl, &ctx, SCL_HASH_SHA256, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_core(&scl, &ctx, message, sizeof(message) - 1);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_finish(&scl, &ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA256_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(0 ==
                     memcmp(expected_digest, digest, sizeof(expected_digest)));
}

TEST(hca_test_sha_256, msg_abc_msg_not_aligned)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] __attribute__((aligned(8))) = {
        0x00,
        0x61,
        0x62,
        0x63,
    };

    uint8_t digest[SHA256_BYTE_HASHSIZE] __attribute__((aligned(8)));
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA256_BYTE_HASHSIZE] = {
        0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40,
        0xDE, 0x5D, 0xAE, 0x22, 0x23, 0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17,
        0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD};

    result = hca_sha_init(&scl, &ctx, SCL_HASH_SHA256, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_core(&scl, &ctx, &message[1], sizeof(message) - 1);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_finish(&scl, &ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA256_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(0 ==
                     memcmp(expected_digest, digest, sizeof(expected_digest)));
}

TEST(hca_test_sha_256, msg_2_blocks_msg_not_aligned)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] __attribute__((aligned(8))) =
        "aabcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

    uint8_t digest[SHA256_BYTE_HASHSIZE] __attribute__((aligned(8)));
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA256_BYTE_HASHSIZE] = {
        0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26,
        0x93, 0x0C, 0x3E, 0x60, 0x39, 0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF,
        0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1};

    result = hca_sha_init(&scl, &ctx, SCL_HASH_SHA256, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_core(&scl, &ctx, &message[1], sizeof(message) - 2);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_finish(&scl, &ctx, digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA256_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(0 ==
                     memcmp(expected_digest, digest, sizeof(expected_digest)));
}

TEST(hca_test_sha_256, msg_abc_digest_not_aligned)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] __attribute__((aligned(8))) = {
        0x61,
        0x62,
        0x63,
    };

    uint8_t digest[SHA256_BYTE_HASHSIZE + 1] __attribute__((aligned(8)));
    size_t digest_len = sizeof(digest) - 1;

    const uint8_t expected_digest[SHA256_BYTE_HASHSIZE] = {
        0xBA, 0x78, 0x16, 0xBF, 0x8F, 0x01, 0xCF, 0xEA, 0x41, 0x41, 0x40,
        0xDE, 0x5D, 0xAE, 0x22, 0x23, 0xB0, 0x03, 0x61, 0xA3, 0x96, 0x17,
        0x7A, 0x9C, 0xB4, 0x10, 0xFF, 0x61, 0xF2, 0x00, 0x15, 0xAD};

    result = hca_sha_init(&scl, &ctx, SCL_HASH_SHA256, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_core(&scl, &ctx, message, sizeof(message));
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_finish(&scl, &ctx, &digest[1], &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA256_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(
        0 == memcmp(expected_digest, &digest[1], sizeof(expected_digest)));
}

TEST(hca_test_sha_256, msg_2_blocks_digest_not_aligned)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] __attribute__((aligned(8))) =
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

    uint8_t digest[SHA256_BYTE_HASHSIZE + 1] __attribute__((aligned(8)));
    size_t digest_len = sizeof(digest) - 1;

    const uint8_t expected_digest[SHA256_BYTE_HASHSIZE] = {
        0x24, 0x8D, 0x6A, 0x61, 0xD2, 0x06, 0x38, 0xB8, 0xE5, 0xC0, 0x26,
        0x93, 0x0C, 0x3E, 0x60, 0x39, 0xA3, 0x3C, 0xE4, 0x59, 0x64, 0xFF,
        0x21, 0x67, 0xF6, 0xEC, 0xED, 0xD4, 0x19, 0xDB, 0x06, 0xC1};

    result = hca_sha_init(&scl, &ctx, SCL_HASH_SHA256, SCL_BIG_ENDIAN_MODE);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_core(&scl, &ctx, message, sizeof(message) - 1);
    TEST_ASSERT_TRUE(0 == result);

    result = hca_sha_finish(&scl, &ctx, &digest[1], &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA256_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(
        0 == memcmp(expected_digest, &digest[1], sizeof(expected_digest)));
}
