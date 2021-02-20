#include "mapComplex.h"

void initMapComplex(struct MapComplex *m) {
	for (int i = 0; i < MAP_SIZE; ++i) {
		m[i].key[0] = 0;
		m[i].value = 0;
		m[i].empty = 1;
	}
}

void insertComplex(struct MapComplex *m, char *key, complex double value) {
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
}

double complex getComplex(struct MapComplex *m, char *key) {
	unsigned int id = hash(key);

	if (!m[id].empty && !strcmp(m[id].key, key))
		return m[id].value;

	unsigned int start = id;
	id = (id + 1) % MAP_SIZE;
	while (id != start)
		if (!m[id].empty && !strcmp(m[id].key, key)) {
			return m[id].value;
		} else
			id = (id + 1) % MAP_SIZE;

	return 0;
}
