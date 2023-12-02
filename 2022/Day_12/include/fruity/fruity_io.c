/* fruity_io.c
 *
 * MIT License
 *
 * Copyright (c) 2022 Chris Schick
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "fruity_io.h"
#include "fruity.h"
#include <stdio.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Global Variables
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

char fruity_io_delimiter = FRUITY_IO_DELIMITER_DEFAULT;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Row Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
fruity_io_newline(void* row_data, void* col_data)
{
    (void)row_data;
    (void)col_data;

    const char* newline = fruity_io_delimiter != '\0' ? "\b \b\n" : "\n";

    printf("%s", newline);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Column Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
fruity_io_print_int(struct fruity_2d_cell cell, void* data)
{
    const int* p = cell.ptr;
    int width = data ? *(int*)data : 0;

    printf("%*d%c", width, *p, fruity_io_delimiter);
}

void
fruity_io_print_char(struct fruity_2d_cell cell, void* data)
{
    const char* p = cell.ptr;
    (void)data;

    printf("%c%c", *p, fruity_io_delimiter);
}
