#include "unity.h"
#include "unity_fixture.h"

// SHA 224
TEST_GROUP_RUNNER(soft_test_sha_224)
{
    RUN_TEST_CASE(soft_test_sha_224, msg_abc_all_aligned);
    RUN_TEST_CASE(soft_test_sha_224, msg_2_blocks_all_aligned);
    RUN_TEST_CASE(soft_test_sha_224, msg_abc_msg_not_aligned);
    RUN_TEST_CASE(soft_test_sha_224, msg_2_blocks_msg_not_aligned);
    RUN_TEST_CASE(soft_test_sha_224, msg_abc_digest_not_aligned);
    RUN_TEST_CASE(soft_test_sha_224, msg_2_blocks_digest_not_aligned);
    RUN_TEST_CASE(soft_test_sha_224, msg_1024_bytes_aligned);
    RUN_TEST_CASE(soft_test_sha_224, msg_1024_bytes_not_aligned);
}

// SHA 256
TEST_GROUP_RUNNER(soft_test_sha_256)
{
    RUN_TEST_CASE(soft_test_sha_256, msg_abc_all_aligned);
    RUN_TEST_CASE(soft_test_sha_256, msg_2_blocks_all_aligned);
    RUN_TEST_CASE(soft_test_sha_256, msg_abc_msg_not_aligned);
    RUN_TEST_CASE(soft_test_sha_256, msg_2_blocks_msg_not_aligned);
    RUN_TEST_CASE(soft_test_sha_256, msg_abc_digest_not_aligned);
    RUN_TEST_CASE(soft_test_sha_256, msg_2_blocks_digest_not_aligned);
    RUN_TEST_CASE(soft_test_sha_256, msg_1024_bytes_aligned);
    RUN_TEST_CASE(soft_test_sha_256, msg_1024_bytes_not_aligned);
}

// SHA 384
TEST_GROUP_RUNNER(soft_test_sha_384)
{
    RUN_TEST_CASE(soft_test_sha_384, msg_abc_all_aligned);
    RUN_TEST_CASE(soft_test_sha_384, msg_2_blocks_all_aligned);
    RUN_TEST_CASE(soft_test_sha_384, msg_abc_msg_not_aligned);
    RUN_TEST_CASE(soft_test_sha_384, msg_2_blocks_msg_not_aligned);
    RUN_TEST_CASE(soft_test_sha_384, msg_abc_digest_not_aligned);
    RUN_TEST_CASE(soft_test_sha_384, msg_2_blocks_digest_not_aligned);
    RUN_TEST_CASE(soft_test_sha_384, msg_1024_bytes_aligned);
    RUN_TEST_CASE(soft_test_sha_384, msg_1024_bytes_not_aligned);
}

// SHA 512
TEST_GROUP_RUNNER(soft_test_sha_512)
{
    RUN_TEST_CASE(soft_test_sha_512, msg_abc_all_aligned);
    RUN_TEST_CASE(soft_test_sha_512, msg_2_blocks_all_aligned);
    RUN_TEST_CASE(soft_test_sha_512, msg_abc_msg_not_aligned);
    RUN_TEST_CASE(soft_test_sha_512, msg_2_blocks_msg_not_aligned);
    RUN_TEST_CASE(soft_test_sha_512, msg_abc_digest_not_aligned);
    RUN_TEST_CASE(soft_test_sha_512, msg_2_blocks_digest_not_aligned);
    RUN_TEST_CASE(soft_test_sha_512, msg_1024_bytes_aligned);
    RUN_TEST_CASE(soft_test_sha_512, msg_1024_bytes_not_aligned);
}
