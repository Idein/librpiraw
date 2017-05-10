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

static void calc_histogram_rgb888(uint32_t hist_r[256], uint32_t hist_g[256],
                                  uint32_t hist_b[256],
                                  uint8_t *img, const unsigned ld,
                                  const unsigned width, const unsigned height)
{
    unsigned i, j;

    memset(hist_r, 0, 256 * sizeof(*hist_r));
    memset(hist_g, 0, 256 * sizeof(*hist_g));
    memset(hist_b, 0, 256 * sizeof(*hist_b));
    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            hist_r[*img++]++;
            hist_g[*img++]++;
            hist_b[*img++]++;
        }
        img += (ld - width) * 3;
    }
}

static void seed_random()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    srandom(t.tv_sec ^ t.tv_usec);
}

static uint8_t random_uint8()
{
    return random() % 0xff;
}

static void test_rgb888_all_random()
{
    uint8_t *img = NULL;
    uint32_t hist_r[256],         hist_g[256],         hist_b[256];
    uint32_t hist_r_correct[256], hist_g_correct[256], hist_b_correct[256];
    unsigned i;
    int ret;

    img = malloc(width * height * 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(img);

    seed_random();

    for (i = 0; i < height * width * 3; i ++)
        img[i] = random_uint8();

    calc_histogram_rgb888(hist_r_correct, hist_g_correct, hist_b_correct,
                          img, width, width, height);
    ret = rpiraw_calc_histogram_rgb888(hist_r, hist_g, hist_b,
                                       img, width, width, height);
    CU_ASSERT_EQUAL(ret, 0);

    for (i = 0; i < 256; i ++) {
        CU_ASSERT_EQUAL(hist_r[i], hist_r_correct[i]);
        CU_ASSERT_EQUAL(hist_g[i], hist_g_correct[i]);
        CU_ASSERT_EQUAL(hist_b[i], hist_b_correct[i]);
    }
}

int suite_histogram()
{
    CU_pSuite suite = CU_add_suite("histogram", NULL, NULL);
    if (suite == NULL)
        return 1;

    if (CU_add_test(suite, "rgb888: all random",
                    test_rgb888_all_random) == NULL)
        return 1;

    return 0;
}
