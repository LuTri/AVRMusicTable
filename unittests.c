#include <stdio.h>
#include <CUnit/Basic.h>

int main(void) {
	unsigned short failed;
    T_FUNC_DESCRIPTOR* testDescriptors = ALL_TEST_FUNCS;
    CU_pSuite pSuite = NULL;

    /* initialize the CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* add a suite to the registry */
    pSuite = CU_add_suite(SUITE_NAME, NULL, NULL);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    while (testDescriptors && testDescriptors->fnc) {
        if ((NULL == CU_add_test(pSuite,
                                 testDescriptors->label,
                                 testDescriptors->fnc)))
        {
            CU_cleanup_registry();
            return CU_get_error();
        }
        testDescriptors++;
    }

    /* Run all tests using the CUnit Basic interface */
    CU_basic_set_mode(CU_BRM_NORMAL);
    CU_basic_run_tests();
    failed = CU_get_number_of_failure_records();
    CU_cleanup_registry();
    return failed;
}
