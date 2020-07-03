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

    /* multiplication */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_mult_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_mult_size_1);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_mult_size_2);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_mult_size_5);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_mult_size_5_zero);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_mult_size_5_identity);

    /* Right shift */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_1);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_2);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_5);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_7);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_5_shift_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_5_shift_159);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_5_shift_160);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_5_in_NULL);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_5_out_NULL);

    /* Left Shift */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_1);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_2);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_5);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_7);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_5_shift_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_5_shift_159);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_5_shift_160);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_5_in_NULL);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_5_out_NULL);
}
