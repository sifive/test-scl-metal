#include "unity_fixture.h"

static void RunAllTests(void)
{
    RUN_TEST_GROUP(test_sha_224_soft);
    RUN_TEST_GROUP(test_sha_256_soft);
    RUN_TEST_GROUP(test_sha_384_soft);
    RUN_TEST_GROUP(test_sha_512_soft);
    // RUN_TEST_GROUP(test_sha_256);
    // RUN_TEST_GROUP(test_sha_384);
}

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
