#include "unity_fixture.h"

static void RunAllTests(void)
{
  RUN_TEST_GROUP(test_sha_256);
  RUN_TEST_GROUP(test_sha_384);
}

int main(int argc, const char * argv[])
{
  return UnityMain(argc, argv, RunAllTests);
}
