#include "unity.h"
#include "unity_fixture.h"

// SHA 224
TEST_GROUP_RUNNER(test_sha_224_scl)
{
    RUN_TEST_CASE(test_sha_224_scl, msg_abc);
    RUN_TEST_CASE(test_sha_224_scl, msg_2_blocks);
}

// SHA 256
TEST_GROUP_RUNNER(test_sha_256_scl)
{
    RUN_TEST_CASE(test_sha_256_scl, msg_abc);
    RUN_TEST_CASE(test_sha_256_scl, msg_2_blocks);
}

// SHA 384
TEST_GROUP_RUNNER(test_sha_384_scl)
{
    RUN_TEST_CASE(test_sha_384_scl, msg_abc);
    RUN_TEST_CASE(test_sha_384_scl, msg_2_blocks);
}

// SHA 512
TEST_GROUP_RUNNER(test_sha_512_scl)
{
    RUN_TEST_CASE(test_sha_512_scl, msg_abc);
    RUN_TEST_CASE(test_sha_512_scl, msg_2_blocks);
}