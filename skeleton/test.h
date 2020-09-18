#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

static inline int get_piece_of_a_word(int word, uint8_t start, uint8_t size)
{
    int mask = 1 << size;
    mask--;
    return (word >> start) & mask;
}
