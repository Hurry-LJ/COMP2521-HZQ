// Implementation of the Map ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"

struct map {
    int nV;    // vertices
	int nE;    // edges
	int **edges;  
	char **name;
};

#define MAX_CITY_NAME_1 30


/**
 * Creates a new map with the given number of cities
 * Assumes that numCities is positive
 */
Map MapNew(int numCities) {
    assert(numCities >= 0);

    Map m = malloc(sizeof(*m));
	if (m == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    m->nV = numCities;
    m->nE = 0;
    
    
    m->edges = malloc(numCities * sizeof(int *));
	
	if (m->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    } 
    
    for (int i = 0; i < numCities; i++) {
        m->edges[i] = calloc(numCities, sizeof(int));
        if (m->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < numCities; i++){
        for(int j = 0; j < numCities; j++){
            m->edges[i][j] = 0;
        }
    }
    
    char **name = (char **) malloc(numCities * sizeof(char * ));

    for(int i = 0; i < numCities; i++){
        name[i] = (char * )malloc(MAX_CITY_NAME_1 * sizeof(char));
    }

    m->name = name;
    // TODO
    return m;
}

/**
 * Frees all memory associated with the given map
 */
void MapFree(Map m) {
    assert(m != NULL);

    for (int i = 0; i < m->nV; i++) {
        free(m->edges[i]);
    }
    free(m->edges);
    free(m);

}

/**
 * Returns the number of cities on the given map
 */
int MapNumCities(Map m) {

    if (m != NULL ) {
        return m->nV;
    }
    return 0;
}

/**
 * Returns the number of roads on the given map
 */
int MapNumRoads(Map m) {
    if (m != NULL) {
        return m->nE;
    }
    return 0;
}

/**
 * Inserts a road between two cities with the given length
 * Does nothing if there is already a road between the two cities
 * Assumes that the cities are valid and are not the same
 * Assumes that the length of the road is positive
 */
void MapInsertRoad(Map m, int city1, int city2, int length) {
    
    m->edges[city1][city2] = length;
    m->edges[city2][city1] = length;
    m->nE++;

}

/**
 * Sets the name of the given city
 */
void MapSetName(Map m, int city, char *name) {
    m->name[city]= strdup(name);
}

/**
 * Returns the name of the given city
 */
char *MapGetName(Map m, int city) {

    char *cityName = m->name[city];
    if (cityName != NULL) {
        return cityName;
    }
    return "unnamed";
}

/**
 * Checks if there is a road between the two given cities
 * Returns the length of the road if there is a road, and 0 otherwise
 */
int MapContainsRoad(Map m, int city1, int city2) {
    return m->edges[city1][city2];
}

/**
 * Returns the number of roads connected to the given city and stores
 * them in the given roads array. The `from` field should be equal to
 * `city` for all the roads in the array.
 * Assumes that the roads array is large enough to store all the roads
 */
int MapGetRoadsFrom(Map m, int city, Road roads[]) {
    //GraphShow(m);
    int numRoads = 0;
    for(int i = 0; i < m->nV; i++){
        if(MapContainsRoad(m, city, i) != 0){
            roads[numRoads].from = city;
            roads[numRoads].to = i;
            roads[numRoads].length = MapContainsRoad(m, city, i);
            numRoads++;
        }
    }
    return numRoads;
}

/**
 * Displays the map
 * !!! DO NOT EDIT THIS FUNCTION !!!
 * This function will work once the other functions are working
 */
void MapShow(Map m) {
    printf("Number of cities: %d\n", MapNumCities(m));
    printf("Number of roads: %d\n", MapNumRoads(m));
    
    Road *roads = malloc(MapNumRoads(m) * sizeof(Road));
    if (roads == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);    
    }
    
    for (int i = 0; i < MapNumCities(m); i++) {
        printf("[%d] %s has roads to:", i, MapGetName(m, i));
        int numRoads = MapGetRoadsFrom(m, i, roads);
        for (int j = 0; j < numRoads; j++) {
            if (j > 0) {
                printf(",");
            }
            printf(" [%d] %s (%d)", roads[j].to, MapGetName(m, roads[j].to),
                   roads[j].length);
        }
        printf("\n");
    }
    
    free(roads);
}

