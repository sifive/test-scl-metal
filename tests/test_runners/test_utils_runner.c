#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(utils)
{
    RUN_TEST_CASE(utils, memcpy_u64_success);
    RUN_TEST_CASE(utils, memset_u64_success);
    RUN_TEST_CASE(utils, memcmp_u64_a_equals_b);
    RUN_TEST_CASE(utils, memcmp_u64_a_greater_than_b_lsb);
    RUN_TEST_CASE(utils, memcmp_u64_a_greater_than_b_msb);
    RUN_TEST_CASE(utils, memcmp_u64_a_lower_than_b_lsb);
    RUN_TEST_CASE(utils, memcmp_u64_a_lower_than_b_msb);
}
