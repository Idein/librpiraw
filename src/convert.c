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
                                 const unsigned width, const unsigned height)
{
    unsigned int i;

    /* As specified in MIPI Alliance Specification for CSI-2. */

    if ((width * height) % 4 != 0)
        return 1;

    for (i = 0; i < width * height; i ++) {
        *dst++ = *src++;
        if (i % 4 == 3)
            src++;
    }

    return 0;
}
