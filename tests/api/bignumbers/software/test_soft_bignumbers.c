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

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_add_size_2)
{
    int32_t result = 0;

    static const uint64_t in_a = 0x00000000FFFFFFFFUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0x0000000100000000UL;

    result =
        soft_bignum_add(NULL, &in_a, &in_b, &out, sizeof(in_a) / sizeof(uint32_t));

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

    result =
        soft_bignum_add(NULL, &in_a, &in_b, &out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(SCL_OK == result);
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

    TEST_ASSERT_TRUE(SCL_OK == result);
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

    result = soft_bignum_add(NULL, in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

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
    static const uint64_t expected_out[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};

    result = soft_bignum_add(NULL, in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(SCL_OK == result);
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

    result = soft_bignum_add(NULL, in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
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

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(expected_out == out);
}

TEST(soft_bignumbers, soft_bignum_sub_size_2)
{
    int32_t result = 0;

    static const uint64_t in_a = 0xFFFFFFFFFFFFFFFEUL;
    static const uint64_t in_b = 1;
    uint64_t out = 0;
    static const uint64_t expected_out = 0xFFFFFFFFFFFFFFFDUL;

    result =
        soft_bignum_sub(NULL, &in_a, &in_b, &out, sizeof(in_a) / sizeof(uint32_t));

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

    result =
        soft_bignum_sub(NULL, &in_a, &in_b, &out, sizeof(in_a) / sizeof(uint32_t));

    TEST_ASSERT_TRUE(SCL_OK == result);
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

    TEST_ASSERT_TRUE(SCL_OK == result);
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

    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(expected_out, out, sizeof(expected_out));
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

    TEST_ASSERT_TRUE(SCL_OK == result);
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

    TEST_ASSERT_TRUE(SCL_OK == result);
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

    TEST_ASSERT_TRUE(SCL_OK == result);
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

    word_size = sizeof(b) / sizeof(*b);

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

    word_size = sizeof(b) / sizeof(*b);

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

    word_size = sizeof(b) / sizeof(*b);

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

    word_size = sizeof(b) / sizeof(*b);

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

    word_size = sizeof(b) / sizeof(*b);

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

    result = soft_bignum_mult(NULL, &in_a, &in_b, out, sizeof(in_a) / sizeof(uint32_t));

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

    result = soft_bignum_mult(NULL, in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

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

    result = soft_bignum_mult(NULL, in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

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

    result = soft_bignum_mult(NULL, in_a, in_b, out, sizeof(in_a) / sizeof(uint32_t));

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
