#include "unity.h"
#include "unity_fixture.h"

// AES 128
TEST_GROUP_RUNNER(hca_aes_128)
{
    RUN_TEST_CASE(hca_aes_128, ecb_F_1_12);
    RUN_TEST_CASE(hca_aes_128, ecb_not_aligned);
    RUN_TEST_CASE(hca_aes_128, cbc_F_2_12);
}

// AES 192
TEST_GROUP_RUNNER(hca_aes_192)
{
    RUN_TEST_CASE(hca_aes_192, ecb_F_1_34);
    RUN_TEST_CASE(hca_aes_192, cbc_F_2_34);
}

// AES 256
TEST_GROUP_RUNNER(hca_aes_256)
{
    RUN_TEST_CASE(hca_aes_256, ecb_F_1_56);
    RUN_TEST_CASE(hca_aes_256, cbc_F_2_56);
}
