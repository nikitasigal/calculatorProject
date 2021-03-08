#include "mapFunctions.h"

void initOperations(struct MapOperations *m) {
	for (int i = 0; i < MAP_SIZE; ++i) {
		m[i].key[0] = 0;
		m[i].operation = NULL;
		m[i].empty = 1;
	}
}

void insertOperation(struct MapOperations *m, char *key, void (*ptr)(struct NodeComplex **)) {
	unsigned int id = hash(key);

	if (m[id].empty) {
		m[id].empty = 0;
		strcpy(m[id].key, key);
		m[id].operation = ptr;
		return;
	}

	unsigned int start = id;
	id = (id + 1) % MAP_SIZE;
	while (id != start)
		if (m[id].empty) {
			m[id].empty = 0;
			strcpy(m[id].key, key);
			m[id].operation = ptr;
			return;
		} else
			id = (id + 1) % MAP_SIZE;

	printf("Critical error: unable to insertOperation() - possible overflow");
	exit(EXIT_FAILURE);
}

//Returns the INDEX of the requested operation in the map or INT_MAX if the operation is not found
unsigned int findOperation(struct MapOperations m[MAP_SIZE], char key[KEY_SIZE]) {
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

	//Requested operation is not found, but this does NOT throw exceptions
	//INT_MAX is used to signal that current calculation element is NOT a operation
	return INT_MAX;
}

