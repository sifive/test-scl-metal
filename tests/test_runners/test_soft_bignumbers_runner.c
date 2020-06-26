#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(soft_bignumbers)
{
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_size_1);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_size_1_with_carry);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_size_2);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_size_2_with_carry);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_size_5);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_size_5_with_carry);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_summ_all_FF);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_carry_1);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_add_carry_2);
}
