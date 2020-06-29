#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(soft_bignumbers)
{
    /* addition */
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

    /* substraction */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_sub_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_sub_size_1);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_sub_size_1_with_carry);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_sub_size_2);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_sub_size_2_with_carry);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_sub_size_5);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_sub_size_5_with_carry);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_sub_size_5_with_carry_2);

    /* increment by one */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_inc_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_inc_size_1);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_inc_size_1_with_carry);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_inc_size_2);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_inc_size_2_with_carry);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_inc_size_5);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_inc_size_5_with_carry);

    /* test compare */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_compare_a_equals_b);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_compare_a_greater_than_b_lsb);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_compare_a_greater_than_b_msb);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_compare_a_lower_than_b_lsb);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_compare_a_lower_than_b_msb);
}
