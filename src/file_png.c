/*
 * Copyright (c) 2017 Sugizaki Yukimasa (ysugi@idein.jp)
 * All rights reserved.
 *
 * This software is licensed under a Modified (3-Clause) BSD License.
 * You should have received a copy of this license along with this
 * software. If not, contact the copyright holder above.
 */

#include "rpiraw.h"
#include <png.h>
#include <stdio.h>
#include <stdlib.h>

int rpiraw_write_png_rgb888(const char *filename,
                            uint8_t *src, const unsigned ld_src,
                            const unsigned width, const unsigned height)
{
    FILE *fp = NULL;
    png_structp pngp = NULL;
    png_infop infop = NULL;
    png_bytep *rows = NULL;
    unsigned i;

    rows = (png_bytep*) malloc(sizeof(png_bytep) * height);
    if (rows == NULL) {
        perror("malloc");
        return 1;
    }
    for (i = 0; i < height; i ++)
        rows[i] = (png_byte*) &src[i * ld_src * 3];

    fp = fopen(filename, "w");
    if (fp == NULL) {
        perror("fopen");
        free(rows);
        return 1;
    }

    pngp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    infop = png_create_info_struct(pngp);

    if (setjmp(png_jmpbuf(pngp))) {
        if (rows != NULL)
            free(rows);
        if (fp != NULL)
            if (fclose(fp) != 0)
                perror("fclose");
        return 1;
    }

    png_init_io(pngp, fp);
    png_set_IHDR(pngp, infop, width, height, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);
    png_write_info(pngp, infop);
    png_write_image(pngp, rows);
    free(rows);
    rows = NULL;

    png_write_end(pngp, infop);
    png_destroy_write_struct(&pngp, &infop);

    return 0;
}
