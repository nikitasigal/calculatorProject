#include "hashing.h"

unsigned int hash(char key[KEY_SIZE]) {
	unsigned long long result = 0;
	unsigned long long multiplier = 1;
	for (int i = 0; i < strlen(key); ++i) {
		result = (result + (unsigned long long) key[i] * multiplier) % MAP_SIZE;
		multiplier *= HASH_MULTIPLIER;
	}
	return (unsigned int) result;
}