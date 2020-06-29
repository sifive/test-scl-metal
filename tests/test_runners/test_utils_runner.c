#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(utils)
{
    RUN_TEST_CASE(utils, memcpy_u64_success);
    RUN_TEST_CASE(utils, memset_u64_success);
}
