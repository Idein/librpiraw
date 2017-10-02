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
#include <math.h> /* For roundf(). */
#include <sys/param.h> /* For MIN() and MAX(). */
#include <omp.h>

#define ALIGN_UP(v, n) (((v) + (n) - 1) & ~((n) - 1))
#define CLAMP(v, min, max) ((MIN(MAX(((v)), min), max)))

unsigned rpiraw_width_raw8_to_raw10_rpi(const unsigned width)
{
    /*
     * MIPI specifies the size be height * width * 10 / 8.
     * However, the firmware of Raspberry Pi uses the equation below.
     * See https://github.com/6by9/RPiTest/commit/d66f1454e1986d137d040a36e1d2f6b5a3217259#commitcomment-21443216
    */
    return ALIGN_UP((width * 5 + 3) >> 2, 32);
}

int rpiraw_raw8bggr_component_gain(uint8_t *dst, const unsigned ld_dst,
                                   uint8_t *src, const unsigned ld_src,
                                   const unsigned width, const unsigned height,
                                   const float gain_r, const float gain_g,
                                   const float gain_b)
{
    int ret = 0;

#define MULT_GAIN(v, gain) CLAMP(roundf(v * gain), 0, 255)

    /* B and G line (0, 2, 4, ...) */
#define DO_EVEN_LINE() \
    do { \
        for (j = 0; j < width - 1; ) { \
            dst[i * ld_dst + j] = MULT_GAIN(src[i * ld_src + j], gain_b); \
            j ++; \
            dst[i * ld_dst + j] = MULT_GAIN(src[i * ld_src + j], gain_g); \
            j ++; \
        } \
        if (j == width - 1) { \
            dst[i * ld_dst + j] = MULT_GAIN(src[i * ld_src + j], gain_b); \
            j ++; \
        } \
    } while (0)

    /* G and R line (1, 3, 5, ...) */
#define DO_ODD_LINE() \
    do { \
        for (j = 0; j < width - 1; ) { \
            dst[i * ld_dst + j] = MULT_GAIN(src[i * ld_src + j], gain_g); \
            j ++; \
            dst[i * ld_dst + j] = MULT_GAIN(src[i * ld_src + j], gain_r); \
            j ++; \
        } \
        if (j == width - 1) { \
            dst[i * ld_dst + j] = MULT_GAIN(src[i * ld_src + j], gain_g); \
            j ++; \
        } \
    } while (0)

#pragma omp parallel firstprivate(dst, src)
    {
        unsigned i, j;
        const int num_threads = omp_get_num_threads(),
                  thread_num = omp_get_thread_num();
        const unsigned i_start = height *  thread_num      / num_threads,
                       i_end   = height * (thread_num + 1) / num_threads;

        i = i_start;
        if (i % 2 == 1) {
            DO_ODD_LINE();  i ++;
        }
        for (; i < i_end - 1; ) {
            DO_EVEN_LINE(); i ++;
            DO_ODD_LINE();  i ++;
        }
        if (i == i_end - 1) {
            DO_EVEN_LINE(); i ++;
        }
    }

#undef DO_ODD_LINE
#undef DO_EVEN_LINE

    return ret;
}
