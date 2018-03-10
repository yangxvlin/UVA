/* Xulin Yang, 904904, time: 2018-3-3 20:44:34, description: UVA p10099
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_VERTICES 100 /* maximum number of vertices */
#define DIRECTED 0
#define GUIDE 1

#define MIN(a, b) ((a<b) ? a : b)
#define MAX(a, b) ((a>b) ? a : b)

typedef struct edgenode edgenode_t;
typedef struct graph graph_t;

struct edgenode {
    int y; /* adjacency info */
    int weight; /* edge weight */
    edgenode_t *next; /* next edge in list */
};

struct graph {
    edgenode_t *edges[MAX_VERTICES]; /* adjacency info */
    int degree[MAX_VERTICES]; /* outdegree of each vertex */
    int nvertices; /* number of vertices in graph */
    int nedges; /* number of edges in graph */
    int directed; /* is the graph directed? */
};

typedef struct trip trip_t;

struct trip {
    int start;
    int end;
    int passenger;
};

int get_edge(graph_t *g, int start, int end);
int min_trips(graph_t *g, trip_t t);
void initialize_graph(graph_t *g, int directed);
void read_graph(graph_t *g, int directed);
void insert_edge(graph_t *g, int x, int y, int w, int directed);
void free_graph(graph_t *g);

int main(int argc, char **argv) {
    graph_t *g = (graph_t*)malloc(sizeof(graph_t));

    read_graph(g, DIRECTED);

    return 0;
}

int get_edge(graph_t *g, int start, int end) {
    int res = 0;
    edgenode_t *tmp = g->edges[start];

    // printf(">>>> \n");
    while(tmp != NULL) {
        // printf("%d ", tmp->y);
        if (tmp->y == end) {
            res = tmp->weight;
            return res;
        }
        tmp = tmp->next;
    }

    return res;
}

int min_trips(graph_t *g, trip_t t) {
    int **path = (int**)malloc(sizeof(int*) * g->nvertices), i, j, k, res;
    assert(path);

    for (i = 0; i < g->nvertices; i++) {
        path[i] = (int*)malloc(sizeof(int) * g->nvertices);
        assert(path[i]);
        for (j = 0; j < g->nvertices; j++) {
            // printf(">>> \n");
            path[i][j] = get_edge(g, i, j);

            
            if (path[i][j] == 0) {
                path[i][j] = -1;
            }
            
        }
    }


    for (i = 0; i < g->nvertices; i++) {
        for (j = 0; j < g->nvertices; j++) {
            printf("%d ", path[i][j]);
        }
        printf("\n");
    }



    for (k = 0; k < g->nvertices; k++) {
        for (i = 0; i < g->nvertices; i++) {
            for (j = 0; j < g->nvertices; j++) {
                // int temp = MIN(path[j][i], path[i][k]);
                // printf("%d %d\n", path[i][k], path[k][j]);
                if (path[i][k] > path[k][j])  
                    path[i][j] = path[k][j] > path[i][j] ? path[k][j] : path[i][j];  
                else  
                    path[i][j] = path[i][j] > path[i][k] ? path[i][j] : path[i][k]; 
            }
        }
    }

    printf("\n");
    for (i = 0; i < g->nvertices; i++) {
        for (j = 0; j < g->nvertices; j++) {
            printf("%d ", path[i][j]);
        }
        printf("\n");
    }


    res = path[t.start - 1][t.end - 1];
    printf("%d %d %d\n", t.start, t.end, res);

    for (i = 0; i < g->nvertices; i++) {
        free(path[i]);
        path[i] = NULL;
    }

    return res;
}

void initialize_graph(graph_t *g, int directed) {
    int i; /* counter */

    g->nedges = 0;
    g->directed = directed;

    for (i = 0; i < MAX_VERTICES; i++) {
        g->degree[i] = 0;
        g->edges[i] = NULL;
    }

    return;
}

void read_graph(graph_t *g, int directed) {
    int i, counter = 0;; /* counter */
    int m; /* number of edges */
    int x, y; /* vertices in edge (x, y) */
    int weight;
    trip_t trip;
    
    initialize_graph(g, directed);

    while(scanf("%d %d", &(g->nvertices), &m) && g->nvertices && m) {
        counter++;

        for (i = 0; i < m; i++) {
            scanf("%d %d %d", &x, &y, &weight);
            insert_edge(g, x-1, y-1, weight, directed);
        }
        scanf("%d %d %d", &(trip.start), &(trip.end), &(trip.passenger));
    
        printf("Scenario #%d\n", counter);
        int min_weight = min_trips(g, trip);
        printf("Minimum Number of Trips = %d\n\n", trip.passenger / min_weight + (trip.passenger %  min_weight != 0));

        free_graph(g);
    }

    return;
}

void insert_edge(graph_t *g, int x, int y, int w, int directed) {
    edgenode_t *tmp; /* temp pointer */

    tmp = (edgenode_t*)malloc(sizeof(edgenode_t)); /* allocate edgenode storage */
    assert(tmp != NULL);

    tmp->weight = w - GUIDE;
    tmp->next = g->edges[x];
    tmp->y = y;
    
    g->edges[x] = tmp; /* insert at head of list */

    g->degree[x] ++;

    if (!directed) {
        insert_edge(g, y, x, w, 1);
    } else {
        g->nedges ++;
    }

    return;
}

void free_graph(graph_t *g) {
    int i; /* counter */
    edgenode_t *tmp, *pre; /* temp pointer */

    for (i = 0; i < g->nvertices; i++) {
        tmp = g->edges[i];
        while(tmp != NULL) {
            pre = tmp;
            tmp = tmp->next;
            free(pre);
            pre = NULL;
        }

        g->edges[i] = NULL;
    }

    return;
}
