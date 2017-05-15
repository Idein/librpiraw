/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include <rpiraw.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/time.h>
#include <CUnit/CUnit.h>

static const unsigned width = 50, height = 50;

static void seed_random()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    srandom(t.tv_sec ^ t.tv_usec);
}

static void test_bt709_rgb888_ld_dst()
{
    uint8_t *src = NULL, *dst = NULL;
    const unsigned ld_dst = width + 7;
    unsigned i, j;

    src = malloc(height * width * 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(src);

    dst = malloc(ld_dst * width);
    CU_ASSERT_PTR_NOT_NULL(dst);
    if (dst == NULL) {
        free(src);
        CU_ASSERT_PTR_NOT_NULL_FATAL(dst);
    }

    /*
     * The src contents are clamped to [0, 255).
     * So dst contents must be less than 255 since
     * 0.2126 * 254 + 0.7152 * 254 + 0.0722 * 254 = 254.0
     */
    seed_random();
    for (i = 0; i < height * width * 3; i ++)
        src[i] = random() % 0xff;
    memset(dst, 0xff, ld_dst * width);

    rpiraw_calc_luminance_bt709_rgb888(dst, ld_dst, src, width,
                                             width, height);

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++)
            CU_ASSERT_NOT_EQUAL(dst[ld_dst * i + j], 255);
        for (; j < ld_dst; j ++)
            CU_ASSERT_EQUAL(dst[ld_dst * i + j], 255);
    }

    free(dst);
    free(src);
}

int suite_luminance()
{
    CU_pSuite suite = CU_add_suite("luminance", NULL, NULL);
    if (suite == NULL)
        return 1;

    if (CU_add_test(suite, "bt709 rgb888: ld_dst != width",
                    test_bt709_rgb888_ld_dst) == NULL)
        return 1;

    return 0;
}
