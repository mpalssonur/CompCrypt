#include <math.h>
#include <stdint.h>
#include <stdio.h>

uint32_t leftrotate(uint32_t a, uint32_t b) {
  return (a << b) | (a >> (32 - b));
}

uint32_t chars_to_word(char *msg, uint32_t g) {
  return (((uint32_t)msg[g]) << 24) + (((uint32_t)msg[g]) << 16) +
         (((uint32_t)msg[g]) << 8) + ((uint32_t)msg[g]);
}

int md5_encode(char *msg, char *hash, int length) {

  int padding = (56 - ((length + 1) % 64)) % 64;
  int padded_length = length + padding;
  int chunk_num = padded_length / 64;

  char padded_msg[padded_length];
  int t = 0;
  // Copy message
  while (t < length) {
    padded_msg[t] = msg[t];
    t++;
  }
  // Append 0x80
  padded_msg[t] = (unsigned char)0x80;
  t++;
  // Pad with 0
  while (t < padded_length) {
    padded_msg[t] = (unsigned char)0;
    t++;
  }

  uint32_t s[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                    5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
                    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};

  uint32_t K[64];
  for (int i = 0; i < 64; i++) {
    K[i] = (uint32_t)(pow(2.0, 32) * fabs(sin(i + 1.0)));
  }

  uint32_t a0 = 0x67452312;
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

  uint32_t temp, mask, block, offset;
  for (int i = 0; i < 16; i++) {
    block = i / 4;
    offset = i % 4;
    switch (block) {
    case 0:
      temp = a0;
      break;
    case 1:
      temp = b0;
      break;
    case 2:
      temp = c0;
      break;
    case 3:
      temp = d0;
      break;
    }
    if (offset == 0) {
      mask = 0x000000FF;
    } else
      mask = mask * 0x100;

    hash[i] = (unsigned char)(((temp & mask) >> (offset * 8)) & 0xFF);
    printf("temp: %x, mask: %08x, hash char: %02x\n", temp, mask, hash[i]);
  }

  return 0;
}
