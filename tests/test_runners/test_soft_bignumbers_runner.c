#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(soft_bignumbers)
{
    RUN_TEST_CASE(soft_bignumbers, bignum_memcpy_success);
    RUN_TEST_CASE(soft_bignumbers, bignum_memset_success);
    RUN_TEST_CASE(soft_bignumbers, bignum_memcmp_a_equals_b);
    RUN_TEST_CASE(soft_bignumbers, bignum_memcmp_a_greater_than_b_lsb);
    RUN_TEST_CASE(soft_bignumbers, bignum_memcmp_a_greater_than_b_msb);
    RUN_TEST_CASE(soft_bignumbers, bignum_memcmp_a_lower_than_b_lsb);
    RUN_TEST_CASE(soft_bignumbers, bignum_memcmp_a_lower_than_b_msb);
}
