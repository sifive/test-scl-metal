#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(soft_ecdsa)
{
    /* addition */
    RUN_TEST_CASE(soft_ecdsa, test_p384r1_curve_input_384B_valid);
    RUN_TEST_CASE(soft_ecdsa, test_p384r1_curve_input_384B_sign);
}
