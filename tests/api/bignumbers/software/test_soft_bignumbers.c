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

/* Increment by one */
TEST(soft_bignumbers, soft_bignum_inc_size_0)
{
    uint64_t carry = 0;
    uint64_t in = 0;
    static const uint64_t expected_out = 0;

    carry = soft_bignum_inc(&in, 0);

    TEST_ASSERT_TRUE(0 == carry);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_1)
{
    uint64_t carry = 0;
    uint64_t in = 0x00000000FFFFFFFEUL;
    static const uint64_t expected_out = 0x00000000FFFFFFFFUL;

    carry = soft_bignum_inc(&in, 1);

    TEST_ASSERT_TRUE(0 == carry);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_1_with_carry)
{
    uint64_t carry = 0;
    uint64_t in = 0x00000000FFFFFFFFUL;
    static const uint64_t expected_out = 0;

    carry = soft_bignum_inc(&in, 1);

    TEST_ASSERT_TRUE(1 == carry);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_2)
{
    uint64_t carry = 0;
    uint64_t in = 0x00000000FFFFFFFFUL;
    static const uint64_t expected_out = 0x0000000100000000UL;

    carry =
        soft_bignum_inc(&in, sizeof(in) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(0 == carry);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_2_with_carry)
{
    uint64_t carry = 0;
    uint64_t in = 0xFFFFFFFFFFFFFFFFUL;
    static const uint64_t expected_out = 0;

    carry =
        soft_bignum_inc(&in, sizeof(in) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(1 == carry);
    TEST_ASSERT_TRUE(expected_out == in);
}

TEST(soft_bignumbers, soft_bignum_inc_size_5)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
    uint64_t in[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000EFFFFFFFUL};
    static const uint64_t expected_out[3] = {0, 0, 0x00000000F0000000UL};

    carry = soft_bignum_inc(in, 5);

    TEST_ASSERT_TRUE(0 == carry);
    result = memcmp(expected_out, in, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}

TEST(soft_bignumbers, soft_bignum_inc_size_5_with_carry)
{
    size_t result = 0;
    uint64_t carry = 0xFFFFFFFFFFFFFFFFUL;
    uint64_t in[3] = {0xFFFFFFFFFFFFFFFFUL, 0xFFFFFFFFFFFFFFFFUL,
                                     0x00000000FFFFFFFFUL};
    static const uint64_t expected_out[3] = {0, 0, 0};

    carry = soft_bignum_inc(in, 5);

    TEST_ASSERT_TRUE(1 == carry);
    result = memcmp(expected_out, in, sizeof(expected_out));
    TEST_ASSERT_TRUE(0 == result);
}

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

    word_size = sizeof(b) / sizeof(*b);

    result = soft_bignum_compare(a, b, word_size);

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

    word_size = sizeof(b) / sizeof(*b);

    result = soft_bignum_compare(a, b, word_size);

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

    word_size = sizeof(b) / sizeof(*b);

    result = soft_bignum_compare(a, b, word_size);

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

    word_size = sizeof(b) / sizeof(*b);

    result = soft_bignum_compare(a, b, word_size);

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

    word_size = sizeof(b) / sizeof(*b);

    result = soft_bignum_compare(a, b, word_size);

    TEST_ASSERT_TRUE(-1 == result);
}
