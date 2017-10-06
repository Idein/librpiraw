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


static inline uint8_t R_AT_B_core(uint8_t *src, const unsigned ld_src,
                                  const unsigned i, const unsigned j,
                                  const unsigned width, const unsigned height)
{
    uint_fast16_t sum = 0;
    uint_fast8_t count = 0;

    if (i >= 1 && j >= 1) {
        sum += src[(i - 1) * ld_src + (j - 1)];
        count ++;
    }
    if (i >= 1 && j < width - 1) {
        sum += src[(i - 1) * ld_src + (j + 1)];
        count ++;
    }
    if (i < height - 1 && j >= 1) {
        sum += src[(i + 1) * ld_src + (j - 1)];
        count ++;
    }
    if (i < height - 1 && j < width - 1) {
        sum += src[(i + 1) * ld_src + (j + 1)];
        count ++;
    }

    return count ? sum / count : 0;
}

static inline uint8_t G_AT_B_core(uint8_t *src, const unsigned ld_src,
                                  const unsigned i, const unsigned j,
                                  const unsigned width, const unsigned height)
{
    uint_fast16_t sum = 0;
    uint_fast8_t count = 0;

    if (i >= 1) {
        sum += src[(i - 1) * ld_src +  j     ];
        count ++;
    }
    if (j >= 1) {
        sum += src[ i      * ld_src + (j - 1)];
        count ++;
    }
    if (j < width - 1) {
        sum += src[ i      * ld_src + (j + 1)];
        count ++;
    }
    if (i < height - 1) {
        sum += src[(i + 1) * ld_src +  j     ];
        count ++;
    }

    return count ? sum / count : 0;
}

static inline uint8_t R_AT_GB_core(uint8_t *src, const unsigned ld_src,
                                   const unsigned i, const unsigned j,
                                   const unsigned width, const unsigned height)
{
    uint_fast16_t sum = 0;
    uint_fast8_t count = 0;

    (void) width;

    if (i >= 1) {
        sum += src[(i - 1) * ld_src +  j     ];
        count ++;
    }
    if (i < height - 1) {
        sum += src[(i + 1) * ld_src +  j     ];
        count ++;
    }

    return count ? sum / count : 0;
}

static inline uint8_t B_AT_GB_core(uint8_t *src, const unsigned ld_src,
                                   const unsigned i, const unsigned j,
                                   const unsigned width, const unsigned height)
{
    uint_fast16_t sum = 0;
    uint_fast8_t count = 0;

    (void) height;

    if (j >= 1) {
        sum += src[ i      * ld_src + (j - 1)];
        count ++;
    }
    if (j < width - 1) {
        sum += src[ i      * ld_src + (j + 1)];
        count ++;
    }

    return count ? sum / count : 0;
}

#define R_AT_B  R_AT_B_core(src, ld_src, i, j, width, height)
#define G_AT_B  G_AT_B_core(src, ld_src, i, j, width, height)
#define B_AT_B  src[i * ld_src + j]

#define R_AT_GB R_AT_GB_core(src, ld_src, i, j, width, height)
#define G_AT_GB src[i * ld_src + j]
#define B_AT_GB B_AT_GB_core(src, ld_src, i, j, width, height)

#define R_AT_GR B_AT_GB
#define G_AT_GR G_AT_GB
#define B_AT_GR R_AT_GB

#define R_AT_R  B_AT_B
#define G_AT_R  G_AT_B
#define B_AT_R  R_AT_B


int rpiraw_raw8bggr_to_rgb888_edge(uint8_t *dst, const unsigned ld_dst,
                                   uint8_t *src, const unsigned ld_src,
                                   const unsigned width, const unsigned height,
                                   const unsigned edge_n, const unsigned edge_e,
                                   const unsigned edge_s, const unsigned edge_w)
{
    unsigned i, j;
    uint8_t * const dst_orig = dst;

    /* North */
    for (i = 0; i < edge_n; i ++) {
        for (j = 0; j < width; j ++) {
            if (i % 2 == 0) {
                if (j % 2 == 0) {
                    *dst++ = R_AT_B;  *dst++ = G_AT_B;  *dst++ = B_AT_B;
                } else {
                    *dst++ = R_AT_GB; *dst++ = G_AT_GB; *dst++ = B_AT_GB;
                }
            } else {
                if (j % 2 == 0) {
                    *dst++ = R_AT_GR; *dst++ = G_AT_GR; *dst++ = B_AT_GR;
                } else {
                    *dst++ = R_AT_R;  *dst++ = G_AT_R;  *dst++ = B_AT_R;
                }
            }
        }
        dst += (ld_dst - width) * 3;
    }

    /* West */
    for (i = edge_n; i < height - edge_s; i ++) {
        for (j = 0; j < edge_w; j ++) {
            if (i % 2 == 0) {
                if (j % 2 == 0) {
                    *dst++ = R_AT_B;  *dst++ = G_AT_B;  *dst++ = B_AT_B;
                } else {
                    *dst++ = R_AT_GB; *dst++ = G_AT_GB; *dst++ = B_AT_GB;
                }
            } else {
                if (j % 2 == 0) {
                    *dst++ = R_AT_GR; *dst++ = G_AT_GR; *dst++ = B_AT_GR;
                } else {
                    *dst++ = R_AT_R;  *dst++ = G_AT_R;  *dst++ = B_AT_R;
                }
            }
        }
        dst += (ld_dst - edge_w) * 3;
    }

    /* East */
    dst = dst_orig + (edge_n * ld_dst + (width - edge_e)) * 3;
    for (i = edge_n; i < height - edge_s; i ++) {
        for (j = width - edge_e; j < width; j ++) {
            if (i % 2 == 0) {
                if (j % 2 == 0) {
                    *dst++ = R_AT_B;  *dst++ = G_AT_B;  *dst++ = B_AT_B;
                } else {
                    *dst++ = R_AT_GB; *dst++ = G_AT_GB; *dst++ = B_AT_GB;
                }
            } else {
                if (j % 2 == 0) {
                    *dst++ = R_AT_GR; *dst++ = G_AT_GR; *dst++ = B_AT_GR;
                } else {
                    *dst++ = R_AT_R;  *dst++ = G_AT_R;  *dst++ = B_AT_R;
                }
            }
        }
        dst += (ld_dst - width) * 3 + (width - edge_e) * 3;
    }

    /* South */
    dst = dst_orig + (height - edge_s) * ld_dst * 3;
    for (i = height - edge_s; i < height; i ++) {
        for (j = 0; j < width; j ++) {
            if (i % 2 == 0) {
                if (j % 2 == 0) {
                    *dst++ = R_AT_B;  *dst++ = G_AT_B;  *dst++ = B_AT_B;
                } else {
                    *dst++ = R_AT_GB; *dst++ = G_AT_GB; *dst++ = B_AT_GB;
                }
            } else {
                if (j % 2 == 0) {
                    *dst++ = R_AT_GR; *dst++ = G_AT_GR; *dst++ = B_AT_GR;
                } else {
                    *dst++ = R_AT_R;  *dst++ = G_AT_R;  *dst++ = B_AT_R;
                }
            }
        }
        dst += (ld_dst - width) * 3;
    }

    return 0;
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
