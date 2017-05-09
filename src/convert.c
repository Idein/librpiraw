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

int rpiraw_convert_raw10_to_raw8(uint8_t *dst, uint8_t *src,
                                 const unsigned width, const unsigned height,
                                 const unsigned raw_width)
{
    unsigned i, j;

    /* As specified in MIPI Alliance Specification for CSI-2. */

    if (width % 4 != 0)
        return 1;

    for (i = 0; i < height; i ++) {
        for (j = 0; j < width / 4; j ++) {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            src++;
        }
        src += raw_width - width * 10 / 8;
    }

    return 0;
}
