#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEIGHT 8
#define WIDTH 8
#define WALL '#'
#define SPACES '.' // visualmente melhor do que espacos em branco
#define ENTRY 'I'
#define EXIT 'O'

// IMPLEMENTACAO DO GRAFO ABAIXO
typedef struct {
    int num_vertex;
    int **matrix_adj; 
} Graph;

Graph* create_graph(int V) {
    Graph* graph = (Graph *) malloc(sizeof(Graph));
    if(graph == NULL) {
        printf("\nErro ao criar o grafo\n");
        exit(1);
    } 
    graph->num_vertex = V;

    graph->matrix_adj = (int**) malloc(V * sizeof(int*));

    if(graph->matrix_adj == NULL) {
        printf("\nErro ao alocar memoria\n");
        exit(1);
    } 

    for(int i=0; i<V; i++) {
        graph->matrix_adj[i] = (int*) calloc(V, sizeof(int));
        if(graph->matrix_adj[i] == NULL) {
            printf("\nErro ao alocar memoria\n");
            exit(1);
        }
    }

    return graph;
}

void add_edge(Graph* graph, int v1, int v2) {
    if(v1 < graph->num_vertex && v2 < graph->num_vertex) {
        graph->matrix_adj[v1][v2] = 1;
        graph->matrix_adj[v2][v1] = 1;
    }
}

void print_matrix(Graph* graph) {
    printf("   ");
    for (int i = 0; i < graph->num_vertex; i++) {
        printf("%2d ", i);
    }
    printf("\n---");
    for (int i = 0; i < graph->num_vertex; i++) {
        printf("---");
    }
    printf("\n");

    for (int i = 0; i < graph->num_vertex; i++) {
        printf("%2d|", i);
        for (int j = 0; j < graph->num_vertex; j++) {
            printf("%2d ", graph->matrix_adj[i][j]);
        }
        printf("\n");
    }
}

void delete_graph(Graph* graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->num_vertex; i++) {
        free(graph->matrix_adj[i]);
    }
    free(graph->matrix_adj);
    free(graph);
}

// IMPLEMENTACAO DO GRAFO ACIMA
// IMPLEMENTACAO DA DFS ABAIXO

typedef struct {
    int *items;
    int top;
    int capacity;
} Stack;

Stack* create_stack(int capacity) {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->items = (int*) malloc(stack->capacity * sizeof(int));
    return stack;
}

bool stack_is_empty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, int value) {
    if (stack->top == stack->capacity - 1) return; 
    stack->top++;
    stack->items[stack->top] = value;
}

int pop(Stack* stack) {
    if (stack_is_empty(stack)) return -1; 
    int item = stack->items[stack->top];
    stack->top--;
    return item;
}

void delete_stack(Stack* stack) {
    free(stack->items);
    free(stack);
}

void depth_first_search(Graph* graph, int init, int end) {
    int V = graph->num_vertex;
    Stack* stack = create_stack(V);
    bool* visited = (bool*) calloc(V, sizeof(bool));
    int* previous = (int*) malloc(V * sizeof(int));

    for(int i = 0; i < V; i++)
        previous[i] = -1;
    
    push(stack, init);
    
    while(!stack_is_empty(stack)) {
        int u = pop(stack);

        if(visited[u]) {
            continue;
        }

        visited[u] = true;

        if (u == end)
            break;
        
        for (int v = V - 1; v >= 0; v--) {
            if (graph->matrix_adj[u][v] == 1 && !visited[v]) {
                previous[v] = u;
                push(stack, v);
            }
        }    
    }

    if (visited[end]) {
        printf("caminho encontrado:\n");
        int path[V];
        int path_size = 0;
        for (int i = end; i != -1; i = previous[i]) {
            path[path_size++] = i;
        }
        for (int i = path_size - 1; i >= 0; i--) {
            printf("%d%s", path[i], (i > 0) ? " -> " : "\n");
        }
    } else {
        printf("nao foi encontrado um caminho da entrada ate a saida\n");
    }

    delete_stack(stack);
    free(visited);
    free(previous);
}


// IMPLEMENTACAO DA DFS ACIMA
// IMPLEMENTACAO DA BFS ABAIXO
typedef struct {
    int *items;
    int next;
    int back;
    int capacity;
} Queue;

Queue* create_queue(int capacity) {
    Queue* queue = (Queue*) malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->next = -1;
    queue->back = -1;
    queue->items = (int*) malloc(queue->capacity * sizeof(int));
    return queue;
}

bool is_void(Queue* queue) {
    return queue->back == -1;
}

void add_to_queue(Queue* queue, int value) {
    if(queue->back != queue->capacity - 1) {
        if(queue->next == -1)
            queue->next = 0;
        queue->back++;
        queue->items[queue->back] = value;
    }
    else {
        printf("\nfila esta cheia\n");
    }
}

int remove_to_queue(Queue* queue) {
    int item;
    if(is_void(queue)) {
        printf("\nfila vazia\n");
        item = -1;
    }
    else {
        item = queue->items[queue->next];
        queue->next++;
        if(queue->next > queue->back) 
            queue->next = queue->back = -1;
    }
    return item;
}

void delete_queue(Queue* queue) {
    free(queue->items);
    free(queue);
}

void breadth_first_search(Graph* graph, int init, int end) {
    int V = graph->num_vertex;
    
    Queue* queue = create_queue(V);

    bool* visited = (bool*) calloc(V, sizeof(bool));

    int* previous = (int*) malloc(V * sizeof(int));
    for (int i = 0; i < V; i++) {
        previous[i] = -1;
    }

    visited[init] = true;
    add_to_queue(queue, init);

    printf("\niniciando busca em largura de %d para %d\n", init, end);

    while (!is_void(queue)) {
        int u = remove_to_queue(queue);

        if (u == end) {
            break;
        }

        for (int v = 0; v < V; v++) {
            if (graph->matrix_adj[u][v] == 1 && !visited[v]) {
                visited[v] = true;
                previous[v] = u; 
                add_to_queue(queue, v);
            }
        }
    }

    if (previous[end] == -1) {
        printf("\nnao foi encontrado um caminho da entrada ate a saida\n");
    } else {
        printf("\ncaminho encontrado\n");
        printf("caminho: ");
        
        int path[V];
        int path_size = 0;
        
        for (int i = end; i != -1; i = previous[i]) {
            path[path_size++] = i;
        }
        
        for (int i = path_size - 1; i >= 0; i--) {
            printf("%d", path[i]);
            if (i > 0) {
                printf(" -> ");
            }
        }
        printf("\n");
    }

    delete_queue(queue);
    free(visited);
    free(previous);
}

// IMPLEMENTACAO DA BFS ACIMA



int main() {
    char labyrinth[HEIGHT][WIDTH] = {
    {ENTRY,  SPACES, SPACES, WALL,   SPACES, SPACES, SPACES, SPACES},
    {WALL,   WALL,   SPACES, WALL,   SPACES, WALL,   WALL,   SPACES},
    {SPACES, SPACES, SPACES, SPACES, SPACES, SPACES,   SPACES, SPACES},
    {SPACES, WALL,   WALL,   WALL,   WALL,   WALL,   SPACES, WALL},
    {SPACES, SPACES, SPACES, SPACES, SPACES, SPACES,   SPACES, SPACES},
    {WALL,   WALL,   WALL,   SPACES, WALL,   WALL,   WALL,   SPACES},
    {SPACES, SPACES, SPACES, SPACES, WALL,   SPACES, SPACES, SPACES},
    {SPACES, WALL,   WALL,   WALL,   WALL,   WALL,   WALL,   EXIT}
    };

    printf("Labirinto:\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c ", labyrinth[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int num_vertex = WIDTH * HEIGHT;
    Graph* labyrinth_graph = create_graph(num_vertex);

    int entry = -1, exit = -1;

    for(int i=0; i<HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int current_v = i * WIDTH + j;
            if(labyrinth[i][j] == ENTRY) entry = current_v;
            if(labyrinth[i][j] == EXIT) exit = current_v;


            if (labyrinth[i][j] != WALL) {
                // int v1 = i * WIDTH + j;

                if (j + 1 < WIDTH && labyrinth[i][j + 1] != WALL) {
                    //int v2 = i * WIDTH + (j + 1);
                    add_edge(labyrinth_graph, current_v, i * WIDTH + (j+1));
                }

                if (i + 1 < HEIGHT && labyrinth[i + 1][j] != WALL) {
                    //int v2 = (i + 1) * WIDTH + j;
                    add_edge(labyrinth_graph, current_v, (i+1) * WIDTH + j);
                }
            }
        }
    }

    printf("matriz de adjacencia:\n", num_vertex - 1);
    print_matrix(labyrinth_graph);

    
    if (entry != -1 && exit != -1) {
        printf("\nBusca em largura:\n");
        breadth_first_search(labyrinth_graph, entry, exit);
        printf("\nBusca em profundiade:\n");
        depth_first_search(labyrinth_graph, entry, exit);
    } else {
        printf("Entrada ou Saída não definida no labirinto!\n");
    }

    delete_graph(labyrinth_graph);
    return 0;
}