#include "unity_fixture.h"

static void RunAllTests(void)
{
    // soft implementation
    RUN_TEST_GROUP(test_sha_224_soft);
    RUN_TEST_GROUP(test_sha_256_soft);
    RUN_TEST_GROUP(test_sha_384_soft);
    RUN_TEST_GROUP(test_sha_512_soft);

    // hardware implementation
    RUN_TEST_GROUP(test_sha_224_hca);
    RUN_TEST_GROUP(test_sha_256_hca);
    RUN_TEST_GROUP(test_sha_384_hca);
    RUN_TEST_GROUP(test_sha_512_hca);

    // scl api implementation
    RUN_TEST_GROUP(test_sha_224_scl);
    RUN_TEST_GROUP(test_sha_256_scl);
    RUN_TEST_GROUP(test_sha_384_scl);
    RUN_TEST_GROUP(test_sha_512_scl);
}

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
