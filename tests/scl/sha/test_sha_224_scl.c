#include "unity.h"
#include "unity_fixture.h"

#include <string.h>

#include <api/scl_api.h>
#include <scl/scl_sha.h>

#include <scl/scl_init.h>

#include <api/software/hash/soft_sha.h>

static const metal_scl_t scl = {.hca_base = 0x20000,
                                .hash_func = {
                                    .sha_init = soft_sha_init,
                                    .sha_core = soft_sha_core,
                                    .sha_finish = soft_sha_finish,
                                }};

TEST_GROUP(scl_test_sha_224);

TEST_SETUP(scl_test_sha_224) { (void)scl_init(&scl); }

TEST_TEAR_DOWN(scl_test_sha_224) {}

TEST(scl_test_sha_224, msg_abc)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] = {
        0x61,
        0x62,
        0x63,
    };

    uint8_t digest[SHA224_BYTE_HASHSIZE];
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA224_BYTE_HASHSIZE] = {
        0x23, 0x09, 0x7D, 0x22, 0x34, 0x05, 0xD8, 0x22, 0x86, 0x42,
        0xA4, 0x77, 0xBD, 0xA2, 0x55, 0xB3, 0x2A, 0xAD, 0xBC, 0xE4,
        0xBD, 0xA0, 0xB3, 0xF7, 0xE3, 0x6C, 0x9D, 0xA7};

    result =
        scl_sha(SCL_HASH_SHA224, message, sizeof(message), digest, &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA224_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(0 ==
                     memcmp(expected_digest, digest, sizeof(expected_digest)));
}

TEST(scl_test_sha_224, msg_2_blocks)
{
    int32_t result = 0;
    sha_ctx_t ctx;

    const uint8_t message[] =
        "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";

    uint8_t digest[SHA224_BYTE_HASHSIZE];
    size_t digest_len = sizeof(digest);

    const uint8_t expected_digest[SHA224_BYTE_HASHSIZE] = {
        0x75, 0x38, 0x8B, 0x16, 0x51, 0x27, 0x76, 0xCC, 0x5D, 0xBA,
        0x5D, 0xA1, 0xFD, 0x89, 0x01, 0x50, 0xB0, 0xC6, 0x45, 0x5C,
        0xB4, 0xF5, 0x8B, 0x19, 0x52, 0x52, 0x25, 0x25};

    result = scl_sha(SCL_HASH_SHA224, message, sizeof(message) - 1, digest,
                     &digest_len);
    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_TRUE(SHA224_BYTE_HASHSIZE == digest_len);
    TEST_ASSERT_TRUE(0 ==
                     memcmp(expected_digest, digest, sizeof(expected_digest)));
}
