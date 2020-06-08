#include "unity_fixture.h"

static void RunAllTests(void)
{
    // soft implementation
    RUN_TEST_GROUP(soft_test_sha_224);
    RUN_TEST_GROUP(soft_test_sha_256);
    RUN_TEST_GROUP(soft_test_sha_384);
    RUN_TEST_GROUP(soft_test_sha_512);

    // hardware implementation
    RUN_TEST_GROUP(hca_test_sha_224);
    RUN_TEST_GROUP(hca_test_sha_256);
    RUN_TEST_GROUP(hca_test_sha_384);
    RUN_TEST_GROUP(hca_test_sha_512);

    // scl api implementation
    RUN_TEST_GROUP(scl_test_sha_224);
    RUN_TEST_GROUP(scl_test_sha_256);
    RUN_TEST_GROUP(scl_test_sha_384);
    RUN_TEST_GROUP(scl_test_sha_512);
}

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
