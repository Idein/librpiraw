/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#ifndef RPIRAW_H
#define RPIRAW_H

#include <stdint.h>

    int rpiraw_convert_raw10_to_raw8(uint8_t *dst, const unsigned ld_dst,
                                     uint8_t *src, const unsigned ld_src,
                                     const unsigned width,
                                     const unsigned height);
    int rpiraw_convert_raw10_to_raw16(uint16_t *dst, const unsigned ld_dst,
                                      uint8_t *src, const unsigned ld_src,
                                      const unsigned width,
                                      const unsigned height);

    int rpiraw_raw8bggr_to_rgb888_edge(uint8_t *dst, const unsigned ld_dst,
                                       uint8_t *src, const unsigned ld_src,
                                       const unsigned width,
                                       const unsigned height,
                                       const unsigned edge_n,
                                       const unsigned edge_e,
                                       const unsigned edge_s,
                                       const unsigned edge_w);
    int rpiraw_raw8bggr_to_rgb888_nearest_neighbor(uint8_t *dst,
                                                   const unsigned ld_dst,
                                                   uint8_t *src,
                                                   const unsigned ld_src,
                                                   const unsigned width,
                                                   const unsigned height);

#endif /* RPIRAW_H */