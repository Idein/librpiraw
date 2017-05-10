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

int rpiraw_convert_raw10_to_raw8(uint8_t *dst, uint8_t *src,
                                 const unsigned width, const unsigned height,
                                 const unsigned raw_width)
{
    /* As specified in MIPI Alliance Specification for CSI-2. */

    if (width % 4 != 0)
        return 1;

#pragma omp parallel firstprivate(dst, src)
    {
        unsigned i, j;
        const int nthreads = omp_get_num_threads();
        const int num = omp_get_thread_num();
        const unsigned i_start = height *  num      / nthreads,
                       i_end   = height * (num + 1) / nthreads;

        dst += i_start * width;
        src += i_start * raw_width;

        for (i = i_start; i < i_end; i ++) {
            for (j = 0; j < width / 4; j ++) {
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                *dst++ = *src++;
                src++;
            }
            src += raw_width - width * 10 / 8;
        }
    }

    return 0;
}
