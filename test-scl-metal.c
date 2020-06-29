#include "unity_fixture.h"

#include <metal/machine/platform.h>
#include <api/hardware/scl_hca.h>

static void RunAllTests(void)
{
    // soft implementation
    RUN_TEST_GROUP(soft_sha_224);
    RUN_TEST_GROUP(soft_sha_256);
    RUN_TEST_GROUP(soft_sha_384);
    RUN_TEST_GROUP(soft_sha_512);

    // scl api implementation
    RUN_TEST_GROUP(scl_soft_sha_224);
    RUN_TEST_GROUP(scl_soft_sha_256);
    RUN_TEST_GROUP(scl_soft_sha_384);
    RUN_TEST_GROUP(scl_soft_sha_512);

    // utils
    RUN_TEST_GROUP(utils);

    // software bignumbers
    RUN_TEST_GROUP(soft_bignumbers);


#if METAL_SIFIVE_HCA_VERSION >= HCA_VERSION(0, 5, 0)
    // hardware implementation
    RUN_TEST_GROUP(hca_sha_224);
    RUN_TEST_GROUP(hca_sha_256);
    RUN_TEST_GROUP(hca_sha_384);
    RUN_TEST_GROUP(hca_sha_512);

    // hardware implementation
    RUN_TEST_GROUP(hca_aes_128);
    RUN_TEST_GROUP(hca_aes_192);
    RUN_TEST_GROUP(hca_aes_256);

    // scl api implementation
    RUN_TEST_GROUP(scl_aes_128);
    RUN_TEST_GROUP(scl_aes_192);
    RUN_TEST_GROUP(scl_aes_256);
#endif

}

int main(int argc, const char *argv[])
{
    return UnityMain(argc, argv, RunAllTests);
}
