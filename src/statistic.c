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
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/*
 * M. V. Shirvaikar, "An optimal measure for camera focus and exposure",
 *   Thirty-Sixth Southeastern Symposium on System Theory,
 *   2004. Proceedings of the, 2004, pp. 472-475.
 */
float rpiraw_calc_mean_by_hist(uint32_t *hist, const unsigned len,
                               const unsigned npixs)
{
    unsigned i;
    uint_fast32_t sum = 0;

#pragma omp parallel for reduction(+:sum)
    for (i = 0; i < len; i ++)
        sum += i * hist[i];

    return (float) sum / npixs;
}

/*
 * M. V. Shirvaikar, "An optimal measure for camera focus and exposure",
 *   Thirty-Sixth Southeastern Symposium on System Theory,
 *   2004. Proceedings of the, 2004, pp. 472-475.
 */
float rpiraw_calc_acm_by_hist(uint32_t *hist, const unsigned len,
                              const unsigned npixs, const float mean)
{
    unsigned i;
    float sum = 0.0;

#pragma omp parallel for reduction(+:sum)
    for (i = 0; i < len; i ++)
        sum += fabsf(mean - i) * hist[i];

    return (float) sum / npixs;
}
