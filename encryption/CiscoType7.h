#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const char *constant = "dsfd;kfoA,.iyewrkldJKDHSUBsgvca69834ncxv9873254k;fg87";

int encode(char *msg, char *hash, int length) {

	if (length == 0) {
		printf("Message must have length greater than 0\n");
		return 1;
	}

	// Generate salt
	uint8_t smallsalt;
	arc4random_buf(&smallsalt, sizeof(smallsalt));
	smallsalt = smallsalt % 16;
	int salt = (int) smallsalt;

	// Encrypt message
	char charhash[length];
	for (int i = 0; i < length; i++) {
		charhash[i] = msg[i] ^ constant[(i + salt) % 53];
	}
	
	// Convert to output format
	char *ptr = hash;
	// Start string with 2 digit hex value of salt
	snprintf(ptr, 4, "%02X", (unsigned char)salt);
	ptr += 2;
	// Convert ascii to string of 2 digit hexidecimal values
	for (int i = 0; i < length; i++) {
		snprintf(ptr, 2*length + 3 - 2*i, "%02X", (unsigned char)charhash[i]);
		ptr += 2;
	}
	*ptr = '\0';

	return 0;
}

int hex_to_char(char c) {
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'A' && c <= 'f') return c - 'A' + 10;
	return -1;
}

int decode(char *hash, char* msg, int hash_length) {
	// convert character string to numerical values.
	int temp[hash_length/2];
	for (int i = 0; i < hash_length/2; i++) {
		int high = hex_to_char(hash[2*i]);
		int low = hex_to_char(hash[2*i+1]);

		if ((high == -1) || (low == -1)) {
			return 1;
		}

		temp[i] = (high << 4) | low;
	}

	int salt = temp[0];

	for (int i = 1; i < hash_length/2; i++) {
		temp[i] = temp[i] ^ constant[((i-1) + salt) % 53];
	}

	char *ptr = msg;
	for (int i = 1; i < hash_length/2; i++) {
		snprintf(ptr, hash_length/2 - (i-1), "%c", (unsigned char) temp[i]);
		ptr++;
	}
	*ptr = '\0';

	return 0;
}


