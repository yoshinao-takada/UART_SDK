#include    <stdio.h>
#include    <stdlib.h>

int test_buffer();
int test_array1D();
int test_UASDK_string();
int test_UASDK_i16array();
int test_UASDK_r32array();

int main(int argc, char* *argv)
{
    static const char* result_label[] = { "SUCCESS", "FAILURE" };
    int result_all = EXIT_SUCCESS;
    int result_each = EXIT_FAILURE;
    result_all |= (result_each = test_buffer());
    fprintf(stdout, "test_buffer(): %s\n", result_label[result_each]);
    result_all |= (result_each = test_array1D());
    fprintf(stdout, "test_array1D(): %s\n", result_label[result_each]);
    result_all |= (result_each = test_UASDK_string());
    fprintf(stdout, "test_UASDK_string(): %s\n", result_label[result_each]);
    result_all |= (result_each = test_UASDK_i16array());
    fprintf(stdout, "test_UASDK_i16array(): %s\n", result_label[result_each]);
    result_all |= (result_each = test_UASDK_r32array());
    fprintf(stdout, "test_UASDK_r32array(): %s\n", result_label[result_each]);
    return 0;
}