/*
 * Copyright (c) 2023 Ian Marco Moffett and the VegaOS team.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of VegaOS nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define COUNTER_BUF_SIZE 27

/*
 * Dump counter.
 *
 * @counters: Counter list.
 * @i: Current index.
 */
static void
try_dump_count(uint32_t counters[COUNTER_BUF_SIZE], unsigned int i)
{
    char c;
    uint16_t count;

    if ((counters[i] & (1 << 31)) != 0) {
        c = counters[i] & 0xFF;
        count = (counters[i] >> 8) & 0xFFFF;
        printf("%d instances of %c\n", count, c);
    }
}

/*
 * Add char to counter
 *
 * @str: String to go through.
 * @counters: List of counters for each char.
 *
 * Returns sum of letters in `str'
 */
static size_t
add_to_counter(const char *str, uint32_t counters[COUNTER_BUF_SIZE])
{
    char tmp;
    uint16_t count;
    size_t idx, sum = 0;

    for (size_t i = 0; i < strlen(str); ++i) {
        tmp = str[i] | 32;
        if (tmp < 'a' && tmp > 'z')
            continue;

        idx = (size_t)tmp - 97;
        count = (counters[idx] >> 8) & 0xFFFF;
        if ((counters[idx] & (1 << 31)) == 0) {
            counters[idx] |= tmp;
            counters[idx] |= (1 << 31);
        }
        counters[idx] &= ~(count << 8);
        counters[idx] |= ((count + 1) << 8);
        ++sum;
    }

    for (int i = 0; i < COUNTER_BUF_SIZE; ++i) {
        try_dump_count(counters, i);
    }

    return sum;
}

int
main(int argc, char **argv)
{
    uint32_t counters[COUNTER_BUF_SIZE] = {0};
    size_t sum = 0;

    for (int i = 1; i < argc; ++i) {
        printf("-- INPUT %d --\n", i);
        sum += add_to_counter(argv[i], counters);
        printf("**TOTAL: %d**\n", sum);
    }

    return 0;
}
