/* fruity_io.h
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
#pragma once

#include "fruity.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Global Variables
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * fruity_io_delimiter
 *
 * An optional character to set to delimit printed columns. Default value
 * is '\0' which is equivalent to no delimiter. It is recommended to reset
 * this variable to its default after printing.
 */
extern char fruity_io_delimiter;

enum { FRUITY_IO_DELIMITER_DEFAULT = '\0' };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Row Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * fruity_io_newline
 *
 * A utility function used to print a newline at the end of a row.
 * Matches `FruityRowFunction` signature.
 *
 * @param row_data      Optional user data (unused).
 * @param col_data      Optional user data (unused).
 */
void
fruity_io_newline(void* row_data, void* col_data);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Column Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**
 * fruity_io_print_int
 *
 * A utility function used to print integers in a 2D array. Matches
 * `FruityColFunctionConst` signature. Takes a field width as an optional
 * 'data' parameter.
 *
 * @param element       The element to print.
 * @param data          A pointer to an integer to be used to optionally set
 *                      the field width (default width is 0).
 */
void
fruity_io_print_int(struct fruity_2d_cell cell, void* data);

/**
 * fruity_io_print_char
 *
 * A utility function used to print characters in a 2D array. Matches
 * `FruityColFunctionConst` signature.
 *
 * @param element       The element to print.
 * @param data          Unused.
 */
void
fruity_io_print_char(struct fruity_2d_cell cell, void* data);

