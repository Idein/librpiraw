/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

/*
 * Reference:
 * H. S. Malvar, Li-wei He and R. Cutler, "High-quality linear interpolation for
 * demosaicing of Bayer-patterned color images," 2004 IEEE International
 * Conference on Acoustics, Speech, and Signal Processing, 2004, pp. iii-485-8
 * vol.3.
 */

#include "rpiraw.h"
#include "local.h"
#include <stdint.h>
#include <omp.h>

#define MARGIN_NORTH 2
#define MARGIN_EAST  2
#define MARGIN_SOUTH 2
#define MARGIN_WEST  2

#define R_AT_R \
       src[ i      * ld_src +  j     ]
#define G_AT_GR R_AT_R
#define G_AT_GB R_AT_R
#define B_AT_B  R_AT_R

#define G_AT_R \
    (- src[(i - 2) * ld_src +  j     ]     \
     + src[(i - 1) * ld_src +  j     ] * 2 \
     - src[ i      * ld_src + (j - 2)]     \
     + src[ i      * ld_src + (j - 1)] * 2 \
     + src[ i      * ld_src +  j     ] * 4 \
     + src[ i      * ld_src + (j + 1)] * 2 \
     - src[ i      * ld_src + (j + 2)]     \
     + src[(i + 1) * ld_src +  j     ] * 2 \
     - src[(i + 2) * ld_src +  j     ]    )
#define G_AT_B  G_AT_R

#define R_AT_GR \
    (  src[(i - 2) * ld_src +  j     ] / 2     \
     - src[(i - 1) * ld_src + (j - 1)]         \
     - src[(i - 1) * ld_src + (j + 1)]         \
     - src[ i      * ld_src + (j - 2)]         \
     + src[ i      * ld_src + (j - 1)] * 4     \
     + src[ i      * ld_src +  j     ] * 5     \
     + src[ i      * ld_src + (j + 1)] * 4     \
     - src[ i      * ld_src + (j + 2)]         \
     - src[(i + 1) * ld_src + (j + 1)]         \
     - src[(i + 1) * ld_src + (j - 1)]         \
     + src[(i + 2) * ld_src +  j     ] / 2)
#define R_AT_GB \
    (  src[(i - 2) * ld_src +  j     ] / 2     \
     - src[(i - 1) * ld_src + (j - 1)]         \
     + src[(i - 1) * ld_src +  j     ] * 4     \
     - src[(i - 1) * ld_src + (j + 1)]         \
     - src[ i      * ld_src + (j - 2)]         \
     + src[ i      * ld_src +  j     ] * 5     \
     - src[ i      * ld_src + (j + 2)]         \
     - src[(i + 1) * ld_src + (j - 1)]         \
     + src[(i - 1) * ld_src +  j     ] * 4     \
     - src[(i + 1) * ld_src + (j + 1)]         \
     + src[(i + 2) * ld_src +  j     ] / 2)
#define R_AT_B \
    (- src[(i - 2) * ld_src +  j     ] * 3 / 2 \
     + src[(i - 1) * ld_src + (j - 1)] * 2     \
     + src[(i - 1) * ld_src + (j + 1)] * 2     \
     - src[ i      * ld_src + (j - 2)] * 3 / 2 \
     + src[ i      * ld_src +  j     ] * 6     \
     - src[ i      * ld_src + (j + 2)] * 3 / 2 \
     + src[(i + 1) * ld_src + (j - 1)] * 2     \
     + src[(i + 1) * ld_src + (j + 1)] * 2     \
     - src[(i + 2) * ld_src +  j     ] * 3 / 2)

#define B_AT_GB R_AT_GR
#define B_AT_GR R_AT_GB
#define B_AT_R  R_AT_B

#define DO_ODD_LINE() \
    do { \
        for (j = MARGIN_WEST; j < width - MARGIN_EAST; ) { \
            *dst++ = R_AT_R;  *dst++ = G_AT_R;  *dst++ = B_AT_R;  j ++; \
            *dst++ = R_AT_GR; *dst++ = G_AT_GR; *dst++ = B_AT_GR; j ++; \
        } \
        if (j == (width - MARGIN_EAST) - 1) { \
            *dst++ = R_AT_R;  *dst++ = G_AT_R;  *dst++ = B_AT_R; \
        } \
    } while (0)

#define DO_EVEN_LINE() \
    do { \
        for (j = MARGIN_WEST; j < width - MARGIN_EAST; ) { \
            *dst++ = R_AT_GB; *dst++ = G_AT_GB; *dst++ = B_AT_GB; j ++; \
            *dst++ = R_AT_B;  *dst++ = G_AT_B;  *dst++ = B_AT_B;  j ++; \
        } \
        if (j == (width - MARGIN_EAST) - 1) { \
            *dst++ = R_AT_GB; *dst++ = G_AT_GB; *dst++ = B_AT_GB; \
        } \
    } while (0)

int rpiraw_raw8bggr_to_rgb888_malvar_he_culter(uint8_t *dst,
                                               const unsigned ld_dst,
                                               uint8_t *src,
                                               const unsigned ld_src,
                                               const unsigned width,
                                               const unsigned height)
{
    int ret = 0;

    ret = rpiraw_raw8bggr_to_rgb888_edge(dst, ld_dst, src, ld_src,
                                         width, height,
                                         MARGIN_NORTH, MARGIN_EAST,
                                         MARGIN_SOUTH, MARGIN_WEST);
    if (ret)
        return ret;

#pragma omp parallel firstprivate(dst, src)
    {
        unsigned i, j;
        const int nthreads = omp_get_num_threads();
        const int num = omp_get_thread_num();
        const unsigned i_start = MARGIN_WEST
                                  + (height - (MARGIN_EAST + MARGIN_WEST))
                                      *  num      / nthreads,
                       i_end   = MARGIN_WEST
                                  + (height - (MARGIN_EAST + MARGIN_WEST))
                                      * (num + 1) / nthreads;
        const unsigned dst_stride = 3 * (MARGIN_WEST + (ld_dst - width)
                                          + MARGIN_EAST);

        dst += 3 * (i_start * ld_dst + MARGIN_EAST);
        i = i_start;
        if (i % 2 == 0) {
            DO_EVEN_LINE(); i ++; dst += dst_stride;
        }
        for (; i < i_end; ) {
            DO_ODD_LINE();  i ++; dst += dst_stride;
            DO_EVEN_LINE(); i ++; dst += dst_stride;
        }
        if (i == i_end - 1) {
            DO_ODD_LINE();        dst += dst_stride;
        }
    }

    return ret;
}

#undef DO_EVEN_LINE
#undef DO_ODD_LINE
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
