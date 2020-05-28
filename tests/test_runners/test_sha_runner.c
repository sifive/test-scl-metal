#include "unity.h"
#include "unity_fixture.h"

// SHA 256
TEST_GROUP_RUNNER(test_sha_256)
{
  RUN_TEST_CASE(test_sha_256, success);
  RUN_TEST_CASE(test_sha_256, fail);
}

// SHA 384
TEST_GROUP_RUNNER(test_sha_384)
{
  RUN_TEST_CASE(test_sha_384, success);
  RUN_TEST_CASE(test_sha_384, fail);
}
