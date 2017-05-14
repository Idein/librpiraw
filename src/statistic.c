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

/*
 * M. V. Shirvaikar, "An optimal measure for camera focus and exposure",
 *   Thirty-Sixth Southeastern Symposium on System Theory,
 *   2004. Proceedings of the, 2004, pp. 472-475.
 */
uint32_t rpiraw_calc_mean_by_hist(uint32_t *hist, const unsigned len,
                                  const unsigned npixs)
{
    unsigned i;
    uint_fast32_t sum = 0;

    for (i = 0; i < len; i ++)
        sum += i * hist[i];

    return sum / npixs;
}
