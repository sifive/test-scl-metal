#include "unity.h"
#include "unity_fixture.h"

#include <string.h>

#include <api/hardware/scl_hca.h>
#include <api/scl_api.h>
#include <api/blockcipher/aes/aes.h>

static const metal_scl_t scl = {
    .hca_base = METAL_SIFIVE_HCA_0_BASE_ADDRESS,
    .aes_func = {
        .setkey = hca_aes_setkey,
        .setiv  = hca_aes_setiv,
        .cipher = hca_aes_cipher,
        .auth   = hca_aes_auth
    }
};

TEST_GROUP(hca_aes_128);

TEST_SETUP(hca_aes_128) {}

TEST_TEAR_DOWN(hca_aes_128) {}

TEST(hca_aes_128, ecb_F_1_12)
{
    /* NIST[nistspecialpublication800-38a.pdf] 
     * F.1.1 ECB-AES128.Encrypt
     * F.1.2 ECB-AES128.Decrypt
     * key: 2b7e151628aed2a6abf7158809cf4f3c
     * Plaintext: 
     *     block1 = 6bc1bee22e409f96e93d7e117393172a
     *     block2 = ae2d8a571e03ac9c9eb76fac45af8e51
     *     block3 = 30c81c46a35ce411e5fbc1191a0a52ef
     *     block4 = f69f2445df4f9b17ad2b417be66c3710
     * Ciphertext: 
     *     block1 = 3ad77bb40d7a3660a89ecaf32466ef97
     *     block2 = f5d3d58503b9699de785895a96fdbaaf
     *     block3 = 43b1cd7f598ece23881b00e3ed030688
     *     block4 = 7b0c785e27e8ad3f8223207104725dd4
     */
    const uint64_t key128[4] = {
        0,
        0,
        0xabf7158809cf4f3c,
        0x2b7e151628aed2a6
    };

    const uint8_t plaintext_be[64] __attribute__ ((aligned (8))) = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
    };

    const uint8_t ciphertext_be[64] __attribute__ ((aligned (8))) = {
        0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97,
        0xf5, 0xd3, 0xd5, 0x85, 0x03, 0xb9, 0x69, 0x9d, 0xe7, 0x85, 0x89, 0x5a, 0x96, 0xfd, 0xba, 0xaf,
        0x43, 0xb1, 0xcd, 0x7f, 0x59, 0x8e, 0xce, 0x23, 0x88, 0x1b, 0x00, 0xe3, 0xed, 0x03, 0x06, 0x88,
        0x7b, 0x0c, 0x78, 0x5e, 0x27, 0xe8, 0xad, 0x3f, 0x82, 0x23, 0x20, 0x71, 0x04, 0x72, 0x5d, 0xd4
    };

    uint8_t tmp[64] __attribute__ ((aligned (8))) = {0};
    int32_t result = 0;

    result = hca_aes_setkey(&scl, SCL_AES_KEY128, key128, SCL_ENCRYPT);
    TEST_ASSERT_TRUE(SCL_OK == result);

    /* F.1.1 ECB-AES128.Encrypt */
    result = hca_aes_cipher(&scl, SCL_AES_ECB, SCL_ENCRYPT, SCL_BIG_ENDIAN_MODE, sizeof(plaintext_be), plaintext_be, tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(ciphertext_be, tmp, sizeof(ciphertext_be)));

    memset(tmp,0,sizeof(tmp));
    /* F.1.2 ECB-AES128.Decrypt */
    result = hca_aes_cipher(&scl, SCL_AES_ECB, SCL_DECRYPT, SCL_BIG_ENDIAN_MODE, sizeof(ciphertext_be), ciphertext_be, tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(plaintext_be, tmp, sizeof(plaintext_be)));
}

TEST(hca_aes_128, ecb_F_1_12_not_aligned)
{
    /* NIST[nistspecialpublication800-38a.pdf] 
     * F.1.1 ECB-AES128.Encrypt
     * F.1.2 ECB-AES128.Decrypt
     * key: 2b7e151628aed2a6abf7158809cf4f3c
     * Plaintext: 
     *     block1 = 6bc1bee22e409f96e93d7e117393172a
     *     block2 = ae2d8a571e03ac9c9eb76fac45af8e51
     *     block3 = 30c81c46a35ce411e5fbc1191a0a52ef
     *     block4 = f69f2445df4f9b17ad2b417be66c3710
     * Ciphertext: 
     *     block1 = 3ad77bb40d7a3660a89ecaf32466ef97
     *     block2 = f5d3d58503b9699de785895a96fdbaaf
     *     block3 = 43b1cd7f598ece23881b00e3ed030688
     *     block4 = 7b0c785e27e8ad3f8223207104725dd4
     */
    const uint64_t key128[4] = {
        0,
        0,
        0xabf7158809cf4f3c,
        0x2b7e151628aed2a6
    };

    const uint8_t plaintext_be[65] __attribute__ ((aligned (8))) = {
        0x00,
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
    };

    const uint8_t ciphertext_be[65] __attribute__ ((aligned (8))) = {
        0x00,
        0x3a, 0xd7, 0x7b, 0xb4, 0x0d, 0x7a, 0x36, 0x60, 0xa8, 0x9e, 0xca, 0xf3, 0x24, 0x66, 0xef, 0x97,
        0xf5, 0xd3, 0xd5, 0x85, 0x03, 0xb9, 0x69, 0x9d, 0xe7, 0x85, 0x89, 0x5a, 0x96, 0xfd, 0xba, 0xaf,
        0x43, 0xb1, 0xcd, 0x7f, 0x59, 0x8e, 0xce, 0x23, 0x88, 0x1b, 0x00, 0xe3, 0xed, 0x03, 0x06, 0x88,
        0x7b, 0x0c, 0x78, 0x5e, 0x27, 0xe8, 0xad, 0x3f, 0x82, 0x23, 0x20, 0x71, 0x04, 0x72, 0x5d, 0xd4
    };

    uint8_t tmp[64] __attribute__ ((aligned (8))) = {0};
    int32_t result = 0;

    result = hca_aes_setkey(&scl, SCL_AES_KEY128, key128, SCL_ENCRYPT);
    TEST_ASSERT_TRUE(SCL_OK == result);

    /* F.1.1 ECB-AES128.Encrypt */
    result = hca_aes_cipher(&scl, SCL_AES_ECB, SCL_ENCRYPT, SCL_BIG_ENDIAN_MODE, sizeof(plaintext_be)-1, &plaintext_be[1], tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(&ciphertext_be[1], tmp, sizeof(ciphertext_be)-1));

    memset(tmp,0,sizeof(tmp));
    /* F.1.2 ECB-AES128.Decrypt */
    result = hca_aes_cipher(&scl, SCL_AES_ECB, SCL_DECRYPT, SCL_BIG_ENDIAN_MODE, sizeof(ciphertext_be)-1, &ciphertext_be[1], tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(&plaintext_be[1], tmp, sizeof(plaintext_be)-1));
}

TEST(hca_aes_128, cbc_F_2_12)
{
    /* NIST[nistspecialpublication800-38a.pdf] 
     * F.2.1 CBC-AES128.Encrypt
     * F.2.2 CBC-AES128.Decrypt
     * key: 2b7e151628aed2a6abf7158809cf4f3c
     * IV:  000102030405060708090a0b0c0d0e0f
     * Plaintext: 
     *     block1 = 6bc1bee22e409f96e93d7e117393172a
     *     block2 = ae2d8a571e03ac9c9eb76fac45af8e51
     *     block3 = 30c81c46a35ce411e5fbc1191a0a52ef
     *     block4 = f69f2445df4f9b17ad2b417be66c3710
     * Ciphertext: 
     *     block1 = 7649abac8119b246cee98e9b12e9197d
     *     block2 = 5086cb9b507219ee95db113a917678b2
     *     block3 = 73bed6b8e3c1743b7116e69e22229516
     *     block4 = 3ff1caa1681fac09120eca307586e1a7
     */
    const uint64_t key128[4] = {
        0,
        0,
        0xabf7158809cf4f3c,
        0x2b7e151628aed2a6
    };

    const uint64_t IV[2] = {
        0x08090a0b0c0d0e0f, 0x0001020304050607
    };

    const uint8_t plaintext_be[64] __attribute__ ((aligned (8))) = {
        0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
        0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
        0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
        0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
    };

    const uint8_t ciphertext_be[64] __attribute__ ((aligned (8))) = {
        0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
        0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
        0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
        0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
    };

    uint8_t tmp[64] __attribute__ ((aligned (8))) = {0};
    int32_t result = 0;

    result = hca_aes_setkey(&scl, SCL_AES_KEY128, key128, SCL_ENCRYPT);
    TEST_ASSERT_TRUE(SCL_OK == result);

    result = hca_aes_setiv(&scl, IV);
    TEST_ASSERT_TRUE(SCL_OK == result);

    /* F.2.1 CBC-AES128.Encrypt */
    result = hca_aes_cipher(&scl, SCL_AES_CBC, SCL_ENCRYPT, SCL_BIG_ENDIAN_MODE, sizeof(plaintext_be), plaintext_be, tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(ciphertext_be, tmp, sizeof(ciphertext_be)));

    /* F.2.2 CBC-AES128.Decrypt */
    memset(tmp,0,sizeof(tmp));
    result = hca_aes_cipher(&scl, SCL_AES_CBC, SCL_DECRYPT, SCL_BIG_ENDIAN_MODE, sizeof(ciphertext_be), ciphertext_be, tmp);
    TEST_ASSERT_TRUE(SCL_OK == result);
    TEST_ASSERT_TRUE(0 == memcmp(plaintext_be, tmp, sizeof(plaintext_be)));
}
