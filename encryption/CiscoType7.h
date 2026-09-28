#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const char *constant = "dsfd;kfoA,.iyewrkldJKDHSUBsgvca69834ncxv9873254k;fg87";

int encode(char *msg, char *hash, int length) {

	if(length == 0) {
		printf("Message must have length greater than 0\n");
		return 1;
	}
	uint8_t smallsalt;

	arc4random_buf(&smallsalt, sizeof(smallsalt));
	smallsalt = smallsalt % 16;
	int salt = (int) smallsalt;

	char charhash[length];
	int i,j;
	for(i = 0; i < length; i++){
		j = (i + salt) % 53;
		charhash[i] = msg[i] ^ constant[j];
	}
	
	char *ptr = hash;
	snprintf(ptr, 4, "%02X", (unsigned char)salt);
	ptr += 2;
	for(i = 0; i < length; i++) {
		snprintf(ptr, 2*length + 3 - 2*i, "%02X", (unsigned char)charhash[i]);
		ptr += 2;
	}
	*ptr = '\0';

	return 0;
}
