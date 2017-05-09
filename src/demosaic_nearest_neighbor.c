/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include "rpiraw.h"
#include "local.h"
#include <stdint.h>


#define R_AT_B  \
    (  src[(i - 1) * ld_src + (j - 1)]      \
     + src[(i - 1) * ld_src + (j + 1)]      \
     + src[(i + 1) * ld_src + (j - 1)]      \
     + src[(i + 1) * ld_src + (j + 1)]) / 4
#define G_AT_B  \
    (  src[(i - 1) * ld_src +  j     ]      \
     + src[ i      * ld_src + (j - 1)]      \
     + src[ i      * ld_src + (j + 1)]      \
     + src[(i + 1) * ld_src +  j     ]) / 4
#define B_AT_B  \
       src[ i      * ld_src +  j]

#define R_AT_GB \
    (  src[(i - 1) * ld_src +  j     ]      \
     + src[(i + 1) * ld_src +  j     ]) / 2
#define G_AT_GB \
    (  src[(i - 1) * ld_src + (j - 1)]      \
     + src[(i - 1) * ld_src + (j + 1)]      \
     + src[ i      * ld_src +  j     ]      \
     + src[(i + 1) * ld_src + (j - 1)]      \
     + src[(i + 1) * ld_src + (j + 1)]) / 5
#define B_AT_GB \
    (  src[ i      * ld_src + (j - 1)]      \
     + src[ i      * ld_src + (j + 1)]) / 2

#define R_AT_GR B_AT_GB
#define G_AT_GR G_AT_GB
#define B_AT_GR R_AT_GB

#define R_AT_R  B_AT_B
#define G_AT_R  G_AT_B
#define B_AT_R  R_AT_B


int rpiraw_raw8bggr_to_rgb888_nearest_neighbor(uint8_t *dst,
                                               const unsigned ld_dst,
                                               uint8_t *src,
                                               const unsigned ld_src,
                                               const unsigned width,
                                               const unsigned height)
{
    unsigned i, j;
    int ret = 0;

    ret = rpiraw_raw8bggr_to_rgb888_edge(dst, ld_dst, src, ld_src,
                                         width, height, 1, 1, 1, 1);
    if (ret)
        return ret;

    dst += ld_dst * 3;
    for (i = 1; i < height - 1; ) {

        dst += 3;

        for (j = 1; j < width - 1; ) {
            *dst++ = R_AT_R;  *dst++ = G_AT_R;  *dst++ = B_AT_R;
            j ++;
            if (j >= width - 1)
                break;
            *dst++ = R_AT_GR; *dst++ = G_AT_GR; *dst++ = B_AT_GR;
            j ++;
        }
        if (j == width - 1 - 1) {
            *dst++ = R_AT_R;  *dst++ = G_AT_R;  *dst++ = B_AT_R;
        }
        i ++;

        if (i >= height - 1)
            break;

        dst += 3 + (ld_dst - width) * 3 + 3;

        for (j = 1; j < width - 1; ) {
            *dst++ = R_AT_GB; *dst++ = G_AT_GB; *dst++ = B_AT_GB;
            j ++;
            if (j >= width - 1)
                break;
            *dst++ = R_AT_B;  *dst++ = G_AT_B;  *dst++ = B_AT_B;
            j ++;
        }
        if (j == width - 1 - 1) {
            *dst++ = R_AT_GB; *dst++ = G_AT_GB; *dst++ = B_AT_GB;
        }
        i ++;

        dst += 3 + (ld_dst - width) * 3;
    }

    return ret;
}

#undef R_AT_B
#undef G_AT_B
#undef B_AT_B
#undef R_AT_GB
#undef G_AT_GB
#undef B_AT_GB
#undef R_AT_GR
#undef G_AT_GR
#undef B_AT_GR
#undef R_AT_R
#undef G_AT_R
#undef B_AT_R
