#pragma once

#include <stdint.h>
#include <string.h>

// SHA-1 해시 함수 (순수 C 구현)
void sha1(const char* data, size_t len, char* out)
{
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xEFCDAB89;
    uint32_t h2 = 0x98BADCFE;
    uint32_t h3 = 0x10325476;
    uint32_t h4 = 0xC3D2E1F0;

    uint32_t w[80];
    uint32_t a, b, c, d, e;
    size_t i, j;

    char padded[64];
    size_t new_len = len + 1;
    while (new_len % 64 != 56) new_len++;
    padded[new_len] = 0x80;
    for (i = 0; i < len; ++i) padded[i] = data[i];
    for (i = len; i < new_len; ++i) padded[i] = 0;
    uint64_t bit_len = len * 8;
    for (i = 0; i < 8; ++i) padded[new_len + i] = (bit_len >> (56 - i * 8)) & 0xFF;

    for (i = 0; i < new_len / 64; ++i) {
        memcpy(w, padded + i * 64, 64);
        for (j = 16; j < 80; ++j) {
            w[j] = w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16];
            w[j] = (w[j] << 1) | (w[j] >> 31);
        }

        a = h0;
        b = h1;
        c = h2;
        d = h3;
        e = h4;

        for (j = 0; j < 80; ++j) {
            uint32_t f, k;
            if (j < 20) {
                f = (b & c) | (~b & d);
                k = 0x5A827999;
            }
            else if (j < 40) {
                f = b ^ c ^ d;
                k = 0x6ED9EBA1;
            }
            else if (j < 60) {
                f = (b & c) | (b & d) | (c & d);
                k = 0x8F1BBCDC;
            }
            else {
                f = b ^ c ^ d;
                k = 0xCA62C1D6;
            }

            uint32_t temp = (a << 5) | (a >> 27);
            temp += f + e + k + w[j];
            e = d;
            d = c;
            c = (b << 30) | (b >> 2);
            b = a;
            a = temp;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
    }

    out[0] = (h0 >> 24) & 0xFF;
    out[1] = (h0 >> 16) & 0xFF;
    out[2] = (h0 >> 8) & 0xFF;
    out[3] = h0 & 0xFF;
    out[4] = (h1 >> 24) & 0xFF;
    out[5] = (h1 >> 16) & 0xFF;
    out[6] = (h1 >> 8) & 0xFF;
    out[7] = h1 & 0xFF;
    out[8] = (h2 >> 24) & 0xFF;
    out[9] = (h2 >> 16) & 0xFF;
    out[10] = (h2 >> 8) & 0xFF;
    out[11] = h2 & 0xFF;
    out[12] = (h3 >> 24) & 0xFF;
    out[13] = (h3 >> 16) & 0xFF;
    out[14] = (h3 >> 8) & 0xFF;
    out[15] = h3 & 0xFF;
    out[16] = (h4 >> 24) & 0xFF;
    out[17] = (h4 >> 16) & 0xFF;
    out[18] = (h4 >> 8) & 0xFF;
    out[19] = h4 & 0xFF;
}

// PBKDF2 함수 (SHA-1 해시 기반)
void pbkdf2_sha1(const char* password, size_t password_len, const char* salt, size_t salt_len,
    int iterations, char* output, size_t output_len)
{
    const int bufSize = 20;

    char buf[bufSize];  // SHA-1 출력 크기
    char* block;
    size_t block_index = 0;
    size_t remaining = output_len;

    for (block_index = 0; remaining > 0; ++block_index)
    {
        char salt_block[64];  // SHA-1 입력 크기
        char result[bufSize];      // SHA-1 해시 결과

        // Salt + 블록 인덱스 추가
        memcpy(salt_block, salt, salt_len);
        salt_block[salt_len] = (block_index >> 24) & 0xFF;
        salt_block[salt_len + 1] = (block_index >> 16) & 0xFF;
        salt_block[salt_len + 2] = (block_index >> 8) & 0xFF;
        salt_block[salt_len + 3] = block_index & 0xFF;

        // 첫 번째 해시 계산
        sha1(salt_block, salt_len + 4, result);
        memcpy(buf, result, bufSize);

        // 반복적인 해시 계산 (iterations 횟수)
        for (int i = 1; i < iterations; ++i)
        {
            sha1(buf, bufSize, result);
            memcpy(buf, result, bufSize);
        }

        // 결과 저장
        size_t to_copy = (remaining > bufSize) ? bufSize : remaining;
        memcpy(output + block_index * bufSize, buf, to_copy);
        remaining -= to_copy;
    }
}