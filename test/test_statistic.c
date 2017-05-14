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

static uint8_t random_uint8()
{
    return random() % 0xff;
}

static void calc_mean_by_img(uint32_t *mean_rp, uint32_t *mean_gp,
                             uint32_t *mean_bp,
                             uint8_t *img, const unsigned ld,
                             const unsigned width, const unsigned height)
{
    unsigned i, j;
    uint32_t sum_r = 0, sum_g = 0, sum_b = 0;

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            sum_r += *img++;
            sum_g += *img++;
            sum_b += *img++;
        }
        img += (ld - width) * 3;
    }

    *mean_rp = sum_r / (width * height);
    *mean_gp = sum_g / (width * height);
    *mean_bp = sum_b / (width * height);
}

static void calc_acm_by_img(uint32_t *acm_rp, uint32_t *acm_gp,
                            uint32_t *acm_bp,
                            const uint32_t mean_r, const uint32_t mean_g,
                            const uint32_t mean_b,
                            uint8_t *img, const unsigned ld,
                            const unsigned width, const unsigned height)
{
    unsigned i, j;
    uint32_t sum_r = 0, sum_g = 0, sum_b = 0;

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            sum_r += abs((int32_t) *img++ - mean_r);
            sum_g += abs((int32_t) *img++ - mean_g);
            sum_b += abs((int32_t) *img++ - mean_b);
        }
        img += (ld - width) * 3;
    }

    *acm_rp = sum_r / (width * height);
    *acm_gp = sum_g / (width * height);
    *acm_bp = sum_b / (width * height);
}

static void test_mean_by_hist_all_random()
{
    uint8_t *img = NULL;
    uint32_t hist_r[256], hist_g[256], hist_b[256];
    uint32_t mean_r,         mean_g,         mean_b;
    uint32_t mean_r_correct, mean_g_correct, mean_b_correct;
    unsigned i;
    int ret;

    img = malloc(width * height * 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(img);

    seed_random();

    for (i = 0; i < height * width * 3; i ++)
        img[i] = random_uint8();

    calc_mean_by_img(&mean_r_correct, &mean_g_correct, &mean_b_correct,
                     img, width, width, height);

    ret = rpiraw_calc_histogram_rgb888(hist_r, hist_g, hist_b,
                                       img, width, width, height);
    CU_ASSERT_EQUAL(ret, 0);
    mean_r = rpiraw_calc_mean_by_hist(hist_r, 256, width * height);
    mean_g = rpiraw_calc_mean_by_hist(hist_g, 256, width * height);
    mean_b = rpiraw_calc_mean_by_hist(hist_b, 256, width * height);

    CU_ASSERT_EQUAL(mean_r, mean_r_correct);
    CU_ASSERT_EQUAL(mean_g, mean_g_correct);
    CU_ASSERT_EQUAL(mean_b, mean_b_correct);
}

static void test_acm_by_hist_all_random()
{
    uint8_t *img = NULL;
    uint32_t hist_r[256], hist_g[256], hist_b[256];
    uint32_t mean_r, mean_g, mean_b;
    uint32_t acm_r,         acm_g,         acm_b;
    uint32_t acm_r_correct, acm_g_correct, acm_b_correct;
    unsigned i;
    int ret;

    img = malloc(width * height * 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(img);

    seed_random();

    for (i = 0; i < height * width * 3; i ++)
        img[i] = random_uint8();

    ret = rpiraw_calc_histogram_rgb888(hist_r, hist_g, hist_b,
                                       img, width, width, height);
    CU_ASSERT_EQUAL(ret, 0);
    mean_r = rpiraw_calc_mean_by_hist(hist_r, 256, width * height);
    mean_g = rpiraw_calc_mean_by_hist(hist_g, 256, width * height);
    mean_b = rpiraw_calc_mean_by_hist(hist_b, 256, width * height);

    calc_acm_by_img(&acm_r_correct, &acm_g_correct, &acm_b_correct,
                    mean_r, mean_g, mean_b,
                    img, width, width, height);

    acm_r = rpiraw_calc_acm_by_hist(hist_r, 256, width * height, mean_r);
    acm_g = rpiraw_calc_acm_by_hist(hist_g, 256, width * height, mean_g);
    acm_b = rpiraw_calc_acm_by_hist(hist_b, 256, width * height, mean_b);

    CU_ASSERT_EQUAL(acm_r, acm_r_correct);
    CU_ASSERT_EQUAL(acm_g, acm_g_correct);
    CU_ASSERT_EQUAL(acm_b, acm_b_correct);
}

int suite_statistic()
{
    CU_pSuite suite = CU_add_suite("statistic", NULL, NULL);
    if (suite == NULL)
        return 1;

    if (CU_add_test(suite, "mean by histogram: all random",
                    test_mean_by_hist_all_random) == NULL)
        return 1;
    if (CU_add_test(suite, "acm by histogram: all random",
                    test_acm_by_hist_all_random) == NULL)
        return 1;

    return 0;
}
