#include "unity.h"
#include "unity_fixture.h"

// SHA 256
TEST_GROUP_RUNNER(test_sha_256_soft)
{
    RUN_TEST_CASE(test_sha_256_soft, msg_abc);
    RUN_TEST_CASE(test_sha_256_soft, msg_2_blocks);
}

// SHA 224
TEST_GROUP_RUNNER(test_sha_224_soft)
{
    RUN_TEST_CASE(test_sha_224_soft, msg_abc);
    RUN_TEST_CASE(test_sha_224_soft, msg_2_blocks);
}

// SHA 384
TEST_GROUP_RUNNER(test_sha_384_soft)
{
    RUN_TEST_CASE(test_sha_384_soft, msg_abc);
    RUN_TEST_CASE(test_sha_384_soft, msg_2_blocks);
}


// SHA 512
TEST_GROUP_RUNNER(test_sha_512_soft)
{
    RUN_TEST_CASE(test_sha_512_soft, msg_abc);
    RUN_TEST_CASE(test_sha_512_soft, msg_2_blocks);
}

