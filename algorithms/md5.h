#include <math.h>
#include <stdint.h>
#include <stdio.h>

uint32_t leftrotate(uint32_t a, uint32_t b) {
  return (a << b) | (a >> (32 - b));
}

uint32_t chars_to_word(unsigned char *msg, uint32_t g) {
  return (((uint32_t)msg[g]) << 24) + (((uint32_t)msg[g + 1]) << 16) +
         (((uint32_t)msg[g + 2]) << 8) + ((uint32_t)msg[g + 3]);
}

int md5_encode(char *msg, unsigned char *hash, int length) {

  int padded_length = ((length + 8) / 64 + 1) * 64;
  int chunk_num = padded_length / 64;
  printf("length: %d, padded length: %d, chunk number: %d\n", length,
         padded_length, chunk_num);

  unsigned char padded_msg[padded_length];
  int t = 0;
  // Copy message
  while (t < length) {
    padded_msg[t] = (unsigned char)msg[t];
    t++;
  }
  // Append 0x80
  padded_msg[t] = (unsigned char)0x80;
  t++;
  // Pad with 0
  while (t < (padded_length - 8)) {
    padded_msg[t] = (unsigned char)0;
    t++;
  }
  // Add length in little endian form
  uint64_t bits_len = ((uint64_t)length) * 8;
  for (int i = 0; i < 8; i++) {
    padded_msg[padded_length - 8 + i] =
        (unsigned char)((bits_len >> (i * 8)) & 0xFF);
  }

  uint32_t s[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

  uint32_t K[64];
  for (int i = 0; i < 64; i++) {
    K[i] = (uint32_t)(pow(2.0, 32) * fabs(sin(i + 1.0)));
  }

  uint32_t a0 = 0x67452301;
  uint32_t b0 = 0xefcdab89;
  uint32_t c0 = 0x98badcfe;
  uint32_t d0 = 0x10325476;

  for (int j = 0; j < chunk_num; j++) {
    int A = a0;
    int B = b0;
    int C = c0;
    int D = d0;
    for (uint32_t i = 0; i < 64; i++) {
      uint32_t F, g;
      if (i < 16) {
        F = (B & C) | ((~B) & D);
        g = i;
      } else if (i < 32) {
        F = (D & B) | ((~D) & C);
        g = (5 * i + 1) % 16;
      } else if (i < 48) {
        F = B ^ C ^ D;
        g = (3 * i + 5) % 16;
      } else {
        F = C ^ (B | (~D));
        g = (7 * i) % 16;
      }
      F = F + A + K[i] + chars_to_word(padded_msg, g);
      A = D;
      D = C;
      C = B;
      B = B + leftrotate(F, s[i]);
    }
    a0 = a0 + A;
    b0 = b0 + B;
    c0 = c0 + C;
    d0 = d0 + D;
  }
  printf("a0: %08x, b0: %08x, c0: %08x, d0: %08x\n", a0, b0, c0, d0);

  uint32_t temp[4] = {a0, b0, c0, d0};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      hash[i * 4 + j] = (temp[i] >> j * 8) & 0xFF;
    }
  }

  return 0;
}
