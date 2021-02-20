#include "mapFunctions.h"

void initMapFunctions(struct MapFunctions m[MAP_SIZE]) {
	for (int i = 0; i < MAP_SIZE; ++i) {
		m[i].key[0] = 0;
		m[i].function = NULL;
		m[i].empty = 1;
	}
}

void insertFunction(struct MapFunctions m[MAP_SIZE], char key[KEY_SIZE], complex double (*ptr)(struct StackComplex *)) {
	unsigned int id = hash(key);

	if (m[id].empty) {
		m[id].empty = 0;
		strcpy(m[id].key, key);
		m[id].function = ptr;
		return;
	}

	unsigned int start = id;
	id = (id + 1) % MAP_SIZE;
	while (id != start)
		if (m[id].empty) {
			m[id].empty = 0;
			strcpy(m[id].key, key);
			m[id].function = ptr;
			return;
		} else
			id = (id + 1) % MAP_SIZE;

	printf("Critical error: unable to insertFunction() - possible overflow");
	exit(EXIT_FAILURE);
}

//Returns the INDEX of the requested function in the map or INT_MAX if the function is not found
unsigned int findFunction(struct MapFunctions m[MAP_SIZE], char key[KEY_SIZE]) {
	unsigned int id = hash(key);

	if (!m[id].empty && !strcmp(m[id].key, key))
		return id;

	unsigned int start = id;
	id = (id + 1) % MAP_SIZE;
	while (id != start)
		if (!m[id].empty && !strcmp(m[id].key, key)) {
			return id;
		} else
			id = (id + 1) % MAP_SIZE;

	//Requested function is not found, but this does NOT throw exceptions
	//INT_MAX is used to signal that current calculation element is NOT a function
	return INT_MAX;
}
