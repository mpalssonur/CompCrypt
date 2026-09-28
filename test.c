// Test file for development

#include "encryption/CiscoType7.h"

int main(void) {
  int flag;

  char *message = "password";
  int length = strlen(message);
  printf("Hashing: %s, length: %d\n", message, length);
  char hashed_message1[2 * length + 3];
  flag = encode(message, hashed_message1, length);
  if (flag == 0) {
    printf("Hashed Message: %s\n", hashed_message1);
  }

  message = "";
  length = strlen(message);
  printf("Hashing: %s, length: %d\n", message, length);
  char hashed_message2[2 * length + 3];
  flag = encode(message, hashed_message2, length);
  if (flag == 0) {
    printf("Hashed Message: %s\n", hashed_message2);
  }

  message = "jiaofejwiAOFNAWOFÆNAWIFOÆAWJFIOAWÆJFAWOKJOIJAsjiofaj";
  printf("Hashing: %s, length: %d\n", message, length);
  length = strlen(message);
  char hashed_message3[2 * length + 3];
  flag = encode(message, hashed_message3, length);
  if (flag == 0) {
    printf("Hashed Message: %s\n", hashed_message3);
  }
}
