#include "unity.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_success(void) {
    TEST_ASSERT_TRUE(1 == 1)
}

void test_fail(void) {
    //more test stuff
    TEST_ASSERT_TRUE(0 == 1)
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_success);
    RUN_TEST(test_fail);
    return UNITY_END();
}