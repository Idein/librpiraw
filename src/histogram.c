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
#include <string.h>
#include <omp.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int rpiraw_calc_histogram_rgb888(uint32_t hist_r[256], uint32_t hist_g[256],
                                 uint32_t hist_b[256],
                                 uint8_t *img, const unsigned ld,
                                 const unsigned width, const unsigned height)
{
    memset(hist_r, 0, 256 * sizeof(*hist_r));
    memset(hist_g, 0, 256 * sizeof(*hist_g));
    memset(hist_b, 0, 256 * sizeof(*hist_b));

#pragma omp parallel firstprivate(img)
    {
        unsigned i, j;
        const int nthreads = omp_get_num_threads();
        const int num = omp_get_thread_num();
        const unsigned i_start = height *  num      / nthreads;
        const unsigned i_end   = height * (num + 1) / nthreads;
        uint32_t hist_r_priv[256], hist_g_priv[256], hist_b_priv[256];

        memset(hist_r_priv, 0, 256 * sizeof(*hist_r_priv));
        memset(hist_g_priv, 0, 256 * sizeof(*hist_g_priv));
        memset(hist_b_priv, 0, 256 * sizeof(*hist_b_priv));
        img += i_start * ld * 3;

        for (i = i_start; i < i_end; i ++) {
            for (j = 0; j < width; j ++) {
                hist_r_priv[*img++]++;
                hist_g_priv[*img++]++;
                hist_b_priv[*img++]++;
            }
            img += (ld - width) * 3;
        }

#pragma omp critical
        for (i = 0; i < 256; i ++)
            hist_r[i] += hist_r_priv[i];
#pragma omp critical
        for (i = 0; i < 256; i ++)
            hist_g[i] += hist_g_priv[i];
#pragma omp critical
        for (i = 0; i < 256; i ++)
            hist_b[i] += hist_b_priv[i];
    }

    return 0;
}

void rpiraw_histogram_to_rgb888(uint8_t *img,
                                const unsigned ld, const unsigned stride,
                                uint32_t *hist, const unsigned hist_len,
                                const uint32_t hist_max,
                                const unsigned hist_width,
                                const unsigned hist_height)
{
    unsigned i, j, k;
    const double mag_height = (double) hist_height / hist_max;

    for (i = 0; i < hist_len; i ++) {
        const unsigned val = MIN(hist[i] * mag_height, hist_height);
        for (j = 0; j < hist_width; j ++) {
            for (k = 0; k < val; k ++)
                img[k * stride * 3] = 0xff;
            img += ld * 3;
        }
    }
}
