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
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_rightshift_size_7_shift_59);

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
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_leftshift_size_7_shift_59);

    /* get non zero 32b words */
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_nb_non_zero_32b_word_array_null_ptr);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_nb_non_zero_32b_word_size_0x80000000);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_nb_non_zero_32b_word_size_0);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_nb_non_zero_32b_word_size_2_expect_1);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_nb_non_zero_32b_word_size_2_expect_2);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_nb_non_zero_32b_word_size_6_expect_1);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_nb_non_zero_32b_word_size_6_expect_5);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_nb_non_zero_32b_word_size_6_expect_6);

    /* test on msb set in word */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_msb_set_in_word_32b_word);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_msb_set_in_word_64b_word_last_bit_set);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_msb_set_in_word_64b_word_first_bit_set);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_msb_set_in_word_32b_word_none_set);

    /* check bignumber null */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_is_null_size_6_lsb_set);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_is_null_size_6_msb_set);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_is_null_size_5_msb_set);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_is_null_size_6_null);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_is_null_size_5_null);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_is_null_array_nullptr);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_is_null_size_0);

    /* Get MSB set in bignumber */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_get_msb_set_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_get_msb_set_nullptr);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_get_msb_set_size_5_expect_1);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_get_msb_set_size_5_expect_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_get_msb_set_size_5_expect_159);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_get_msb_set_size_5_124);

    /* set one bit in a big integer */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_set_bit_null_ptr);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_set_bit_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_set_bit_size_5_set_first);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_set_bit_size_5_set_last);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_set_bit_size_5_set_out_of_range);

    /* Compare with different length */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_compare_len_diff_a_equals_b);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_compare_len_diff_a_greater_than_b_lsb);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_compare_len_diff_a_greater_than_b_msb);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_compare_len_diff_a_lower_than_b_lsb);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_compare_len_diff_a_lower_than_b_msb);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_compare_len_diff_a_equals_b_len_a_greater);
    RUN_TEST_CASE(
        soft_bignumbers,
        soft_bignum_compare_len_diff_a_greater_than_b_lsb_len_a_greater);
    RUN_TEST_CASE(
        soft_bignumbers,
        soft_bignum_compare_len_diff_a_lower_than_b_lsb_len_a_greater);
    RUN_TEST_CASE(soft_bignumbers,
                  soft_bignum_compare_len_diff_a_equals_b_len_b_greater);
    RUN_TEST_CASE(
        soft_bignumbers,
        soft_bignum_compare_len_diff_a_greater_than_b_lsb_len_a_greater);
    RUN_TEST_CASE(
        soft_bignumbers,
        soft_bignum_compare_len_diff_a_lower_than_b_lsb_len_a_greater);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_compare_len_diff_len_a_greater);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_compare_len_diff_len_b_greater);

    /* div */
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_div_by_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_div_dividend_null_ptr);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_div_divisor_null_ptr);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_div_dividend_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_div_divisor_size_0);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_div_dividend_lt_divisor);
    RUN_TEST_CASE(soft_bignumbers, soft_bignum_div_success_1);
}
