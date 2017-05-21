#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <stdint.h>
#include <unistd.h>

/* Howto: http://cunit.sourceforge.net/doc/introduction.html */

int suite_demosaic_nearest_neighbor();
int suite_histogram();
int suite_statistic();
int suite_luminance();
int suite_file_png();

int main()
{
    int ret = 0;

    CU_initialize_registry();

    ret = suite_demosaic_nearest_neighbor();
    if (ret)
        goto end;
    ret = suite_histogram();
    if (ret)
        goto end;
    ret = suite_statistic();
    if (ret)
        goto end;
    ret = suite_luminance();
    if (ret)
        goto end;
    ret = suite_file_png();
    if (ret)
        goto end;

    isatty(fileno(stdout)) ? CU_console_run_tests() : CU_basic_run_tests();
    ret = CU_get_number_of_failures();

    CU_cleanup_registry();

end:
    if (CU_get_error() != CUE_SUCCESS)
        fprintf(stderr, "error: CUnit: %s\n", CU_get_error_msg());
    else if (ret != 0)
        fprintf(stderr, "error: %d\n", ret);
    return !!ret;
}
