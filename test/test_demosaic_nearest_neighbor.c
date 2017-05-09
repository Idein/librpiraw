#include <rpiraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/CUnit.h>

static const unsigned width = 50, height = 50;

static void test_bggr_b_all_255()
{
    uint8_t *dst, *src;
    unsigned i, j;
    int ret = 0;

    src = malloc(width * height);
    CU_ASSERT_PTR_NOT_NULL_FATAL(src);
    dst = malloc(width * height * 3);
    if (dst == NULL) {
        free(src);
        CU_ASSERT_PTR_NOT_NULL_FATAL(dst);
    }

    memset(src, 0, width * height);
    memset(dst, 0, width * height * 3);
    for (i = 0; i < height; i += 2)
        for (j = 0; j < width; j += 2)
            src[i * width + j] = 255;

    ret = rpiraw_raw8bggr_to_rgb888_nearest_neighbor(dst, width, src, width,
                                                     width, height);
    CU_ASSERT_EQUAL(ret, 0);
    if (ret != 0) {
        free(dst);
        free(src);
        CU_ASSERT_EQUAL_FATAL(ret, 0);
    }

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 0], 0);
            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1], 0);
            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 2], 255);
        }
    }

    free(dst);
    free(src);
}

static void test_bggr_gb_all_255()
{
    uint8_t *dst, *src;
    unsigned i, j;
    int ret = 0;

    src = malloc(width * height);
    CU_ASSERT_PTR_NOT_NULL_FATAL(src);
    dst = malloc(width * height * 3);
    if (dst == NULL) {
        free(src);
        CU_ASSERT_PTR_NOT_NULL_FATAL(dst);
    }

    memset(src, 0, width * height);
    memset(dst, 0, width * height * 3);
    for (i = 0; i < height; i += 2)
        for (j = 1; j < width; j += 2)
            src[i * width + j] = 255;

    ret = rpiraw_raw8bggr_to_rgb888_nearest_neighbor(dst, width, src, width,
                                                     width, height);
    CU_ASSERT_EQUAL(ret, 0);
    if (ret != 0) {
        free(dst);
        free(src);
        CU_ASSERT_EQUAL_FATAL(ret, 0);
    }

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 0], 0);

            if (i % 2 == 0 && j % 2 == 0) { /* G at B */
                uint16_t sum = 255 * 2;
                uint8_t count = 4;
                if (j == 0) {
                    sum -= 255;
                    count --;
                }
                if (i == 0)
                    count --;
                if (j == width - 1) {
                    sum -= 255;
                    count --;
                }
                if (i == height - 1)
                    count --;
                CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1],
                                count ? sum / count : 0);
            } else if (i % 2 == 1 && j % 2 == 1) { /* G at R */
                uint16_t sum = 255 * 2;
                uint8_t count = 4;
                if (j == 0)
                    count --;
                if (i == 0) {
                    sum -= 255;
                    count --;
                }
                if (j == width - 1)
                    count --;
                if (i == height - 1) {
                    sum -= 255;
                    count --;
                }
                CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1],
                                count ? sum / count : 0);
            } else if (i % 2 == 0 && j % 2 == 1) { /* G at GB */
                uint8_t count = 5;
                if (i == 0 || j == 0)
                    count --;
                if (i == 0 || j == width - 1)
                    count --;
                if (i == height - 1 || j == 0)
                    count --;
                if (i == height - 1 || j == width - 1)
                    count --;
                CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1], 255 / count);
            } else { /* G at GR */
                uint16_t sum = 255 * 4;
                uint8_t count = 5;
                if (i == 0 || j == 0) {
                    sum -= 255;
                    count --;
                }
                if (i == 0 || j == width - 1) {
                    sum -= 255;
                    count --;
                }
                if (i == height - 1 || j == 0) {
                    sum -= 255;
                    count --;
                }
                if (i == height - 1 || j == width - 1) {
                    sum -= 255;
                    count --;
                }
                CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1], sum / count);
            }

            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 2], 0);
        }
    }

    free(dst);
    free(src);
}

static void test_bggr_gr_all_255()
{
    uint8_t *dst, *src;
    unsigned i, j;
    int ret = 0;

    src = malloc(width * height);
    CU_ASSERT_PTR_NOT_NULL_FATAL(src);
    dst = malloc(width * height * 3);
    if (dst == NULL) {
        free(src);
        CU_ASSERT_PTR_NOT_NULL_FATAL(dst);
    }

    memset(src, 0, width * height);
    memset(dst, 0, width * height * 3);
    for (i = 1; i < height; i += 2)
        for (j = 0; j < width; j += 2)
            src[i * width + j] = 255;

    ret = rpiraw_raw8bggr_to_rgb888_nearest_neighbor(dst, width, src, width,
                                                     width, height);
    CU_ASSERT_EQUAL(ret, 0);
    if (ret != 0) {
        free(dst);
        free(src);
        CU_ASSERT_EQUAL_FATAL(ret, 0);
    }

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 0], 0);

            if (i % 2 == 0 && j % 2 == 0) { /* G at B */
                uint16_t sum = 255 * 2;
                uint8_t count = 4;
                if (j == 0)
                    count --;
                if (i == 0) {
                    sum -= 255;
                    count --;
                }
                if (j == width - 1)
                    count --;
                if (i == height - 1) {
                    sum -= 255;
                    count --;
                }
                CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1],
                                count ? sum / count : 0);
            } else if (i % 2 == 1 && j % 2 == 1) { /* G at R */
                uint16_t sum = 255 * 2;
                uint8_t count = 4;
                if (j == 0) {
                    sum -= 255;
                    count --;
                }
                if (i == 0)
                    count --;
                if (j == width - 1) {
                    sum -= 255;
                    count --;
                }
                if (i == height - 1)
                    count --;
                CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1],
                                count ? sum / count : 0);
            } else if (i % 2 == 0 && j % 2 == 1) { /* G at GB */
                uint16_t sum = 255 * 4;
                uint8_t count = 5;
                if (i == 0 || j == 0) {
                    sum -= 255;
                    count --;
                }
                if (i == 0 || j == width - 1) {
                    sum -= 255;
                    count --;
                }
                if (i == height - 1 || j == 0) {
                    sum -= 255;
                    count --;
                }
                if (i == height - 1 || j == width - 1) {
                    sum -= 255;
                    count --;
                }
                CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1], sum / count);
            } else { /* G at GR */
                uint8_t count = 5;
                if (i == 0 || j == 0)
                    count --;
                if (i == 0 || j == width - 1)
                    count --;
                if (i == height - 1 || j == 0)
                    count --;
                if (i == height - 1 || j == width - 1)
                    count --;
                CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1], 255 / count);
            }

            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 2], 0);
        }
    }

    free(dst);
    free(src);
}

static void test_bggr_r_all_255()
{
    uint8_t *dst, *src;
    unsigned i, j;
    int ret = 0;

    src = malloc(width * height);
    CU_ASSERT_PTR_NOT_NULL_FATAL(src);
    dst = malloc(width * height * 3);
    if (dst == NULL) {
        free(src);
        CU_ASSERT_PTR_NOT_NULL_FATAL(dst);
    }

    memset(src, 0, width * height);
    memset(dst, 0, width * height * 3);
    for (i = 1; i < height; i += 2)
        for (j = 1; j < width; j += 2)
            src[i * width + j] = 255;

    ret = rpiraw_raw8bggr_to_rgb888_nearest_neighbor(dst, width, src, width,
                                                     width, height);
    CU_ASSERT_EQUAL(ret, 0);
    if (ret != 0) {
        free(dst);
        free(src);
        CU_ASSERT_EQUAL_FATAL(ret, 0);
    }

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 0], 255);
            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 1], 0);
            CU_ASSERT_EQUAL(dst[i * width * 3 + j * 3 + 2], 0);
        }
    }

    free(dst);
    free(src);
}

int suite_demosaic_nearest_neighbor()
{
    CU_pSuite suite = CU_add_suite("demosaic_nearest_neighbor", NULL, NULL);
    if (suite == NULL)
        return 1;

    if (CU_add_test(suite, "bggr: b: all 255",
                    test_bggr_b_all_255 ) == NULL)
        return 1;
    if (CU_add_test(suite, "bggr: gb: all 255",
                    test_bggr_gb_all_255) == NULL)
        return 1;
    if (CU_add_test(suite, "bggr: gr: all 255",
                    test_bggr_gr_all_255) == NULL)
        return 1;
    if (CU_add_test(suite, "bggr: r: all 255",
                    test_bggr_r_all_255 ) == NULL)
        return 1;

    return 0;
}
