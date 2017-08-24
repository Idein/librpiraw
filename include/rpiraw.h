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

    unsigned rpiraw_width_raw8_to_raw10_rpi(const unsigned width);

    int rpiraw_convert_raw10_to_raw8(uint8_t *dst, uint8_t *src,
                                     const unsigned width,
                                     const unsigned height,
                                     const unsigned raw_width);
    int rpiraw_convert_raw10_to_raw16(uint16_t *dst, uint8_t *src,
                                      const unsigned width,
                                      const unsigned height);

    int rpiraw_write_png_rgb888(const char *filename,
                                uint8_t *src, const unsigned ld_src,
                                const unsigned width, const unsigned height);

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
    int rpiraw_raw8bggr_to_rgb888_malvar_he_culter(uint8_t *dst,
                                                   const unsigned ld_dst,
                                                   uint8_t *src,
                                                   const unsigned ld_src,
                                                   const unsigned width,
                                                   const unsigned height);

    int rpiraw_calc_histogram_rgb888(uint32_t hist_r[256], uint32_t hist_g[256],
                                     uint32_t hist_b[256],
                                     uint8_t *img, const unsigned ld,
                                     const unsigned width,
                                     const unsigned height);

    void rpiraw_histogram_to_rgb888(uint8_t *img,
                                    const unsigned ld, const unsigned stride,
                                    uint32_t *hist, const unsigned hist_len,
                                    const uint32_t hist_max,
                                    const unsigned hist_width,
                                    const unsigned hist_height);

    float rpiraw_calc_mean_by_hist(uint32_t *hist, const unsigned len,
                                   const unsigned npixs);

    float rpiraw_calc_acm_by_hist(uint32_t *hist, const unsigned len,
                                  const unsigned npixs, const float mean);

    int rpiraw_histogram_equalize_rgb888(uint8_t *dst, const unsigned ld_dst,
                                         uint8_t *src, const unsigned ld_src,
                                         const unsigned width,
                                         const unsigned height,
                                         uint32_t hist_r[256],
                                         uint32_t hist_g[256],
                                         uint32_t hist_b[256]);

    void rpiraw_calc_luminance_bt709_rgb888(uint8_t *dst,
                                            const unsigned ld_dst,
                                            uint8_t *src,
                                            const unsigned ld_src,
                                            const unsigned width,
                                            const unsigned height);

#endif /* RPIRAW_H */
