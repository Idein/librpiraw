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

static const unsigned width = 1024, height = 1024;

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

static void test_all_random()
{
    uint8_t *img = NULL;
    unsigned i;
    int ret;

    img = malloc(width * height * 3);
    CU_ASSERT_PTR_NOT_NULL_FATAL(img);

    seed_random();
    for (i = 0; i < height * width * 3; i ++)
        img[i] = random_uint8();

    ret = rpiraw_write_png_rgb888("file_png_random.png",
                                  img, width, width, height);
    CU_ASSERT_EQUAL(ret, 0);
}

int suite_file_png()
{
    CU_pSuite suite = CU_add_suite("file_png", NULL, NULL);
    if (suite == NULL)
        return 1;

    if (CU_add_test(suite, "all random",
                    test_all_random) == NULL)
        return 1;

    return 0;
}
