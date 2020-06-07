#include "unity.h"
#include "unity_fixture.h"

// SHA 224
TEST_GROUP_RUNNER(test_sha_224_hca)
{
    RUN_TEST_CASE(test_sha_224_hca, msg_abc);
    RUN_TEST_CASE(test_sha_224_hca, msg_2_blocks);
}

// SHA 256
TEST_GROUP_RUNNER(test_sha_256_hca)
{
    RUN_TEST_CASE(test_sha_256_hca, msg_abc);
    RUN_TEST_CASE(test_sha_256_hca, msg_2_blocks);
}

// SHA 384
TEST_GROUP_RUNNER(test_sha_384_hca)
{
    RUN_TEST_CASE(test_sha_384_hca, msg_abc);
    RUN_TEST_CASE(test_sha_384_hca, msg_2_blocks);
}

// SHA 512
TEST_GROUP_RUNNER(test_sha_512_hca)
{
    RUN_TEST_CASE(test_sha_512_hca, msg_abc);
    RUN_TEST_CASE(test_sha_512_hca, msg_2_blocks);
}
