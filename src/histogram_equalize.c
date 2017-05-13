/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include "rpiraw.h"
#include <stdint.h>
#include <omp.h>

/*
 * https://www.math.uci.edu/icamp/courses/math77c/demos/hist_eq.pdf
 */

int rpiraw_histogram_equalize_rgb888(uint8_t *dst, const unsigned ld_dst,
                                     uint8_t *src, const unsigned ld_src,
                                     const unsigned width,
                                     const unsigned height,
                                     uint32_t hist_r[256],
                                     uint32_t hist_g[256],
                                     uint32_t hist_b[256])
{
    unsigned i, j;
    uint32_t sum_r = 0, sum_g = 0, sum_b = 0;
    uint8_t cdf_r[256], cdf_g[256], cdf_b[256];
    const unsigned pixels = width * height;
    int ret = 0;

    for (i = 0; i < 256; i ++) {
        sum_r += hist_r[i];
        sum_g += hist_g[i];
        sum_b += hist_b[i];
        cdf_r[i] = sum_r * 255 / pixels;
        cdf_g[i] = sum_g * 255 / pixels;
        cdf_b[i] = sum_b * 255 / pixels;
    }

#pragma omp parallel firstprivate(src, dst) private(i, j)
    {
        const int nthreads = omp_get_num_threads();
        const int num = omp_get_thread_num();
        const unsigned i_start = height *  num      / nthreads,
                       i_end   = height * (num + 1) / nthreads;

        dst += i_start * ld_dst * 3;
        src += i_start * ld_src * 3;

        for (i = i_start; i < i_end; i ++) {
            for (j = 0; j < width; j ++) {
                *dst++ = cdf_r[*src++];
                *dst++ = cdf_g[*src++];
                *dst++ = cdf_b[*src++];
            }
            dst += (ld_dst - width) * 3;
            src += (ld_src - width) * 3;
        }
    }

    return ret;
}
