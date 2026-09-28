// Test file for development

#include "algorithms/CiscoType7.h"
#include "algorithms/md5.h"

void test_cisco_type7(char *message, int length) {
  printf("================================================================\n");
  printf("\n");

  printf("Hashing: %s, length: %d\n", message, length);

  char hashed_message1[2 * length + 3];
  int flag = cisco7_encode(message, hashed_message1, length);

  if (flag == 0) {
    printf("Hashed Message: %s\n", hashed_message1);

    char decoded_message[length + 1];
    flag = cisco7_decode(hashed_message1, decoded_message, 2 * length + 2);

    if (flag == 0) {
      printf("Decoded Message: %s\n", decoded_message);
    } else {
      printf("Decoding Failed\n");
    }

  } else {
    printf("Hash Failed\n");
  }

  printf("\n");
}

int main(void) {
  char *msg = "The quick brown fox jumps over the lazy dog";
  int length = strlen(msg);
  unsigned char hash[16];
  md5_encode(msg, hash, length);
  for (int i = 0; i < 16; i++)
    printf("%02x", hash[i]);
  printf("\n");
}
