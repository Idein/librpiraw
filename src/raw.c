/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include "rpiraw.h"

#define ALIGN_UP(v, n) (((v) + (n) - 1) & ~((n) - 1))

unsigned rpiraw_width_raw8_to_raw10_rpi(const unsigned width)
{
    return ALIGN_UP((width * 5 + 3) >> 2, 32);
}
