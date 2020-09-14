#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP_RUNNER(soft_ecdsa)
{
    /* verification */

    /* SECP384r1 */
    RUN_TEST_CASE(soft_ecdsa, test_p384r1_curve_input_384B_verif_success);
    RUN_TEST_CASE(soft_ecdsa,
                  test_p384r1_curve_input_384B_verif_invalid_signature);

    /* SECP521r1 */
    RUN_TEST_CASE(soft_ecdsa, test_p521r1_curve_input_512B_verif_success);
    RUN_TEST_CASE(soft_ecdsa,
                  test_p521r1_curve_input_512B_verif_invalid_signature);

    /* signature */

    /* SECP384r1 */
    RUN_TEST_CASE(soft_ecdsa, test_p384r1_curve_input_384B_sign);
    /* SECP521r1 */
    RUN_TEST_CASE(soft_ecdsa, test_p521r1_curve_input_512B_sign);
}
