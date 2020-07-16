#include "unity.h"
#include "unity_fixture.h"

#include <stdbool.h>
#include <string.h>

#include <api/software/scl_soft.h>

static const metal_scl_t scl = {
    .hca_base = 0,
    .bignum_func =
        {
            .compare = soft_bignum_compare,
            .compare_len_diff = soft_bignum_compare_len_diff,
            .is_null = soft_bignum_is_null,
            .inc = soft_bignum_inc,
            .add = soft_bignum_add,
            .sub = soft_bignum_sub,
            .mult = soft_bignum_mult,
            .leftshift = soft_bignum_leftshift,
            .rightshift = soft_bignum_rightshift,
            .msb_set_in_word = soft_bignum_msb_set_in_word,
            .get_msb_set = soft_bignum_get_msb_set,
            .set_bit = soft_bignum_set_bit,
            .div = soft_bignum_div,
            .mod = soft_bignum_mod,
        },
};

TEST_GROUP(soft_bignumbers);

TEST_SETUP(soft_bignumbers) {}

TEST_TEAR_DOWN(soft_bignumbers) {}

/* Addition */
TEST(soft_bignumbers, soft_bignum_add_size_0)
{
    int32_t result = 0;

    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0;

    result = soft_bignum_add(NULL, &in_a, &in_b, &out, 0);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_1)
{
    int32_t result = 0;

    static const uint64_t in_a = 0x00000000FFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0x00000000FFFFFFFFUL;

    result = soft_bignum_add(NULL, &in_a, &in_b, &out, 1);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_1_with_carry)
{
    int32_t result = 0;

    static const uint64_t in_a = 0x00000000FFFFFFFFUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0;

    result = soft_bignum_add(NULL, &in_a, &in_b, &out, 1);

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_2)
{
    int32_t result = 0;

    static const uint64_t in_a = 0x00000000FFFFFFFFUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0x0000000100000000UL;

    result = soft_bignum_add(NULL, &in_a, &in_b, &out,
                             sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_2_with_carry)
{
    int32_t result = 0;

    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0;

    result = soft_bignum_add(NULL, &in_a, &in_b, &out,
                             sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_5)
{
    int32_t result = 0;

    static const uint64_t in_a[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000EFFFFFFFUL};
    static const uint64_t in_b[3] = {1, 0, 0};
    uint64_t out[4] = {0, 0, 0, 0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t expected_out[4] = {0, 0, 0x00000000F0000000UL,
                                             0xFFFFFFFFFFFFFFFFUL};

    result = soft_bignum_add(NULL, in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_add_size_5_with_carry)
{
    int32_t result = 0;

    static const uint64_t in_a[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000FFFFFFFFUL};
    static const uint64_t in_b[3] = {1, 0, 0};
    uint64_t out[3] = {0};
    static const uint64_t expected_out[3] = {0, 0, 0};

    result = soft_bignum_add(NULL, in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_add_summ_all_FF)
{
    int32_t result = 0;

    static const uint64_t in_a[10] = {
        0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0x00000000FFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t in_b[10] = {1, 0, 0, 0, 0xFFFFFFFF00000000UL,
                                      0, 0, 0, 0, 0};
    uint64_t out[10] = {0};
    static const uint64_t expected_out[10] = {
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL};

    result =
        soft_bignum_add(NULL, in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_add_carry_1)
{
    int32_t result = 0;

    static const uint64_t in_a[10] = {
        0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0};
    static const uint64_t in_b[10] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint64_t out[10] = {0};
    static const uint64_t expected_out[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    result =
        soft_bignum_add(NULL, in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t) - 2);

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_add_carry_2)
{
    int32_t result = 0;

    static const uint64_t in_a[10] = {
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t in_b[10] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint64_t out[10] = {0};
    static const uint64_t expected_out[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    result =
        soft_bignum_add(NULL, in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_add_in_a_is_output)
{
    int32_t result = 0;

    uint64_t in_a[10] = {0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL};
    static const uint64_t in_b[10] = {
        0x1111111111111111UL, 0x1111111111111111UL, 0x1111111111111111UL,
        0x1111111111111111UL, 0x1111111111111111UL, 0x1111111111111111UL,
        0x1111111111111111UL, 0x1111111111111111UL, 0x1111111111111111UL,
        0x1111111111111111UL};

    static const uint64_t expected_out[10] = {
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x1111111122222222UL};

    result = soft_bignum_add(NULL, in_a, in_b, in_a,
                             sizeof(in_a) / sizeof(uint32_t) - 1);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, in_a, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_add_in_b_is_output)
{
    int32_t result = 0;

    static const uint64_t in_a[10] = {
        0x1111111111111111UL, 0x1111111111111111UL, 0x1111111111111111UL,
        0x1111111111111111UL, 0x1111111111111111UL, 0x1111111111111111UL,
        0x1111111111111111UL, 0x1111111111111111UL, 0x1111111111111111UL,
        0x1111111111111111UL};
    uint64_t in_b[10] = {0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL};

    static const uint64_t expected_out[10] = {
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x1111111122222222UL};

    result = soft_bignum_add(NULL, in_a, in_b, in_b,
                             sizeof(in_a) / sizeof(uint32_t) - 1);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, in_b, sizeof(expected_out));
}

/* Substraction */
TEST(soft_bignumbers, soft_bignum_sub_size_0)
{
    int32_t result = 0;

    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0;

    result = soft_bignum_sub(NULL, &in_a, &in_b, &out, 0);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_1)
{
    int32_t result = 0;

    static const uint64_t in_a = 0x00000000FFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0x00000000FFFFFFFDUL;

    result = soft_bignum_sub(NULL, &in_a, &in_b, &out, 1);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_1_with_carry)
{
    int32_t result = 0;

    static const uint64_t in_a = 1;
    static const uint64_t in_b = 0x00000000FFFFFFFFUL;
    uint64_t out = 0;
    static const uint64_t expected_out = 2;

    result = soft_bignum_sub(NULL, &in_a, &in_b, &out, 1);

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_2)
{
    int32_t result = 0;

    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0xFFFFFFFFFFFFFFFDUL;

    result = soft_bignum_sub(NULL, &in_a, &in_b, &out,
                             sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_2_with_carry)
{
    int32_t result = 0;

    static const uint64_t in_a = 1;
    static const uint64_t in_b = 0x00000000FFFFFFFFUL;
    uint64_t out = 0;
    static const uint64_t expected_out = 0xFFFFFFFF00000002;

    result = soft_bignum_sub(NULL, &in_a, &in_b, &out,
                             sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_5)
{
    int32_t result = 0;

    static const uint64_t in_a[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000EFFFFFFFUL};
    static const uint64_t in_b[3] = {1, 0, 0};
    uint64_t out[4] = {0, 0, 0, 0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t expected_out[4] = {
        0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL, 0x00000000EFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL};

    result = soft_bignum_sub(NULL, in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_sub_size_5_with_carry)
{
    int32_t result = 0;

    static const uint64_t in_a[3] = {1, 0, 0};
    static const uint64_t in_b[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000FFFFFFFFUL};

    uint64_t out[3] = {0};
    static const uint64_t expected_out[3] = {
        0x0000000000000002, 0x0000000000000000UL, 0x0000000000000000UL};

    result = soft_bignum_sub(NULL, in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_sub_size_5_with_carry_2)
{
    int32_t result = 0;

    static const uint64_t in_a[3] = {0, 2, 0};
    static const uint64_t in_b[3] = {1, 0xFF, 0};

    uint64_t out[3] = {0};
    static const uint64_t expected_out[3] = {
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFF02UL, 0x00000000FFFFFFFFUL};

    result = soft_bignum_sub(NULL, in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_sub_in_a_is_output)
{
    int32_t result = 0;

    uint64_t in_a[10] = {0x3333333333333333UL, 0x3333333333333333UL,
                         0x3333333333333333UL, 0x3333333333333333UL,
                         0x3333333333333333UL, 0x3333333333333333UL,
                         0x3333333333333333UL, 0x3333333333333333UL,
                         0x3333333333333333UL, 0x1111111133333333UL};
    static const uint64_t in_b[10] = {
        0x1111111111111111UL, 0x1111111111111111UL, 0x1111111111111111UL,
        0x1111111111111111UL, 0x1111111111111111UL, 0x1111111111111111UL,
        0x1111111111111111UL, 0x1111111111111111UL, 0x1111111111111111UL,
        0x1111111111111111UL};

    static const uint64_t expected_out[10] = {
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x1111111122222222UL};

    result = soft_bignum_sub(NULL, in_a, in_b, in_a,
                             sizeof(in_a) / sizeof(uint32_t) - 1);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, in_a, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_sub_in_b_is_output)
{
    int32_t result = 0;

    static const uint64_t in_a[10] = {
        0x3333333333333333UL, 0x3333333333333333UL, 0x3333333333333333UL,
        0x3333333333333333UL, 0x3333333333333333UL, 0x3333333333333333UL,
        0x3333333333333333UL, 0x3333333333333333UL, 0x3333333333333333UL,
        0x1111111133333333UL};
    uint64_t in_b[10] = {0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL,
                         0x1111111111111111UL, 0x1111111111111111UL};

    static const uint64_t expected_out[10] = {
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x2222222222222222UL, 0x2222222222222222UL, 0x2222222222222222UL,
        0x1111111122222222UL};

    result = soft_bignum_sub(NULL, in_a, in_b, in_b,
                             sizeof(in_a) / sizeof(uint32_t) - 1);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, in_b, sizeof(expected_out));
}

/* Increment by one */
TEST(soft_bignumbers, soft_bignum_inc_size_0)
{
    int32_t result = 0;

    uint64_t in = 0;
    static const uint64_t expected_out = 0;

    result = soft_bignum_inc(NULL, &in, 0);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_1)
{
    int32_t result = 0;

    uint64_t in = 0x00000000FFFFFFFEUL;
    static const uint64_t expected_out = 0x00000000FFFFFFFFUL;

    result = soft_bignum_inc(NULL, &in, 1);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_1_with_carry)
{
    int32_t result = 0;

    uint64_t in = 0x00000000FFFFFFFFUL;
    static const uint64_t expected_out = 0;

    result = soft_bignum_inc(NULL, &in, 1);

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_2)
{
    int32_t result = 0;

    uint64_t in = 0x00000000FFFFFFFFUL;
    static const uint64_t expected_out = 0x0000000100000000UL;

    result = soft_bignum_inc(NULL, &in, sizeof(in) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_2_with_carry)
{
    int32_t result = 0;
    uint64_t in = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t expected_out = 0;

    result = soft_bignum_inc(NULL, &in, sizeof(in) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_5)
{
    int32_t result = 0;
    uint64_t in[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                      0x00000000EFFFFFFFUL};
    static const uint64_t expected_out[3] = {0, 0, 0x00000000F0000000UL};

    result = soft_bignum_inc(NULL, in, 5);

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, in, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_inc_size_5_with_carry)
{
    int32_t result = 0;
    uint64_t in[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                      0x00000000FFFFFFFFUL};
    static const uint64_t expected_out[3] = {0, 0, 0};

    result = soft_bignum_inc(NULL, in, 5);

    TEST_ASSERT_TRUE(1 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, in, sizeof(expected_out));
}

/* Compare */
TEST(soft_bignumbers, soft_bignum_compare_a_equals_b)
{
    int32_t result = 0;
    size_t word_size;

    static const uint64_t a[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    word_size = sizeof(b) / sizeof(uint32_t);

    result = soft_bignum_compare(NULL, a, b, word_size);

    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_a_greater_than_b_lsb)
{
    int32_t result = 0;
    size_t word_size;

    static const uint64_t a[24] = {
        0xF2A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    word_size = sizeof(b) / sizeof(uint32_t);

    result = soft_bignum_compare(NULL, a, b, word_size);

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_a_greater_than_b_msb)
{
    int32_t result = 0;
    size_t word_size;

    static const uint64_t a[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3F798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    word_size = sizeof(b) / sizeof(uint32_t);

    result = soft_bignum_compare(NULL, a, b, word_size);

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_a_lower_than_b_lsb)
{
    int32_t result = 0;
    size_t word_size;

    static const uint64_t a[24] = {
        0xF0A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    word_size = sizeof(b) / sizeof(uint32_t);

    result = soft_bignum_compare(NULL, a, b, word_size);

    TEST_ASSERT_TRUE(-1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_a_lower_than_b_msb)
{
    int32_t result = 0;
    size_t word_size;

    static const uint64_t a[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3D798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    word_size = sizeof(b) / sizeof(uint32_t);

    result = soft_bignum_compare(NULL, a, b, word_size);

    TEST_ASSERT_TRUE(-1 == result);
}

/* Multiplication */
TEST(soft_bignumbers, soft_bignum_mult_size_0)
{
    int32_t result = 0;
    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out[2] = {0};
    static const uint64_t expected_out[2] = {0};

    result = soft_bignum_mult(NULL, &in_a, &in_b, out, 0);

    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
}

TEST(soft_bignumbers, soft_bignum_mult_size_1)
{
    int32_t result = 0;
    static const uint64_t in_a = 0x00000000FFFFFFFFUL;
    static const uint64_t in_b = 0x00000000FFFFFFFFUL;
    uint64_t out = 0;
    static const uint64_t expected_out = 0xFFFFFFFE00000001UL;

    result = soft_bignum_mult(NULL, &in_a, &in_b, &out, 1);

    TEST_ASSERT_TRUE(expected_out == out);
    TEST_ASSERT_TRUE(SCL_OK == result);
}

TEST(soft_bignumbers, soft_bignum_mult_size_2)
{
    int32_t result = 0;
    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_b = 0xFFFFFFFFFFFFFFFFUL;
    uint64_t out[2] = {0};
    static const uint64_t expected_out[2] = {0x0000000000000001UL,
                                             0xFFFFFFFFFFFFFFFEUL};

    result = soft_bignum_mult(NULL, &in_a, &in_b, out,
                              sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(SCL_OK == result);
}

TEST(soft_bignumbers, soft_bignum_mult_size_5)
{
    int32_t result = 0;
    static const uint64_t in_a[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000FFFFFFFFUL};
    static const uint64_t in_b[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000FFFFFFFFUL};

    uint64_t out[5] = {0};
    static const uint64_t expected_out[5] = {
        0x0000000000000001UL, 0x0000000000000000UL, 0xFFFFFFFE00000000UL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL};

    result = soft_bignum_mult(NULL, in_a, in_b, out,
                              sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(SCL_OK == result);
}

TEST(soft_bignumbers, soft_bignum_mult_size_5_zero)
{
    int32_t result = 0;
    static const uint64_t in_a[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000FFFFFFFFUL};
    static const uint64_t in_b[3] = {0, 0, 0};

    uint64_t out[5] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                       0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                       0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t expected_out[5] = {0, 0, 0, 0, 0};

    result = soft_bignum_mult(NULL, in_a, in_b, out,
                              sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(SCL_OK == result);
}

TEST(soft_bignumbers, soft_bignum_mult_size_5_identity)
{
    int32_t result = 0;
    static const uint64_t in_a[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000FFFFFFFFUL};
    static const uint64_t in_b[3] = {1, 0, 0};

    uint64_t out[5] = {0, 0, 0, 0, 0};
    static const uint64_t expected_out[5] = {
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0x00000000FFFFFFFFUL, 0, 0};

    result = soft_bignum_mult(NULL, in_a, in_b, out,
                              sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(SCL_OK == result);
}

/* Right shift */
TEST(soft_bignumbers, soft_bignum_rightshift_size_0)
{
    int32_t retval = 0;
    uint64_t in[1] = {0xFFFFFFFFFFFFFFFFUL};
    uint64_t out[1] = {0};
    static const uint64_t expected_out[1] = {0};

    retval = soft_bignum_rightshift(NULL, in, out, 2, 0);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_1)
{
    int32_t retval = 0;
    uint64_t in[1] = {0x00000000FFFFFFFFUL};
    uint64_t out[1] = {0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t expected_out[1] = {0xFFFFFFFF3FFFFFFFUL};

    retval = soft_bignum_rightshift(NULL, in, out, 2, 1);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_2)
{
    int32_t retval = 0;
    uint64_t in[1] = {0xFFFFFFFFFFFFFFFFUL};
    uint64_t out[1] = {0};
    static const uint64_t expected_out[1] = {0x3FFFFFFFFFFFFFFFUL};

    retval = soft_bignum_rightshift(NULL, in, out, 2, 2);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_5)
{
    int32_t retval = 0;
    uint64_t in[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                      0x0FFFFFFFEFFFFFFFUL};
    uint64_t out[3] = {0, 0, 0x0FFFFFFF00000000UL};
    static const uint64_t expected_out[3] = {
        0xFFFFFFFFFFFFFFFFUL, 0x00000000EFFFFFFFUL, 0x0FFFFFFF00000000UL};

    retval = soft_bignum_rightshift(NULL, in, out, 64, 5);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_7)
{
    int32_t retval = 0;
    uint64_t in[4] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                      0xFFFFFFFFFFFFFFFFUL, 0x00000000EFFFFFFFUL};
    uint64_t out[4] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                       0xFFFFFFFFFFFFFFFFUL, 0x0FFFFFFFFFFFFFFFUL};
    static const uint64_t expected_out[4] = {
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0x0000000077FFFFFFUL,
        0x0FFFFFFF00000000UL};

    retval = soft_bignum_rightshift(NULL, in, out, 65, 7);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_5_shift_0)
{
    int32_t retval = 0;
    uint64_t in[3] = {0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL,
                      0x000000000FFFFFFFUL};
    uint64_t out[3] = {0, 0, 0};
    static const uint64_t expected_out[3] = {
        0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL, 0x000000000FFFFFFFUL};

    retval = soft_bignum_rightshift(NULL, in, out, 0, 5);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_5_shift_159)
{
    int32_t retval = 0;
    uint64_t in[3] = {0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL,
                      0x00000000FFFFFFFFUL};
    uint64_t out[3] = {0, 0, 0};
    static const uint64_t expected_out[3] = {1, 0, 0};

    retval = soft_bignum_rightshift(NULL, in, out, 159, 5);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_5_shift_160)
{
    int32_t retval = 0;
    uint64_t in[3] = {0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL,
                      0x000000000FFFFFFFUL};
    uint64_t out[3] = {0, 0, 0};
    static const uint64_t expected_out[3] = {0, 0, 0};

    retval = soft_bignum_rightshift(NULL, in, out, 160, 5);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_5_in_NULL)
{
    int32_t retval = 0;
    uint64_t out[3] = {0, 0, 0};

    retval = soft_bignum_rightshift(NULL, NULL, out, 64, 5);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == retval);
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_5_out_NULL)
{
    int32_t retval = 0;
    uint64_t in[3] = {0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL,
                      0x000000000FFFFFFFUL};

    retval = soft_bignum_rightshift(NULL, in, NULL, 64, 5);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == retval);
}

TEST(soft_bignumbers, soft_bignum_rightshift_size_7_shift_59)
{
    int32_t retval = 0;
    uint64_t in[4] = {0x7800000000000000, 0x00091A2B3C4D5E6F, 0,
                      0xffffffff00000000};

    static const uint64_t expected_out[4] = {0x0123456789ABCDEF, 0, 0,
                                             0xffffffff00000000};

    retval = soft_bignum_rightshift(NULL, in, in, 59, 7);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, in, sizeof(expected_out));
}

/* Left shift */
TEST(soft_bignumbers, soft_bignum_leftshift_size_0)
{
    int32_t retval = 0;
    uint64_t in[1] = {0xFFFFFFFFFFFFFFFFUL};
    uint64_t out[1] = {0};
    static const uint64_t expected_out[1] = {0};

    retval = soft_bignum_leftshift(NULL, in, out, 2, 0);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_1)
{
    int32_t retval = 0;
    uint64_t in[1] = {0x000000000FFFFFFFUL};
    uint64_t out[1] = {0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t expected_out[1] = {0xFFFFFFFF3FFFFFFCUL};

    retval = soft_bignum_leftshift(NULL, in, out, 2, 1);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_2)
{
    int32_t retval = 0;
    uint64_t in[1] = {0x0FFFFFFFFFFFFFFFUL};
    uint64_t out[1] = {0};
    static const uint64_t expected_out[1] = {0x3FFFFFFFFFFFFFFCUL};

    retval = soft_bignum_leftshift(NULL, in, out, 2, 2);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_5)
{
    int32_t retval = 0;
    uint64_t in[3] = {0xFFFFFFFF00000000UL, 0xFFFFFFFFFFFFFFFFUL,
                      0x0FFFFFFF0FFFFFFFUL};
    uint64_t out[3] = {0, 0, 0};
    static const uint64_t expected_out[3] = {0, 0xFFFFFFFF00000000UL,
                                             0x00000000FFFFFFFFUL};

    retval = soft_bignum_leftshift(NULL, in, out, 64, 5);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_7)
{
    int32_t retval = 0;
    uint64_t in[4] = {0xFFFFFFFF00000000UL, 0xFFFFFFFFFFFFFFFFUL,
                      0xFFFFFFFFFFFFFFFFUL, 0x00000000FFFFFFFFUL};
    uint64_t out[4] = {0, 0, 0, 0};
    static const uint64_t expected_out[4] = {
        0, 0xFFFFFFFE00000000UL, 0xFFFFFFFFFFFFFFFFUL, 0x00000000FFFFFFFFUL};

    retval = soft_bignum_leftshift(NULL, in, out, 65, 7);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_5_shift_0)
{
    int32_t retval = 0;
    uint64_t in[3] = {0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL,
                      0x000000000FFFFFFFUL};
    uint64_t out[3] = {0, 0, 0};
    static const uint64_t expected_out[3] = {
        0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL, 0x000000000FFFFFFFUL};

    retval = soft_bignum_leftshift(NULL, in, out, 0, 5);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_5_shift_159)
{
    int32_t retval = 0;
    uint64_t in[3] = {0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL,
                      0x000000000FFFFFFFUL};
    uint64_t out[3] = {0, 0, 0};
    static const uint64_t expected_out[3] = {0, 0, 0x80000000};

    retval = soft_bignum_leftshift(NULL, in, out, 159, 5);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_5_shift_160)
{
    int32_t retval = 0;
    uint64_t in[3] = {0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL,
                      0x000000000FFFFFFFUL};
    uint64_t out[3] = {0, 0, 0};
    static const uint64_t expected_out[3] = {0, 0, 0};

    retval = soft_bignum_leftshift(NULL, in, out, 160, 5);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_5_in_NULL)
{
    int32_t retval = 0;
    uint64_t out[3] = {0, 0, 0};

    retval = soft_bignum_leftshift(NULL, NULL, out, 64, 5);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == retval);
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_5_out_NULL)
{
    int32_t retval = 0;
    uint64_t in[3] = {0x0FFFFFFF0FFFFFFFUL, 0x0FFFFFFF0FFFFFFFUL,
                      0x000000000FFFFFFFUL};

    retval = soft_bignum_leftshift(NULL, in, NULL, 64, 5);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == retval);
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_7_shift_59)
{
    int32_t retval = 0;
    uint64_t in[4] = {0x0123456789ABCDEF, 0, 0, 0xffffffff00000000};
    static const uint64_t expected_out[4] = {
        0x7800000000000000, 0x00091A2B3C4D5E6F, 0, 0xffffffff00000000};

    retval = soft_bignum_leftshift(NULL, in, in, 59, 7);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, in, sizeof(expected_out));
}

TEST(soft_bignumbers, soft_bignum_leftshift_size_5_shift_39)
{
    int32_t retval = 0;
    uint64_t in[3] = {0x0123456789ABCDEF, 0x0123456789ABCDEF,
                      0xFFFFFFFF00000000};
    static const uint64_t expected_out[3] = {
        0xD5E6F78000000000UL, 0xD5E6F78091A2B3C4UL, 0xFFFFFFFF91A2B3C4UL};

    retval = soft_bignum_leftshift(NULL, in, in, 39, 5);

    TEST_ASSERT_TRUE(0 == retval);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, in, sizeof(expected_out));
}

/* test on msb set in word */
TEST(soft_bignumbers, soft_bignum_msb_set_in_word_32b_word)
{
    int32_t result = 0;
    uint32_t word = 0x80000000;

    result = soft_bignum_msb_set_in_word(word);

    TEST_ASSERT_TRUE(32 == result);
}

TEST(soft_bignumbers, soft_bignum_msb_set_in_word_64b_word_last_bit_set)
{
    int32_t result = 0;
    uint64_t word = 0x8000000000000000;

    result = soft_bignum_msb_set_in_word(word);

    TEST_ASSERT_TRUE(64 == result);
}

TEST(soft_bignumbers, soft_bignum_msb_set_in_word_64b_word_first_bit_set)
{
    int32_t result = 0;
    uint64_t word = 0x0000000000000001;

    result = soft_bignum_msb_set_in_word(word);

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers, soft_bignum_msb_set_in_word_32b_word_none_set)
{
    int32_t result = 0;
    uint32_t word[2] = {0x00000000, 0x00000001};

    result = soft_bignum_msb_set_in_word(word[0]);

    TEST_ASSERT_TRUE(0 == result);
}

/* test if bignumber is null */
TEST(soft_bignumbers, soft_bignum_is_null_size_6_lsb_set)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000001, 0x0000000000000000,
                         0x0000000000000000};

    result = soft_bignum_is_null(NULL, (uint32_t *)array, 6);

    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_is_null_size_6_msb_set)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x0000000000000000,
                         0x8000000000000000};

    result = soft_bignum_is_null(NULL, (uint32_t *)array, 6);

    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_is_null_size_5_msb_set)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x0000000000000000,
                         0x0000000080000000};

    result = soft_bignum_is_null(NULL, (uint32_t *)array, 5);

    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_is_null_size_6_null)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000};

    result = soft_bignum_is_null(NULL, (uint32_t *)array, 6);

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers, soft_bignum_is_null_size_5_null)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000};

    result = soft_bignum_is_null(NULL, (uint32_t *)array, 5);

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers, soft_bignum_is_null_array_nullptr)
{
    int32_t result = 0;

    result = soft_bignum_is_null(NULL, NULL, 6);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == result);
}

TEST(soft_bignumbers, soft_bignum_is_null_size_0)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000001, 0x0000000000000000,
                         0x0000000000000000};

    result = soft_bignum_is_null(NULL, (uint32_t *)array, 0);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
}

/* Get MSB set in bignumber */
TEST(soft_bignumbers, soft_bignum_get_msb_set_size_0)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000001, 0x0000000000000000,
                         0x0000000000000000};

    result = soft_bignum_get_msb_set(NULL, array, 0);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
}

TEST(soft_bignumbers, soft_bignum_get_msb_set_nullptr)
{
    int32_t result = 0;

    result = soft_bignum_get_msb_set(NULL, NULL, 6);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == result);
}

TEST(soft_bignumbers, soft_bignum_get_msb_set_size_5_expect_1)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000001, 0x0000000000000000,
                         0x0000000000000000};

    result = soft_bignum_get_msb_set(NULL, array, 5);

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers, soft_bignum_get_msb_set_size_5_expect_0)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x0000000000000000,
                         0x0000000100000000};

    result = soft_bignum_get_msb_set(NULL, array, 5);

    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_get_msb_set_size_5_expect_159)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x0000000000000000,
                         0x0000000080000000};

    result = soft_bignum_get_msb_set(NULL, array, 5);

    TEST_ASSERT_TRUE(160 == result);
}

TEST(soft_bignumbers, soft_bignum_get_msb_set_size_5_124)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x1000000000000000,
                         0x0000000000000000};

    result = soft_bignum_get_msb_set(NULL, array, 5);

    TEST_ASSERT_TRUE(125 == result);
}

/* set one bit in a big integer */
TEST(soft_bignumbers, soft_bignum_set_bit_null_ptr)
{
    int32_t result = 0;

    result = soft_bignum_set_bit(NULL, NULL, 5, 1);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == result);
}

TEST(soft_bignumbers, soft_bignum_set_bit_size_0)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x1000000000000000,
                         0x0000000000000000};

    result = soft_bignum_set_bit(NULL, array, 0, 1);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
}

TEST(soft_bignumbers, soft_bignum_set_bit_size_5_set_first)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000};
    uint64_t expected_array[3] = {0x0000000000000001, 0x0000000000000000,
                                  0x0000000000000000};

    result = soft_bignum_set_bit(NULL, array, 5, 0);

    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_array, array, sizeof(expected_array));
}

TEST(soft_bignumbers, soft_bignum_set_bit_size_5_set_last)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000};
    uint64_t expected_array[3] = {0x0000000000000000, 0x0000000000000000,
                                  0x0000000080000000};

    result = soft_bignum_set_bit(NULL, array, 5, 159);

    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_array, array, sizeof(expected_array));
}

TEST(soft_bignumbers, soft_bignum_set_bit_size_5_set_out_of_range)
{
    int32_t result = 0;
    uint64_t array[3] = {0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000};
    uint64_t expected_array[3] = {0x0000000000000000, 0x0000000000000000,
                                  0x0000000000000000};

    result = soft_bignum_set_bit(NULL, array, 5, 160);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_array, array, sizeof(expected_array));
}

/* Compare with different length */
TEST(soft_bignumbers, soft_bignum_compare_len_diff_a_equals_b)
{
    int32_t result = 0;
    static const uint64_t a[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_len_diff_a_greater_than_b_lsb)
{
    int32_t result = 0;

    static const uint64_t a[24] = {
        0xF2A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_len_diff_a_greater_than_b_msb)
{
    int32_t result = 0;

    static const uint64_t a[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3F798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_len_diff_a_lower_than_b_lsb)
{
    int32_t result = 0;

    static const uint64_t a[24] = {
        0xF0A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(-1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_len_diff_a_lower_than_b_msb)
{
    int32_t result = 0;

    static const uint64_t a[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3D798BF33B755747UL,
    };

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(-1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_len_diff_a_equals_b_len_a_greater)
{
    int32_t result = 0;
    static const uint64_t a[25] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
        0x0000000000000000UL};

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers,
     soft_bignum_compare_len_diff_a_greater_than_b_lsb_len_a_greater)
{
    int32_t result = 0;

    static const uint64_t a[25] = {
        0xF2A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
        0x0000000000000000UL};

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers,
     soft_bignum_compare_len_diff_a_lower_than_b_lsb_len_a_greater)
{
    int32_t result = 0;

    static const uint64_t a[25] = {
        0xF0A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
        0x0000000000000000UL};

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(-1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_len_diff_a_equals_b_len_b_greater)
{
    int32_t result = 0;
    static const uint64_t a[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[25] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
        0x0000000000000000UL};

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers,
     soft_bignum_compare_len_diff_a_greater_than_b_lsb_len_b_greater)
{
    int32_t result = 0;

    static const uint64_t a[24] = {
        0xF2A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[25] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
        0x0000000000000000UL};

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers,
     soft_bignum_compare_len_diff_a_lower_than_b_lsb_len_b_greater)
{
    int32_t result = 0;

    static const uint64_t a[24] = {
        0xF0A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[25] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
        0x0000000000000000UL};

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(-1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_len_diff_len_a_greater)
{
    int32_t result = 0;
    static const uint64_t a[25] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
        0x0000000000000001UL};

    static const uint64_t b[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == result);
}

TEST(soft_bignumbers, soft_bignum_compare_len_diff_len_b_greater)
{
    int32_t result = 0;
    static const uint64_t a[24] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
    };

    static const uint64_t b[25] = {
        0xF1A731C16826B112UL, 0xC2D6C36F322ADB31UL, 0xADF814D04293621AUL,
        0x99257956ACB04888UL, 0x213CB160A56E652BUL, 0x5EA07B4C36F5E742UL,
        0x7C6EA7AFFD28FD8CUL, 0x60A8B22AC65FF673UL, 0x35299037A28056EEUL,
        0xA6396CA2E6D640CAUL, 0x1BAACCE52D040622UL, 0x780D9E8F08E3822EUL,
        0x67EF2D9DD4D5E501UL, 0x62EE9A2018317A61UL, 0xFB1B54732E3EA55FUL,
        0x31B582608D37B9AEUL, 0xE661A5C8F4AFCDADUL, 0x6FB02950DC6A0F66UL,
        0x47FE18A5DA8C7F52UL, 0x9C0BB95DD4E9D4ABUL, 0xBB734830CFEAE7A3UL,
        0x96F63E471BF5B240UL, 0xAB08087F8E40F50FUL, 0x3E798BF33B755747UL,
        0x0000000000000001UL};

    result = soft_bignum_compare_len_diff(NULL, a, sizeof(a) / sizeof(uint32_t),
                                          b, sizeof(b) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(-1 == result);
}

/* big integer division */
TEST(soft_bignumbers, soft_bignum_div_by_0)
{
    int32_t result = 0;
    uint64_t dividend[6] = {0x0000000000000001, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t divisor[3] = {0x0000000000000000, 0x0000000000000000,
                           0x0000000000000000};
    uint64_t quotient[6] = {0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};

    uint64_t expected_quotient[6] = {0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000};
    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result = soft_bignum_div(
        &scl, dividend, sizeof(dividend) / sizeof(uint32_t), divisor,
        sizeof(divisor) / sizeof(uint32_t), remainder, quotient);

    TEST_ASSERT_TRUE(SCL_ZERO_DIVISION == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_quotient, quotient,
                                 sizeof(expected_quotient));
}

TEST(soft_bignumbers, soft_bignum_div_dividend_null_ptr)
{
    int32_t result = 0;
    uint64_t dividend[6] = {0x0000000000000001, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t divisor[3] = {0x0000000000000000, 0x0000000000000000,
                           0x0000000000000000};
    uint64_t quotient[6] = {0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};

    uint64_t expected_quotient[6] = {0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000};
    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result = soft_bignum_div(&scl, NULL, sizeof(dividend) / sizeof(uint32_t),
                             divisor, sizeof(divisor) / sizeof(uint32_t),
                             remainder, quotient);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_quotient, quotient,
                                 sizeof(expected_quotient));
}

TEST(soft_bignumbers, soft_bignum_div_divisor_null_ptr)
{
    int32_t result = 0;
    uint64_t dividend[6] = {0x0000000000000001, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t divisor[3] = {0x0000000000000001, 0x0000000000000000,
                           0x0000000000000000};
    uint64_t quotient[6] = {0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};

    uint64_t expected_quotient[6] = {0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000};
    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result = soft_bignum_div(
        &scl, dividend, sizeof(dividend) / sizeof(uint32_t), NULL,
        sizeof(divisor) / sizeof(uint32_t), remainder, quotient);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_quotient, quotient,
                                 sizeof(expected_quotient));
}

TEST(soft_bignumbers, soft_bignum_div_dividend_size_0)
{
    int32_t result = 0;
    uint64_t dividend[6] = {0x0000000000000001, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t divisor[3] = {0x0000000000000001, 0x0000000000000000,
                           0x0000000000000000};
    uint64_t quotient[6] = {0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};

    uint64_t expected_quotient[6] = {0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000};
    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result = soft_bignum_div(&scl, dividend, 0, divisor,
                             sizeof(divisor) / sizeof(uint32_t), remainder,
                             quotient);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_quotient, quotient,
                                 sizeof(expected_quotient));
}

TEST(soft_bignumbers, soft_bignum_div_divisor_size_0)
{
    int32_t result = 0;
    uint64_t dividend[6] = {0x0000000000000001, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t divisor[3] = {0x0000000000000001, 0x0000000000000000,
                           0x0000000000000000};
    uint64_t quotient[6] = {0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};

    uint64_t expected_quotient[6] = {0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000,
                                     0x0000000000000000, 0x0000000000000000};
    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result =
        soft_bignum_div(&scl, dividend, sizeof(dividend) / sizeof(uint32_t),
                        divisor, 0, remainder, quotient);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_quotient, quotient,
                                 sizeof(expected_quotient));
}

TEST(soft_bignumbers, soft_bignum_div_dividend_lt_divisor)
{
    int32_t result = 0;
    uint64_t dividend[6] = {
        0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xEFFFFFFFFFFFFFFF, 0, 0, 0};
    uint64_t divisor[3] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                           0xFFFFFFFFFFFFFFFF};
    uint64_t quotient[6] = {0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000,
                            0x0000000000000000, 0x0000000000000000};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};

    uint64_t expected_quotient[6] = {0, 0, 0, 0, 0, 0};
    uint64_t expected_remainder[3] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                      0xEFFFFFFFFFFFFFFF};

    result = soft_bignum_div(
        &scl, dividend, sizeof(dividend) / sizeof(uint32_t), divisor,
        sizeof(divisor) / sizeof(uint32_t), remainder, quotient);

    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_quotient, quotient,
                                 sizeof(expected_quotient));
}

TEST(soft_bignumbers, soft_bignum_div_dividend_gt_divisor)
{
    int32_t result = 0;
    uint64_t dividend[6] = {0x0FFFFFFFFFFFFFFF, 0, 0, 0, 0, 0};
    uint64_t divisor[3] = {0x0123456789ABCDEF, 0, 0};
    uint64_t quotient[6] = {0, 0, 0, 0, 0, 0};
    uint64_t remainder[3] = {0, 0, 0};

    uint64_t expected_quotient[6] = {0x0E, 0, 0, 0, 0, 0};
    uint64_t expected_remainder[3] = {0x0123456789ABCED, 0, 0};

    result = soft_bignum_div(
        &scl, dividend, sizeof(dividend) / sizeof(uint32_t), divisor,
        sizeof(divisor) / sizeof(uint32_t), remainder, quotient);

    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_quotient, quotient,
                                 sizeof(expected_quotient));
}

/* modulus computation */
TEST(soft_bignumbers, soft_bignum_mod_modulus_0)
{
    int32_t result = 0;
    uint64_t input[6] = {0x0000000000000001, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000};
    uint64_t modulus[3] = {0x0000000000000000, 0x0000000000000000,
                           0x0000000000000000};

    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};

    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result =
        soft_bignum_mod(&scl, input, sizeof(input) / sizeof(uint32_t), modulus,
                        sizeof(modulus) / sizeof(uint32_t), remainder);

    TEST_ASSERT_TRUE(SCL_ZERO_DIVISION == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
}

TEST(soft_bignumbers, soft_bignum_mod_input_null_ptr)
{
    int32_t result = 0;
    uint64_t input[6] = {0x0000000000000001, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000};
    uint64_t modulus[3] = {0x0000000000000000, 0x0000000000000000,
                           0x0000000000000000};

    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};

    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result =
        soft_bignum_mod(&scl, NULL, sizeof(input) / sizeof(uint32_t), modulus,
                        sizeof(modulus) / sizeof(uint32_t), remainder);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
}

TEST(soft_bignumbers, soft_bignum_mod_modulus_null_ptr)
{
    int32_t result = 0;
    uint64_t input[6] = {0x0000000000000001, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000};
    uint64_t modulus[3] = {0x0000000000000001, 0x0000000000000000,
                           0x0000000000000000};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};
    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result =
        soft_bignum_mod(&scl, input, sizeof(input) / sizeof(uint32_t), NULL,
                        sizeof(modulus) / sizeof(uint32_t), remainder);

    TEST_ASSERT_TRUE(SCL_INVALID_INPUT == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
}

TEST(soft_bignumbers, soft_bignum_mod_input_size_0)
{
    int32_t result = 0;
    uint64_t input[6] = {0x0000000000000001, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000};
    uint64_t modulus[3] = {0x0000000000000001, 0x0000000000000000,
                           0x0000000000000000};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};
    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result = soft_bignum_mod(&scl, input, 0, modulus,
                             sizeof(modulus) / sizeof(uint32_t), remainder);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
}

TEST(soft_bignumbers, soft_bignum_mod_modulus_size_0)
{
    int32_t result = 0;
    uint64_t input[6] = {0x0000000000000001, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000,
                         0x0000000000000000, 0x0000000000000000};
    uint64_t modulus[3] = {0x0000000000000001, 0x0000000000000000,
                           0x0000000000000000};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};
    uint64_t expected_remainder[3] = {0x0000000000000000, 0x0000000000000000,
                                      0x0000000000000000};

    result = soft_bignum_mod(&scl, input, sizeof(input) / sizeof(uint32_t),
                             modulus, 0, remainder);

    TEST_ASSERT_TRUE(SCL_INVALID_LENGTH == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
}

TEST(soft_bignumbers, soft_bignum_mod_input_lt_modulus)
{
    int32_t result = 0;
    uint64_t input[6] = {
        0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xEFFFFFFFFFFFFFFF, 0, 0, 0};
    uint64_t modulus[3] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                           0xFFFFFFFFFFFFFFFF};
    uint64_t remainder[3] = {0x0000000000000000, 0x0000000000000000,
                             0x0000000000000000};
    uint64_t expected_remainder[3] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                      0xEFFFFFFFFFFFFFFF};

    result =
        soft_bignum_mod(&scl, input, sizeof(input) / sizeof(uint32_t), modulus,
                        sizeof(modulus) / sizeof(uint32_t), remainder);

    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
}

TEST(soft_bignumbers, soft_bignum_mod_input_gt_modulus)
{
    int32_t result = 0;
    uint64_t input[6] = {0x0FFFFFFFFFFFFFFF, 0, 0, 0, 0, 0};
    uint64_t modulus[3] = {0x0123456789ABCDEF, 0, 0};
    uint64_t remainder[3] = {0, 0, 0};

    uint64_t expected_remainder[3] = {0x0123456789ABCED, 0, 0};

    result =
        soft_bignum_mod(&scl, input, sizeof(input) / sizeof(uint32_t), modulus,
                        sizeof(modulus) / sizeof(uint32_t), remainder);

    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
}

TEST(soft_bignumbers, soft_bignum_mod_input_gt_modulus_2)
{
    int32_t result = 0;
    uint64_t input[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                         0x00000000FFFFFFFFUL};
    uint64_t modulus[2] = {0x0123456789ABCDEFUL, 0x0123456789ABCDEFUL};
    uint64_t remainder[2] = {0, 0};
    uint64_t expected_remainder[2] = {0x000000F0FFFFFFFFUL, 0x000000F000000000};

    result =
        soft_bignum_mod(&scl, input, sizeof(input) / sizeof(uint32_t), modulus,
                        sizeof(modulus) / sizeof(uint32_t), remainder);

    TEST_ASSERT_TRUE(0 == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_remainder, remainder,
                                 sizeof(expected_remainder));
}
