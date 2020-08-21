#include "unity_fixture.h"

#include <stdint.h>
#include <stdlib.h>

#include <api/hardware/scl_hca.h>
#include <metal/machine/platform.h>

#if UINT32_MAX == UINTPTR_MAX
#define STACK_CHK_GUARD 0xe2dee396
#else
#define STACK_CHK_GUARD 0x595e9fbd94fda766
#endif

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

void __stack_chk_fail(void) { TEST_FAIL_MESSAGE("Stack smashing detected"); }

static void RunAllTests(void)
{
    UnityFixture.Verbose = 1;
    //UnityFixture.GroupFilter = "hca_aes_128";
    //UnityFixture.NameFilter = "ccm_4b";

    // soft implementation
# ifdef TEST_SOFT_SHA
    RUN_TEST_GROUP(soft_sha_224);
    RUN_TEST_GROUP(soft_sha_256);
    RUN_TEST_GROUP(soft_sha_384);
    RUN_TEST_GROUP(soft_sha_512);
# endif // TEST_SOFT_SHA

    // scl api implementation
# ifdef TEST_SCL_SOFT_SHA
    RUN_TEST_GROUP(scl_soft_sha_224);
    RUN_TEST_GROUP(scl_soft_sha_256);
    RUN_TEST_GROUP(scl_soft_sha_384);
    RUN_TEST_GROUP(scl_soft_sha_512);
# endif // TEST_SCL_SOFT_SHA

# ifdef TEST_SOFT_BIGNUM
    // utils
    RUN_TEST_GROUP(utils);

    // software bignumbers
    RUN_TEST_GROUP(soft_bignumbers);
# endif // TEST_SOFT_BIGNUM

#if METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0)
# ifdef TEST_HCA_SHA
    // hardware implementation
    RUN_TEST_GROUP(hca_sha_224);
    RUN_TEST_GROUP(hca_sha_256);
    RUN_TEST_GROUP(hca_sha_384);
    RUN_TEST_GROUP(hca_sha_512);
# endif // TEST_HCA_SHA

    // hardware implementation
# ifdef TEST_HCA_AES
    RUN_TEST_GROUP(hca_aes_128);
    RUN_TEST_GROUP(hca_aes_192);
    RUN_TEST_GROUP(hca_aes_256);
# endif // TEST_HCA_AES

    // scl api implementation
# ifdef TEST_SCL_AES
    RUN_TEST_GROUP(scl_aes_128);
    RUN_TEST_GROUP(scl_aes_192);
    RUN_TEST_GROUP(scl_aes_256);
# endif // TEST_SCL_AES
#endif
}

int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}
