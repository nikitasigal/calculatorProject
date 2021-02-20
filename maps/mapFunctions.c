#include "mapFunctions.h"

void initMapFunctions(struct MapFunctions m[MAP_SIZE]) {
	for (int i = 0; i < MAP_SIZE; ++i) {
		m[i].key[0] = 0;
		m[i].function = NULL;
		m[i].empty = 1;
	}
}

void insertFunction(struct MapFunctions m[MAP_SIZE], char *key, complex double (*ptr)(struct StackComplex *)) {
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
}

unsigned int findFunction(struct MapFunctions m[MAP_SIZE], char *key) {
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

	return INT_MAX;
}
