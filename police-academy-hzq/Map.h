// Interface to the Map ADT
// Cities are identified by integers between 0 and N - 1 where N is the
// number of cities. All roads are bidirectional.
// Cities can be given names via the MapSetName function.

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

typedef struct {
    int from;
    int to;
    int length;
} Road;

typedef struct map *Map;

/**
 * Creates a new map with the given number of cities
 * Assumes that numCities is positive
 */
Map MapNew(int numCities);

/**
 * Frees all memory associated with the given map
 */
void MapFree(Map m);

/**
 * Returns the number of cities on the given map
 */
int MapNumCities(Map m);

/**
 * Returns the number of roads on the given map
 */
int MapNumRoads(Map m);

/**
 * Inserts a road between two cities with the given length
 * Does nothing if there is already a road between the two cities
 * Assumes that the cities are valid and are not the same
 * Assumes that the length of the road is positive
 */
void MapInsertRoad(Map m, int city1, int city2, int length);

/**
 * Sets the name of the given city
 */
void MapSetName(Map m, int city, char *name);

/**
 * Returns the name of the given city
 */
char *MapGetName(Map m, int city);

/**
 * Checks if there is a road between the two given cities
 * Returns the length of the road if there is a road, and 0 otherwise
 */
int MapContainsRoad(Map m, int city1, int city2);

/**
 * Returns the number of roads connected to the given city and stores
 * them in the given roads array. The `from` field should be equal to
 * `city` for all the roads in the array.
 * Assumes that the roads array is large enough to store all the roads
 */
int MapGetRoadsFrom(Map m, int city, Road roads[]);

/**
 * Displays the map
 */
void MapShow(Map m);

#endif

