#include "mapComplex.h"

void initVariables(struct MapComplex *m) {
	for (int i = 0; i < MAP_SIZE; ++i) {
		m[i].key[0] = 0;
		m[i].value = 0;
		m[i].empty = 1;
	}
}

void insertVariable(struct MapComplex *m, char *key, complex long double value) {
	unsigned int id = hash(key);

	if (m[id].empty) {
		m[id].empty = 0;
		strcpy(m[id].key, key);
		m[id].value = value;
		return;
	}

	unsigned int start = id;
	id = (id + 1) % MAP_SIZE;
	while (id != start)
		if (m[id].empty) {
			m[id].empty = 0;
			strcpy(m[id].key, key);
			m[id].value = value;
			return;
		} else
			id = (id + 1) % MAP_SIZE;

	printf("Critical error: unable to insertVariable() - possible overflow");
	exit(EXIT_FAILURE);
}

//Returns the VALUE of the requested variable or throws exception if the variable is not found
unsigned int findVariable(struct MapComplex *m, char *key) {
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
