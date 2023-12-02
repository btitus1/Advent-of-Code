/* fruity.c
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
#include "fruity.h"
#include <stdlib.h>
#include <string.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity Management Functions
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void*
fruity_new(struct fruity_2d* pfs, size_t rows, size_t cols, size_t size)
{
    char** pp = malloc(rows * sizeof(char*) + rows * cols * size);

    if (pp) {
        char* arr = (char*)(pp + rows);
        for (size_t i = 0; i < rows; ++i)
            pp[i] = (arr + i * cols * size);
        pfs->data = pp;
        pfs->rows = rows;
        pfs->cols = cols;
        pfs->size = size;
    }

    return (void*)pp;
}

void*
fruity_copy(const struct fruity_2d* src, struct fruity_2d* dst)
{
    if (!fruity_new(dst, src->rows, src->cols, src->size))
        return NULL;

    const void* psrc = fruity_get(src, 0, 0);
    void* pdst = fruity_get_mut(dst, 0, 0);

    memcpy(pdst, psrc, src->size * src->rows * src->cols);
    return (void*)dst->data;
}

void
fruity_free(void* p)
{
    struct fruity_2d* pfs = p;

    free(pfs->data);
    memset(pfs, 0, sizeof(struct fruity_2d));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity inline getter symbols
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t
fruity_rows(const struct fruity_2d* pfs);

size_t
fruity_cols(const struct fruity_2d* pfs);

const void*
fruity_data(const struct fruity_2d* pfs);

void*
fruity_data_mut(struct fruity_2d* pfs);

const void*
fruity_get(const struct fruity_2d* pfs, size_t row, size_t col);

void*
fruity_get_mut(struct fruity_2d* pfs, size_t row, size_t col);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity Standard Algorithms
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void
fruity_init(struct fruity_2d* pfs, const void* value)
{
    char** p = pfs->data;
    const size_t sz = pfs->size;

    for (size_t i = 0; i < pfs->rows; ++i)
        for (size_t j = 0; j < pfs->cols; ++j)
            memcpy(&p[i][j * sz], value, sz);
}

void
fruity_foreach(const struct fruity_2d* pfs,
               FruityRowFunction row_func,
               void* row_data,
               FruityColFunction col_func,
               void* col_data)
{
    char** p = pfs->data;

    for (size_t i = 0; i < pfs->rows; ++i) {
        for (size_t j = 0; j < pfs->cols; ++j)
            if (col_func)
                col_func((struct fruity_2d_cell){
                        .ptr = &p[i][j*pfs->size],
                        .row = i,
                        .col = j,
                }, col_data);
        if (row_func)
            row_func(row_data, col_data);
    }
}

void
fruity_transform(struct fruity_2d* pfs,
                 FruityRowFunction row_func,
                 void* row_data,
                 FruityColFunction col_func,
                 void* col_data)
{
    char** p = pfs->data;

    for (size_t i = 0; i < pfs->rows; ++i) {
        for (size_t j = 0; j < pfs->cols; ++j)
            if (col_func)
                col_func((struct fruity_2d_cell){
                        .ptr = &p[i][j*pfs->size],
                        .row = i,
                        .col = j,
                }, col_data);
        if (row_func)
            row_func(row_data, col_data);
    }
}

int
fruity_count_if(const struct fruity_2d* pfs,
                FruityPredicate pred,
                void* data)
{
    int sum = 0;
    char** p = pfs->data;
    for (size_t i = 0; i < pfs->rows; ++i)
        for (size_t j = 0; j < pfs->cols; ++j) {
            struct fruity_2d_cell cell = {
                    .ptr = &p[i][j * pfs->size],
                    .row = i,
                    .col = j,
            };
            if (pred(cell, data))
                ++sum;
        }

    return sum;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Fruity Pathfinding
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int
fruity_adjacent_4(struct fruity_2d* pfs, size_t r, size_t c,
                  struct fruity_2d_cell adj[4])
{
    int count = 0;
    char** p = pfs->data;
    const size_t sz = pfs->size;

    if (r != 0)             // UP
        adj[count++] = (struct fruity_2d_cell){
                .ptr = &p[r - 1][c * sz],
                .row = r - 1,
                .col = c,
        };
    if (c + 1 < pfs->cols)  // RIGHT
        adj[count++] = (struct fruity_2d_cell){
                .ptr = &p[r][(c + 1) * sz],
                .row = r,
                .col = c + 1,
        };
    if (r + 1 < pfs->rows)  // DOWN
        adj[count++] = (struct fruity_2d_cell){
                .ptr = &p[r + 1][c * sz],
                .row = r + 1,
                .col = c,
        };
    if (c != 0)             // LEFT
        adj[count++] = (struct fruity_2d_cell){
                .ptr = &p[r][(c - 1) * sz],
                .row = r,
                .col = c - 1,
        };

    return count;
}