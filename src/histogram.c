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

int rpiraw_calc_histogram_rgb888(uint32_t hist_r[256], uint32_t hist_g[256],
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

    return 0;
}
