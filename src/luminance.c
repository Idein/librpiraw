/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include "rpiraw.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>

/* ITU (International Telecommunication Union),
 *   "BT.709 : Parameter values for the HDTV standards for production and
 *   international programme exchange", http://www.itu.int/rec/R-REC-BT.709
 */

void rpiraw_calc_luminance_bt709_rgb888(uint8_t *dst,
                                        const unsigned ld_dst,
                                        uint8_t *src,
                                        const unsigned ld_src,
                                        const unsigned width,
                                        const unsigned height)
{
    unsigned i, j;

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            *dst++ = round(  0.2126 * src[0]
                           + 0.7152 * src[1]
                           + 0.0722 * src[2]);
            src += 3;
        }
        dst += ld_dst - width;
        src += (ld_src - width) * 3;
    }
}
