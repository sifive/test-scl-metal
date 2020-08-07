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
            .negate = soft_bignum_negate,
            .inc = soft_bignum_inc,
            .add = soft_bignum_add,
            .sub = soft_bignum_sub,
            .mult = soft_bignum_mult,
            .square = soft_bignum_square_with_mult,
            // .square = soft_bignum_square,
            .leftshift = soft_bignum_leftshift,
            .rightshift = soft_bignum_rightshift,
            .msb_set_in_word = soft_bignum_msb_set_in_word,
            .get_msb_set = soft_bignum_get_msb_set,
            .set_bit = soft_bignum_set_bit,
            .div = soft_bignum_div,
            .mod = soft_bignum_mod,
            .set_modulus = soft_bignum_set_modulus,
            .mod_add = soft_bignum_mod_add,
            .mod_sub = soft_bignum_mod_sub,
            .mod_mult = soft_bignum_mod_mult,
            .mod_inv = soft_bignum_mod_inv,
            .mod_square = soft_bignum_mod_square,
        },
};

TEST_GROUP(soft_ecc);

TEST_SETUP(soft_ecc) {}

TEST_TEAR_DOWN(soft_ecc) {}

TEST(soft_ecc, test_p384r1_affine_2_jacobian_2_affine)
{
    int32_t result = 0;

    uint64_t point_x[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0xE8239FC890BC6894, 0xC61EF3EF31AE0D87, 0xC5AD236CA59684BE,
        0xBABCE4EB75047E7A, 0xA75A8848900CC063, 0xA011B3C98CCC2720};

    uint64_t point_y[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0x96455AEE5D3E8912, 0xA0DA7CBF700931A6, 0xF8BDABFB61F95731,
        0x9A2708BE527AE3C9, 0x024C8F69D35F423E, 0xE03D61232DEDE896};

    static const uint64_t point_expected_x[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0xE8239FC890BC6894, 0xC61EF3EF31AE0D87, 0xC5AD236CA59684BE,
        0xBABCE4EB75047E7A, 0xA75A8848900CC063, 0xA011B3C98CCC2720};

    static const uint64_t point_expected_y[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0x96455AEE5D3E8912, 0xA0DA7CBF700931A6, 0xF8BDABFB61F95731,
        0x9A2708BE527AE3C9, 0x024C8F69D35F423E, 0xE03D61232DEDE896};

    uint64_t point_jac_x[ECC_SECP384R1_64B_WORDS_SIZE] = {0};
    uint64_t point_jac_y[ECC_SECP384R1_64B_WORDS_SIZE] = {0};
    uint64_t point_jac_z[ECC_SECP384R1_64B_WORDS_SIZE] = {0};

    ecc_bignum_affine_point_t point = {.x = point_x, .y = point_y};
    ecc_bignum_jacobian_point_t point_jac = {
        .x = point_jac_x, .y = point_jac_y, .z = point_jac_z};

    result = soft_ecc_convert_affine_to_jacobian(
        &scl, &ecc_secp384r1, &point, &point_jac, ECC_SECP384R1_32B_WORDS_SIZE);
    TEST_ASSERT_TRUE(SCL_OK == result);

    result = soft_ecc_convert_jacobian_to_affine(
        &scl, &ecc_secp384r1, &point_jac, &point, ECC_SECP384R1_32B_WORDS_SIZE);

    TEST_ASSERT_TRUE(SCL_OK == result);

    TEST_ASSERT_EQUAL_HEX8_ARRAY(point_expected_x, point_x,
                                 ECC_SECP384R1_BYTESIZE);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(point_expected_y, point_y,
                                 ECC_SECP384R1_BYTESIZE);
}

/* Addition */
TEST(soft_ecc, test_p384r1_double_affine_point_via_jacobian)
{
    int32_t result = 0;

    uint64_t point_x[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0xE8239FC890BC6894, 0xC61EF3EF31AE0D87, 0xC5AD236CA59684BE,
        0xBABCE4EB75047E7A, 0xA75A8848900CC063, 0xA011B3C98CCC2720};

    uint64_t point_y[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0x96455AEE5D3E8912, 0xA0DA7CBF700931A6, 0xF8BDABFB61F95731,
        0x9A2708BE527AE3C9, 0x024C8F69D35F423E, 0xE03D61232DEDE896};

    static const uint64_t point_expected_x[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0x19E63250FA210E7A, 0x5DD95D157FFA6D40, 0xBC442EEC637045B9,
        0x38D9EB89EBA8D821, 0x9F0B89A7F2352465, 0x2A70394A45A7F16A};

    static const uint64_t point_expected_y[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0x204D242FF2A87083, 0x743B7B7C2E90AF89, 0x4223F8C849D15897,
        0xA6F54667A5903978, 0x12E307875CD94CC1, 0x85E0719553E1310C};

    uint64_t point_jac_x[ECC_SECP384R1_64B_WORDS_SIZE] = {0};
    uint64_t point_jac_y[ECC_SECP384R1_64B_WORDS_SIZE] = {0};
    uint64_t point_jac_z[ECC_SECP384R1_64B_WORDS_SIZE] = {0};

    ecc_bignum_affine_point_t point = {.x = point_x, .y = point_y};
    ecc_bignum_jacobian_point_t point_jac = {
        .x = point_jac_x, .y = point_jac_y, .z = point_jac_z};

    result = soft_ecc_convert_affine_to_jacobian(
        &scl, &ecc_secp384r1, &point, &point_jac, ECC_SECP384R1_32B_WORDS_SIZE);
    TEST_ASSERT_TRUE(SCL_OK == result);

    result = soft_ecc_double_jacobian(&scl, &ecc_secp384r1, &point_jac,
                                      &point_jac, ECC_SECP384R1_32B_WORDS_SIZE);

    TEST_ASSERT_TRUE(SCL_OK == result);

    result = soft_ecc_convert_jacobian_to_affine(
        &scl, &ecc_secp384r1, &point_jac, &point, ECC_SECP384R1_32B_WORDS_SIZE);

    TEST_ASSERT_TRUE(SCL_OK == result);

    TEST_ASSERT_EQUAL_HEX8_ARRAY(point_expected_x, point_x,
                                 ECC_SECP384R1_BYTESIZE);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(point_expected_y, point_y,
                                 ECC_SECP384R1_BYTESIZE);
}

TEST(soft_ecc, test_p384r1_add_affine_point_via_jacobian)
{
    int32_t result = 0;

    uint64_t point_1_x[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0xE8239FC890BC6894, 0xC61EF3EF31AE0D87, 0xC5AD236CA59684BE,
        0xBABCE4EB75047E7A, 0xA75A8848900CC063, 0xA011B3C98CCC2720};

    uint64_t point_1_y[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0x96455AEE5D3E8912, 0xA0DA7CBF700931A6, 0xF8BDABFB61F95731,
        0x9A2708BE527AE3C9, 0x024C8F69D35F423E, 0xE03D61232DEDE896};

    uint64_t point_2_x[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0xBFAA1EB3B390DBFA, 0x62E5751DE484DDD2, 0xF16D54C2C6738D6E,
        0x7CE17409915FBB40, 0x7E4CEEB38FA9E747, 0x0F898007251672BC};

    uint64_t point_2_y[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0x0F5D52BED5693395, 0xE459A59E70947C50, 0x2DEE2BCBC649B425,
        0xCC61342E5F3060CD, 0x6BFDA3B7E1206C55, 0xA4A4F7F882FE209D};

    static const uint64_t point_expected_x[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0xF9AFDB62A269112C, 0x4F674222FC299B31, 0xF1D4A9A5A4092BC6,
        0x4B65085D26F1E2B9, 0xF3624D0FF7BDFF2C, 0xD8AC864C156ED992};

    static const uint64_t point_expected_y[ECC_SECP384R1_64B_WORDS_SIZE] = {
        0xBA14BD0E731F5BA3, 0xC67A07E21FFA99F7, 0x866DD917F44879D1,
        0x19D9705110670710, 0x1A87FDE0EAD01FBD, 0xD7DFBF6E1E5EAA0A};

    uint64_t point_1_jac_x[ECC_SECP384R1_64B_WORDS_SIZE] = {0};
    uint64_t point_1_jac_y[ECC_SECP384R1_64B_WORDS_SIZE] = {0};
    uint64_t point_1_jac_z[ECC_SECP384R1_64B_WORDS_SIZE] = {0};

    uint64_t point_2_jac_x[ECC_SECP384R1_64B_WORDS_SIZE] = {0};
    uint64_t point_2_jac_y[ECC_SECP384R1_64B_WORDS_SIZE] = {0};
    uint64_t point_2_jac_z[ECC_SECP384R1_64B_WORDS_SIZE] = {0};

    ecc_bignum_affine_point_t point_1 = {.x = point_1_x, .y = point_1_y};
    ecc_bignum_affine_point_t point_2 = {.x = point_2_x, .y = point_2_y};

    ecc_bignum_jacobian_point_t point_1_jac = {
        .x = point_1_jac_x, .y = point_1_jac_y, .z = point_1_jac_z};
    ecc_bignum_jacobian_point_t point_2_jac = {
        .x = point_2_jac_x, .y = point_2_jac_y, .z = point_2_jac_z};

    result = soft_ecc_convert_affine_to_jacobian(&scl, &ecc_secp384r1, &point_1,
                                                 &point_1_jac,
                                                 ECC_SECP384R1_32B_WORDS_SIZE);
    TEST_ASSERT_TRUE(SCL_OK == result);

    result = soft_ecc_convert_affine_to_jacobian(&scl, &ecc_secp384r1, &point_2,
                                                 &point_2_jac,
                                                 ECC_SECP384R1_32B_WORDS_SIZE);
    TEST_ASSERT_TRUE(SCL_OK == result);

    result = soft_ecc_add_jacobian_jacobian(&scl, &ecc_secp384r1, &point_1_jac,
                                            &point_2_jac, &point_1_jac,
                                            ECC_SECP384R1_32B_WORDS_SIZE);

    TEST_ASSERT_TRUE(SCL_OK == result);

    result = soft_ecc_convert_jacobian_to_affine(&scl, &ecc_secp384r1,
                                                 &point_1_jac, &point_1,
                                                 ECC_SECP384R1_32B_WORDS_SIZE);

    TEST_ASSERT_TRUE(SCL_OK == result);

    TEST_ASSERT_EQUAL_HEX8_ARRAY(point_expected_x, point_1_x,
                                 ECC_SECP384R1_BYTESIZE);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(point_expected_y, point_1_y,
                                 ECC_SECP384R1_BYTESIZE);
}
