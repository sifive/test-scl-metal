#include "unity.h"
#include "unity_fixture.h"

// AES 128
TEST_GROUP_RUNNER(scl_aes_128)
{
    RUN_TEST_CASE(scl_aes_128, ecb_F_1_12);
    RUN_TEST_CASE(scl_aes_128, ecb_F_1_12_high);
    RUN_TEST_CASE(scl_aes_128, ecb_not_aligned);
    RUN_TEST_CASE(scl_aes_128, cbc_F_2_12);
    RUN_TEST_CASE(scl_aes_128, cbc_F_2_12_high);
}

// AES 192
TEST_GROUP_RUNNER(scl_aes_192)
{
    RUN_TEST_CASE(scl_aes_192, ecb_F_1_34);
    RUN_TEST_CASE(scl_aes_192, ecb_F_1_34_high);
    RUN_TEST_CASE(scl_aes_192, cbc_F_2_34);
    RUN_TEST_CASE(scl_aes_192, cbc_F_2_34_high);
}

// AES 256
TEST_GROUP_RUNNER(scl_aes_256)
{
    RUN_TEST_CASE(scl_aes_256, ecb_F_1_56);
    RUN_TEST_CASE(scl_aes_256, ecb_F_1_56_high);
    RUN_TEST_CASE(scl_aes_256, cbc_F_2_56);
    RUN_TEST_CASE(scl_aes_256, cbc_F_2_56_high);
}
