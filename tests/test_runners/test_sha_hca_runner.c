#include "unity.h"
#include "unity_fixture.h"

// SHA 224
TEST_GROUP_RUNNER(hca_test_sha_224)
{
    RUN_TEST_CASE(hca_test_sha_224, msg_abc);
    RUN_TEST_CASE(hca_test_sha_224, msg_2_blocks);
}

// SHA 256
TEST_GROUP_RUNNER(hca_test_sha_256)
{
    RUN_TEST_CASE(hca_test_sha_256, msg_abc);
    RUN_TEST_CASE(hca_test_sha_256, msg_2_blocks);
}

// SHA 384
TEST_GROUP_RUNNER(hca_test_sha_384)
{
    RUN_TEST_CASE(hca_test_sha_384, msg_abc);
    RUN_TEST_CASE(hca_test_sha_384, msg_2_blocks);
}

// SHA 512
TEST_GROUP_RUNNER(hca_test_sha_512)
{
    RUN_TEST_CASE(hca_test_sha_512, msg_abc);
    RUN_TEST_CASE(hca_test_sha_512, msg_2_blocks);
}
