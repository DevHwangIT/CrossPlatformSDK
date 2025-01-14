#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const int base64_reverse_table[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 0-15
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 16-31
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 32-47
    -1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, // 48-63
    60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6,          // 64-79
    7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,         // 80-95
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,         // 96-111
    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1          // 112-127
};

static const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// Base64 인코딩 함수
char* base64_encode(const unsigned char* input, size_t length) {
    size_t output_length = 4 * ((length + 2) / 3);
    char* output = (char*)malloc(output_length + 1);
    if (!output) return NULL;  // 메모리 할당 실패 시 NULL 반환

    int i = 0, j = 0;
    unsigned char char_array_3[3], char_array_4[4];

    while (length--) {
        char_array_3[i++] = *(input++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] >> 2) & 0x3F;
            char_array_4[1] = ((char_array_3[0] << 4) & 0x30) | ((char_array_3[1] >> 4) & 0x0F);
            char_array_4[2] = ((char_array_3[1] << 2) & 0x3C) | ((char_array_3[2] >> 6) & 0x03);
            char_array_4[3] = char_array_3[2] & 0x3F;

            for (i = 0; i < 4; i++) {
                output[j++] = base64_table[char_array_4[i]];
            }
            i = 0;
        }
    }

    if (i) {
        for (int k = i; k < 3; k++) {
            char_array_3[k] = '\0';
        }

        char_array_4[0] = (char_array_3[0] >> 2) & 0x3F;
        char_array_4[1] = ((char_array_3[0] << 4) & 0x30) | ((char_array_3[1] >> 4) & 0x0F);
        char_array_4[2] = ((char_array_3[1] << 2) & 0x3C) | ((char_array_3[2] >> 6) & 0x03);
        char_array_4[3] = char_array_3[2] & 0x3F;

        for (int k = 0; k < i + 1; k++) {
            output[j++] = base64_table[char_array_4[k]];
        }

        while (i++ < 3) {
            output[j++] = '=';  // '='는 패딩
        }
    }

    output[j] = '\0';
    return output;
}

// Base64 디코딩 함수
int base64_decode(const char* input, unsigned char* output) {
    int len = strlen(input);
    int i = 0, j = 0;
    unsigned char char_array_4[4], char_array_3[3];
    int output_index = 0;

    while (len--) {
        if (*input == '=' || *input == '\0') break;
        char_array_4[i++] = *(input++);
        if (i == 4) {
            char_array_3[0] = (base64_reverse_table[char_array_4[0]] << 2) | (base64_reverse_table[char_array_4[1]] >> 4);
            char_array_3[1] = (base64_reverse_table[char_array_4[1]] << 4) | (base64_reverse_table[char_array_4[2]] >> 2);
            char_array_3[2] = (base64_reverse_table[char_array_4[2]] << 6) | base64_reverse_table[char_array_4[3]];

            for (i = 0; i < 3; i++) {
                output[output_index++] = char_array_3[i];
            }
            i = 0;
        }
    }

    if (i) {
        for (int k = i; k < 4; k++) {
            char_array_4[k] = 0;
        }

        char_array_3[0] = (base64_reverse_table[char_array_4[0]] << 2) | (base64_reverse_table[char_array_4[1]] >> 4);
        char_array_3[1] = (base64_reverse_table[char_array_4[1]] << 4) | (base64_reverse_table[char_array_4[2]] >> 2);
        char_array_3[2] = (base64_reverse_table[char_array_4[2]] << 6) | base64_reverse_table[char_array_4[3]];

        for (int k = 0; k < i - 1; k++) {
            output[output_index++] = char_array_3[k];
        }
    }

    return output_index;
}
