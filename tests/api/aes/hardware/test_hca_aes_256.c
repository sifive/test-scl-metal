#include "unity.h"
#include "unity_fixture.h"

#include <string.h>

#include <api/blockcipher/aes/aes.h>
#include <api/hardware/scl_hca.h>
#include <api/scl_api.h>

static const metal_scl_t scl = {.hca_base = METAL_SIFIVE_HCA_0_BASE_ADDRESS,
                                .aes_func = {.setkey = hca_aes_setkey,
                                             .setiv = hca_aes_setiv,
                                             .cipher = hca_aes_cipher,
                                             .auth = hca_aes_auth}};

TEST_GROUP(hca_aes_256);

TEST_SETUP(hca_aes_256) {}

TEST_TEAR_DOWN(hca_aes_256) {}

TEST(hca_aes_256, ecb_F_1_56)
{
    /* NIST[nistspecialpublication800-38a.pdf]
     * F.1.5 ECB-AES256.Encrypt
     * F.1.6 ECB-AES256.Decrypt
     * key: 603deb1015ca71be 2b73aef0857d7781 1f352c073b6108d7 2d9810a30914dff4
     * Plaintext:
     *     block1 = 6bc1bee22e409f96e93d7e117393172a
     *     block2 = ae2d8a571e03ac9c9eb76fac45af8e51
     *     block3 = 30c81c46a35ce411e5fbc1191a0a52ef
     *     block4 = f69f2445df4f9b17ad2b417be66c3710
     * Ciphertext:
     *     block1 = f3eed1bdb5d2a03c064b5a7e3db181f8
     *     block2 = 591ccb10d410ed26dc5ba74a31362870
     *     block3 = b6ed21b99ca6f4f9f153e7b1beafed1d
     *     block4 = 23304b7a39f9f3ff067d8d8f9e24ecc7
     */
    static const uint64_t key256[4] = {0x2d9810a30914dff4, 0x1f352c073b6108d7,
                                0x2b73aef0857d7781, 0x603deb1015ca71be};

    static const uint8_t plaintext_be[64] __attribute__((aligned(8))) = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e,
        0x11, 0x73, 0x93, 0x17, 0x2a, 0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03,
        0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51, 0x30,
        0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19,
        0x1a, 0x0a, 0x52, 0xef, 0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b,
        0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};

    static const uint8_t ciphertext_be[64] __attribute__((aligned(8))) = {
        0xf3, 0xee, 0xd1, 0xbd, 0xb5, 0xd2, 0xa0, 0x3c, 0x06, 0x4b, 0x5a,
        0x7e, 0x3d, 0xb1, 0x81, 0xf8, 0x59, 0x1c, 0xcb, 0x10, 0xd4, 0x10,
        0xed, 0x26, 0xdc, 0x5b, 0xa7, 0x4a, 0x31, 0x36, 0x28, 0x70, 0xb6,
        0xed, 0x21, 0xb9, 0x9c, 0xa6, 0xf4, 0xf9, 0xf1, 0x53, 0xe7, 0xb1,
        0xbe, 0xaf, 0xed, 0x1d, 0x23, 0x30, 0x4b, 0x7a, 0x39, 0xf9, 0xf3,
        0xff, 0x06, 0x7d, 0x8d, 0x8f, 0x9e, 0x24, 0xec, 0xc7};

    uint8_t tmp[64] __attribute__((aligned(8))) = {0};
    int32_t result = 0;

    result = hca_aes_setkey(&scl, SCL_AES_KEY256, key256, SCL_ENCRYPT);
    TEST_ASSERT_TRUE(SCL_OK == result);

    /* F.1.5 ECB-AES256.Encrypt */
    result = hca_aes_cipher(&scl, SCL_AES_ECB, SCL_ENCRYPT, SCL_BIG_ENDIAN_MODE,
                            plaintext_be, sizeof(plaintext_be), tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(ciphertext_be, tmp, sizeof(ciphertext_be)));

    memset(tmp, 0, sizeof(tmp));
    /* F.1.6 ECB-AES256.Decrypt */
    result = hca_aes_cipher(&scl, SCL_AES_ECB, SCL_DECRYPT, SCL_BIG_ENDIAN_MODE,
                            ciphertext_be, sizeof(ciphertext_be), tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(plaintext_be, tmp, sizeof(plaintext_be)));
}

TEST(hca_aes_256, cbc_F_2_56)
{
    /* NIST[nistspecialpublication800-38a.pdf]
     * F.2.5 CBC-AES256.Encrypt
     * F.2.6 CBC-AES256.Decrypt
     * key: 603deb1015ca71be 2b73aef0857d7781 1f352c073b6108d7 2d9810a30914dff4
     * IV:  000102030405060708090a0b0c0d0e0f
     * Plaintext:
     *     block1 = 6bc1bee22e409f96e93d7e117393172a
     *     block2 = ae2d8a571e03ac9c9eb76fac45af8e51
     *     block3 = 30c81c46a35ce411e5fbc1191a0a52ef
     *     block4 = f69f2445df4f9b17ad2b417be66c3710
     * Ciphertext:
     *     block1 = f58c4c04d6e5f1ba779eabfb5f7bfbd6
     *     block2 = 9cfc4e967edb808d679f777bc6702c7d
     *     block3 = 39f23369a9d9bacfa530e26304231461
     *     block4 = b2eb05e2c39be9fcda6c19078c6a9d1b
     */
    static const uint64_t key256[4] = {0x2d9810a30914dff4, 0x1f352c073b6108d7,
                                0x2b73aef0857d7781, 0x603deb1015ca71be};

    static const uint64_t IV[2] = {0x08090a0b0c0d0e0f, 0x0001020304050607};

    static const uint8_t plaintext_be[64] __attribute__((aligned(8))) = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e,
        0x11, 0x73, 0x93, 0x17, 0x2a, 0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03,
        0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51, 0x30,
        0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19,
        0x1a, 0x0a, 0x52, 0xef, 0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b,
        0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};

    static const uint8_t ciphertext_be[64] __attribute__((aligned(8))) = {
        0xf5, 0x8c, 0x4c, 0x04, 0xd6, 0xe5, 0xf1, 0xba, 0x77, 0x9e, 0xab,
        0xfb, 0x5f, 0x7b, 0xfb, 0xd6, 0x9c, 0xfc, 0x4e, 0x96, 0x7e, 0xdb,
        0x80, 0x8d, 0x67, 0x9f, 0x77, 0x7b, 0xc6, 0x70, 0x2c, 0x7d, 0x39,
        0xf2, 0x33, 0x69, 0xa9, 0xd9, 0xba, 0xcf, 0xa5, 0x30, 0xe2, 0x63,
        0x04, 0x23, 0x14, 0x61, 0xb2, 0xeb, 0x05, 0xe2, 0xc3, 0x9b, 0xe9,
        0xfc, 0xda, 0x6c, 0x19, 0x07, 0x8c, 0x6a, 0x9d, 0x1b};

    uint8_t tmp[64] __attribute__((aligned(8))) = {0};
    int32_t result = 0;

    result = hca_aes_setkey(&scl, SCL_AES_KEY256, key256, SCL_ENCRYPT);
    TEST_ASSERT_TRUE(SCL_OK == result);

    result = hca_aes_setiv(&scl, IV);
    TEST_ASSERT_TRUE(SCL_OK == result);

    /* F.2.5 CBC-AES256.Encrypt */
    result = hca_aes_cipher(&scl, SCL_AES_CBC, SCL_ENCRYPT, SCL_BIG_ENDIAN_MODE,
                            plaintext_be, sizeof(plaintext_be), tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(ciphertext_be, tmp, sizeof(ciphertext_be)));

    /* F.2.6 CBC-AES256.Decrypt */
    memset(tmp, 0, sizeof(tmp));
    result = hca_aes_cipher(&scl, SCL_AES_CBC, SCL_DECRYPT, SCL_BIG_ENDIAN_MODE,
                            ciphertext_be, sizeof(ciphertext_be), tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(plaintext_be, tmp, sizeof(plaintext_be)));
}
