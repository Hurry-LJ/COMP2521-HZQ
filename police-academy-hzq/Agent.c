// Implementation of the Agent ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Agent.h"
#include "Map.h"
#include "Queue.h"


#define BUFFER 10
#define INFINITY 2147483647
#define UNDEFINED -1

// This struct stores information about an individual agent and can be
// used to store information that the agent needs to remember.
struct agent {
    char *name;
    int startLocation;
    int location;
    int maxStamina; // max stamina
    int stamina;    // current stamina
    int strategy;
    Map map;
    queue movePath;    //5 6 7 newDFS 
    
    int *dfs;
    // for DFS move
    bool *dfs_vis;
    bool *dfs_move_vis;
    int move_citys;
    // dij
    int *pre;
    queue shortPath;
    bool hasInformant;
    // bbb
    int visted[];
    
    // TODO: Add more fields here as needed
};
void dfs(Map m, Agent agent, int start);
void cleanDfsVis(Map m, Agent agent);
void cleanDfsMoveVis(Map m, Agent agent);

int GetTurns(Agent agent, int stam, int u, int v);
void Dijkstra(Agent agent, int start);
void ShortDFS(Agent agent, int pre[], int start, int ed);
static Move chooseRandomMove(Agent agent, Map m);

static int filterRoads(Agent agent, Road roads[], int numRoads,
                       Road legalRoads[]);
static Move chooseCLVMove(Agent agent, Map m);
static Move chooseDFSMove(Agent agent, Map m);
static int getLeastVisted(Agent agent, Road legalRoads[], int numLegalRoads);
static int getLeastStamina(Agent agent, Road legalRoads[], int numLegalRoads);


/**
 * Creates a new agent
 */
Agent AgentNew(int start, int stamina, int strategy, Map m, char *name) {
    if (start >= MapNumCities(m)) {
        fprintf(stderr, "error: starting city (%d) is invalid\n", start);
        exit(EXIT_FAILURE);
    }
    
    Agent agent = malloc(sizeof(struct agent) + sizeof(int) * MapNumCities(m));
    if (agent == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    agent->startLocation = start;
    agent->location = start;
    agent->maxStamina = stamina;
    agent->stamina = stamina;
    agent->strategy = strategy;
    agent->map = m;
    agent->name = strdup(name);

    agent->movePath = newQueue();
    agent->move_citys = 0;

    agent->shortPath = newQueue();
    agent->hasInformant = false;

    // initalise visted count array
    for (int i = 0; i < MapNumCities(m); i++) {
            agent->visted[i] = 0; 
    }
    // find dfs from starting point
    // cleanDfsVis(m, agent);
    // cleanDfsMoveVis(m, agent);
    // dfs(m, agent, start);
    return agent;
}

/**
 * Frees all memory associated with the agent
 * NOTE: You should not free the map because the map is owned by the
 *       main program, and the main program will free it
 */
void AgentFree(Agent agent) {
    // TODO: You may need to update this to free any extra memory you use
    dropQueue(agent->movePath);
    dropQueue(agent->shortPath);
    free(agent->name);
    free(agent);
}



void cleanDfsVis(Map m, Agent agent){
    int nV = MapNumCities(m);
	bool *visited = malloc(nV * sizeof(bool));
    for (int i = 0; i < nV; i++) {
        visited[i] = false;
    }
    agent->dfs_vis = visited;
}

void cleanDfsMoveVis(Map m, Agent agent){
    int nV = MapNumCities(m);
	bool *visited = malloc(nV * sizeof(bool));
    for (int i = 0; i < nV; i++) {
        visited[i] = false;
    }
    agent->dfs_move_vis = visited;
}

void dfs(Map m, Agent agent, int start){
    
    agent->dfs_vis[start] = true;
    int nV = MapNumCities(m);
   
    // printf("MapContainsRoad:%d->\n", MapContainsRoad(m, 3, 4));
    for(int i = 0; i < nV; i++){
        int minId = INFINITY;

        for(int j = 0; j < nV; j++){
            if(agent->dfs_vis[j] == false && MapContainsRoad(m, start, j) != 0){
                if(minId > j){
                    minId = j;
                }
            }
        }

        if(minId != INFINITY){
            QueueEnqueue(agent->movePath, minId);
            // printf("%d->", minId);
            dfs(m, agent, minId);
            QueueEnqueue(agent->movePath, start);
            // printf("%d->  ", start);
        }
    }
}

////////////////////////////////////////////////////////////////////////
// Making moves

/**
 * Calculates the agent's next move
 */
Move AgentGetNextMove(Agent agent, Map m) {
    switch (agent->strategy) {
        case STATIONARY: return (Move){agent->location, 0};
        case RANDOM:     return chooseRandomMove(agent, m);
        case CHEAPEST_LEAST_VISITED: return chooseCLVMove(agent, m);
        case DFS: return chooseDFSMove(agent, m);
        // TODO: Implement more strategies here
        default:
            printf("error: strategy not implemented yet\n");
            exit(EXIT_FAILURE);
    }
}

static Move chooseRandomMove(Agent agent, Map m) {
    Move move;
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));

    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    if (numLegalRoads > 0) {

        // Sort the roads using insertion sort
        for (int i = 1; i < numLegalRoads; i++) {
            Road r = legalRoads[i];
            int j = i;
            while (j > 0 && r.to < legalRoads[j - 1].to) {
                legalRoads[j] = legalRoads[j - 1];
                j--;
            }
            legalRoads[j] = r;
        }
        
        // nextMove is randomly chosen from the legal roads
        int k = rand() % numLegalRoads;
        move = (Move){legalRoads[k].to, legalRoads[k].length};
    } else {
        // The agent must stay in the same location
        move = (Move){agent->location, 0};
    }
    
    free(legalRoads);
    free(roads);
    return move;
}

// Takes an array with all the possible roads and puts the ones the agent
// has enough stamina for into the legalRoads array
// Returns the number of legal roads
static int filterRoads(Agent agent, Road roads[], int numRoads,
                       Road legalRoads[]) {
    int numLegalRoads = 0;
    for (int i = 0; i < numRoads; i++) {
        if (roads[i].length <= agent->stamina) {
            legalRoads[numLegalRoads++] = roads[i];
        }
    }
    return numLegalRoads;
}

static Move chooseCLVMove(Agent agent, Map m) {
    
    if(agent->hasInformant){
        return chooseDFSMove(agent, m);
    }

    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));
    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);
    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);
    Move move;
    bool legalMoveFound = false;
    if (numLegalRoads > 0) {

        numLegalRoads = getLeastVisted(agent, legalRoads, numLegalRoads);
        numLegalRoads = getLeastStamina(agent, legalRoads, numLegalRoads);
        move = (Move){legalRoads[0].to, legalRoads[0].length};
        legalMoveFound = true;

    }
    if (!legalMoveFound) {
        // The agent must stay in the same location
        move = (Move){agent->location, 0};
    }
    agent->visted[agent->location]++;

    free(legalRoads);
    free(roads);
    
    return move;
    }


static int getLeastVisted(Agent agent, Road legalRoads[], int numLegalRoads) {

    // find least visted cities
    int LVRecord = INFINITY;
    for (int i = 0; i < numLegalRoads; i++) {
        if (agent->visted[legalRoads[i].to] < LVRecord) {
            LVRecord = agent->visted[legalRoads[i].to];
        }
    }

    // assemble array of least cities
    int roads = 0;
    for (int i = 0; i < numLegalRoads; i++) {
        if (agent->visted[legalRoads[i].to] == LVRecord) {
            legalRoads[roads++] = legalRoads[i];
        }
    }
    // printf("%d\n",roads);
    return roads;

}

static int getLeastStamina(Agent agent, Road legalRoads[], int numLegalRoads) {


    // find least visted cities
    int LSRecord = INFINITY;
    
    for (int i = 0; i < numLegalRoads; i++) {
        if (legalRoads[i].length < LSRecord) {
            LSRecord = legalRoads[i].length;
        }
    }

    int roads = 0;
    for (int i = 0; i < numLegalRoads; i++) {
        if (legalRoads[i].length == LSRecord) {
            legalRoads[roads++] = legalRoads[i];
        }
    }
    return roads;

}

static Move chooseDFSMove(Agent agent, Map m) {
    
    
    Move move;
    
    if(agent->hasInformant == false){
        if ( QueueIsEmpty(agent->movePath) || agent->move_citys == MapNumCities(m)) {
            cleanDfsVis(m, agent);
            cleanDfsMoveVis(m, agent);
            agent->move_citys = 0;
            dfs(m, agent, agent->location);
        }

        int possibleCity = peekQueue(agent->movePath);
        int dist = MapContainsRoad(agent->map, possibleCity, agent->location);

        if (agent->stamina >= dist) {
            int city = QueueDequeue(agent->movePath);
            move = (Move){city, dist};

            if(agent->dfs_move_vis[city] == false){
                agent->move_citys++;
                agent->dfs_move_vis[city] = true;
            }
        } else {
            // The agent must stay in the same location
            move = (Move){agent->location, 0};
        }
    }else{
        
        // > update (agent->location)

        int possibleCity = peekQueue(agent->shortPath);
        int dist = MapContainsRoad(agent->map, possibleCity, agent->location);
        

        if (agent->stamina >= dist) {
            int city = QueueDequeue(agent->shortPath);
            move = (Move){city, dist};
            if(QueueIsEmpty(agent->shortPath)){
                agent->hasInformant = false;
            }
        } else {
            // The agent must stay in the same location
            move = (Move){agent->location, 0};
        }
    }

    return move;
}

/**
 * Executes a given move by updating the agent's internal state
 */
void AgentMakeNextMove(Agent agent, Move move) {
    if (move.to == agent->location) {
        agent->stamina = agent->maxStamina;
    } else {
        agent->stamina -= move.staminaCost;
    }
    agent->location = move.to;
    

    // TODO: You may need to add to this to handle different strategies
}

////////////////////////////////////////////////////////////////////////
// Gets information about the agent
// NOTE: It is expected that these functions do not need to be modified

/**
 * Gets the name of the agent
 */
char *AgentName(Agent agent) {
    return agent->name;
}

/**
 * Gets the current location of the agent
 */
int AgentLocation(Agent agent) {
    return agent->location;
}

/**
 * Gets the amount of stamina the agent currently has
 */
int AgentStamina(Agent agent) {
    return agent->stamina;
}

////////////////////////////////////////////////////////////////////////
// Learning information
void ShortDFS(Agent agent, int pre[], int start, int ed){
    if(ed == start){
        QueueEnqueue(agent->shortPath, ed);
        return;
    }
    ShortDFS(agent, pre, start, pre[ed]);
    if(ed != start){
        QueueEnqueue(agent->shortPath, ed);
    }
}

int GetTurns(Agent agent, int stam, int u, int v){
    int dist = MapContainsRoad(agent->map, u, v);
    if(dist == 0) return INFINITY;
    int result = 1;
    while(stam < 0){
        stam = agent->maxStamina;
        result++;
    }
    return result;
}

void Dijkstra(Agent agent, int start){
    int nV = MapNumCities(agent->map);
    int *turns = malloc(nV * sizeof(int));
    int *dist = malloc(nV * sizeof(int));
    int *stam = malloc(nV * sizeof(int));

    int *pre = malloc(nV * sizeof(int));
    bool *visted = malloc(nV * sizeof(bool));
    for (int i = 0; i < nV; i++) {
        
        if(MapContainsRoad(agent->map, start, i) != 0){
            stam[i] = agent->stamina - MapContainsRoad(agent->map, start, i);
            int turn = 1;
            while(stam[i] < 0){
                stam[i] = agent->maxStamina;
                turn++;
            }
            turns[i] = turn;
            dist[i] = MapContainsRoad(agent->map, start, i);
        }
        else{
            turns[i] = INFINITY;
            dist[i] = INFINITY;
            stam[i] = 0;
        }
        
        pre[i] = i;
        visted[i] = false;
    }
    turns[start] = 0;
    dist[start] = 0;
    stam[start] = agent->stamina;

    for(int i = 0; i < nV; i++){
        int u = -1, MIN = INFINITY;
        for(int j = 0; j < nV; j++){
            if(visted[j] == false && turns[j] < MIN){
                u = j;
                MIN = turns[j];
            }
        }
        if(u == -1) return;
        visted[u] = true;
        for(int v = 0; v < nV; v++){
            if(visted[v] == false && MapContainsRoad(agent->map, u, v) != 0){
                if(turns[u] + GetTurns(agent, stam[u], u, v) < turns[v]){
                    turns[v] = turns[u] + GetTurns(agent, stam[u], u, v);
                    stam[v] = stam[u] - MapContainsRoad(agent->map, u, v);
                    int t = 0;
                    while(stam[v] < 0){
                        stam[v] = agent->maxStamina;
                        t++;
                    }
                    dist[v] = dist[u] + MapContainsRoad(agent->map, u, v);
                    pre[v] = u;
                }
                else if(turns[u] + GetTurns(agent, stam[u], u, v) == turns[v]){
                    if(dist[u] + MapContainsRoad(agent->map, u, v) < dist[v]){
                        dist[v] = dist[u] + MapContainsRoad(agent->map, u, v);
                        pre[v] = u;
                    }
                    else if(dist[u] + MapContainsRoad(agent->map, u, v) == dist[v]){
                        pre[v] = u;
                    }
                }
            }
        }
    }

    agent->pre = pre;
    
}

/**
 * Tells the agent where the thief is
 */
void AgentGainInfo(Agent agent, int thiefLocation) {
    agent->hasInformant = true;
    if(QueueIsEmpty(agent->shortPath)){
        Dijkstra(agent, agent->location);
        ShortDFS(agent, agent->pre, agent->location, thiefLocation);
        QueueDequeue(agent->shortPath);
        // AgentShow(agent);
    }    
    
}

////////////////////////////////////////////////////////////////////////
// Displaying state

/**
 * Prints information about the agent (for debugging purposes)
 */
void AgentShow(Agent agent) {
    printf("%s\n", agent->name);
    queue q = agent->shortPath;

    // TODO: You can implement this function however you want
    //       You can leave this function blank if you want
    while (!QueueIsEmpty(q))
    {
        /* code */
        int p = QueueDequeue(q);
        printf("%d->",p);
        
    }
    printf("\n");
    
}