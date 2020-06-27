#include "unity.h"
#include "unity_fixture.h"

#include <stdbool.h>
#include <string.h>

#include <api/software/scl_soft.h>

TEST_GROUP(soft_bignumbers);

TEST_SETUP(soft_bignumbers) {}

TEST_TEAR_DOWN(soft_bignumbers) {}

/* Addition */
TEST(soft_bignumbers, soft_bignum_add_size_0)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0;

    carry = soft_bignum_add(&in_a, &in_b, &out, 0);

    TEST_ASSERT_TRUE(0 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_1)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 0x00000000FFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0x00000000FFFFFFFFUL;

    carry = soft_bignum_add(&in_a, &in_b, &out, 1);

    TEST_ASSERT_TRUE(0 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_1_with_carry)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 0x00000000FFFFFFFFUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0;

    carry = soft_bignum_add(&in_a, &in_b, &out, 1);

    TEST_ASSERT_TRUE(1 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_2)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 0x00000000FFFFFFFFUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0x0000000100000000UL;

    carry =
        soft_bignum_add(&in_a, &in_b, &out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(0 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_2_with_carry)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0;

    carry =
        soft_bignum_add(&in_a, &in_b, &out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_5)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_a[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000EFFFFFFFUL};
    static const uint64_t in_b[3] = {1, 0, 0};
    uint64_t out[4] = {0, 0, 0, 0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t expected_out[4] = {0, 0, 0x00000000F0000000UL,
                                             0xFFFFFFFFFFFFFFFFUL};

    carry = soft_bignum_add(in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(0 == carry);
    result = memcmp(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_add_size_5_with_carry)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_a[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000FFFFFFFFUL};
    static const uint64_t in_b[3] = {1, 0, 0};
    uint64_t out[3] = {0};
    static const uint64_t expected_out[3] = {0, 0, 0};

    carry = soft_bignum_add(in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(1 == carry);
    result = memcmp(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_add_summ_all_FF)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
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

    carry = soft_bignum_add(in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(0 == carry);
    result = memcmp(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_add_carry_1)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_a[10] = {
        0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0};
    static const uint64_t in_b[10] = {2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint64_t out[10] = {0};
    static const uint64_t expected_out[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};

    carry = soft_bignum_add(in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(0 == carry);
    result = memcmp(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_add_carry_2)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_a[10] = {
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t in_b[10] = {1, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint64_t out[10] = {0};
    static const uint64_t expected_out[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    carry = soft_bignum_add(in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == carry);
    result = memcmp(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}

/* Substraction */
TEST(soft_bignumbers, soft_bignum_sub_size_0)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0;

    carry = soft_bignum_sub(&in_a, &in_b, &out, 0);

    TEST_ASSERT_TRUE(0 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_1)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 0x00000000FFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0x00000000FFFFFFFDUL;

    carry = soft_bignum_sub(&in_a, &in_b, &out, 1);

    TEST_ASSERT_TRUE(0 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_1_with_carry)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 1;
    static const uint64_t in_b = 0x00000000FFFFFFFFUL;
    uint64_t out = 0;
    static const uint64_t expected_out = 2;

    carry = soft_bignum_sub(&in_a, &in_b, &out, 1);

    TEST_ASSERT_TRUE(1 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_2)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0xFFFFFFFFFFFFFFFDUL;

    carry =
        soft_bignum_sub(&in_a, &in_b, &out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(0 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_2_with_carry)
{
    uint64_t carry = 0;

    static const uint64_t in_a = 1;
    static const uint64_t in_b = 0x00000000FFFFFFFFUL;
    uint64_t out = 0;
    static const uint64_t expected_out = 0xFFFFFFFF00000002;

    carry =
        soft_bignum_sub(&in_a, &in_b, &out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == carry);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_5)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_a[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000EFFFFFFFUL};
    static const uint64_t in_b[3] = {1, 0, 0};
    uint64_t out[4] = {0, 0, 0, 0xFFFFFFFFFFFFFFFFUL};
    static const uint64_t expected_out[4] = {
        0xFFFFFFFFFFFFFFFEUL, 0xFFFFFFFFFFFFFFFFUL, 0x00000000EFFFFFFFUL,
        0xFFFFFFFFFFFFFFFFUL};

    carry = soft_bignum_sub(in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(0 == carry);
    result = memcmp(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_sub_size_5_with_carry)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_a[3] = {1, 0, 0};
    static const uint64_t in_b[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000FFFFFFFFUL};

    uint64_t out[3] = {0};
    static const uint64_t expected_out[3] = {
        0x0000000000000002, 0x0000000000000000UL, 0x0000000000000000UL};

    carry = soft_bignum_sub(in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(1 == carry);
    result = memcmp(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_sub_size_5_with_carry_2)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t in_a[3] = {0, 2, 0};
    static const uint64_t in_b[3] = {1, 0xFF, 0};

    uint64_t out[3] = {0};
    static const uint64_t expected_out[3] = {
        0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFF02UL, 0x00000000FFFFFFFFUL};

    carry = soft_bignum_sub(in_a, in_b, out, 5);

    TEST_ASSERT_TRUE(1 == carry);
    result = memcmp(expected_out, out, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}
