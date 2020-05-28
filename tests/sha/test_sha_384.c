#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(test_sha_384);

TEST_SETUP(test_sha_384)
{
}

TEST_TEAR_DOWN(test_sha_384)
{
}

TEST(test_sha_384, success) {
    TEST_ASSERT_TRUE(1 == 1);
}

TEST(test_sha_384, fail) {
    //more test stuff
    TEST_ASSERT_TRUE(0 == 1);
}
