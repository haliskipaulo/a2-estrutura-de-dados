// CODIGO MENOR DISTANCA E DE DIKSTRA PEGO DO MATERIAL DO PROFESSOR

#include <stdio.h>
#include <limits.h> 

#define V 6 // Número de vértices (estações/paradas)

// Função para encontrar o vértice com a menor distância que ainda não foi visitado
// (Não precisa de alteração)
int minDistancia(int dist[], int visitado[]) {
    int min = INT_MAX, min_index;

    for (int v = 0; v < V; v++) {
        if (!visitado[v] && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}

// funcao para imprimir o caminho do destino ate a origem 
void imprimirCaminho(int antecessor[], int destino, const char* nomesEstacoes[]) {
    
    // se nao tem antecessor, chegamos na origem ou n tem caminho
    if (antecessor[destino] == -1 && destino != 0) { 
        printf("%s", nomesEstacoes[destino]);
        return;
    }
    if (antecessor[destino] == -1) {
         printf("%s", nomesEstacoes[destino]);
         return;
    }

    // print recursivo para imprimir na ordem correta
    imprimirCaminho(antecessor, antecessor[destino], nomesEstacoes);
    
    // imprime a atual estacao
    printf(" -> %s", nomesEstacoes[destino]);
}



void dijkstra(int grafo[V][V], int origem, int destino, const char* nomesEstacoes[]) {
    int dist[V];      // dist[i] armazena a menor distância de origem até i
    int visitado[V]; //é verdadeiro se o vértice já foi incluído
    int antecessor[V]; // armazena o antecessor para printar o caminho dps

    // Inicializa todas as distâncias, visitados e antecessores
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visitado[i] = 0;
        antecessor[i] = -1; // negativo pois n tem antecessor ainda
    }

    dist[origem] = 0; // A distância da origem para ela mesma é sempre 0

    // Encontra o caminho mais curto para todos os vértices
    for (int count = 0; count < V - 1; count++) {
        int u = minDistancia(dist, visitado);
        visitado[u] = 1;

        for (int v = 0; v < V; v++) {
            // Atualiza a distância se v não foi visitado, existe uma aresta de u para v,
            // e o novo caminho via u é menor que a distância atual de v.
            if (!visitado[v] && grafo[u][v] && dist[u] != INT_MAX &&
                dist[u] + grafo[u][v] < dist[v]) {
                dist[v] = dist[u] + grafo[u][v];
                antecessor[v] = u; // guarda o caminho
            }
        }
    }

    // imprime o melho caminho
    if (dist[destino] != INT_MAX) {
        printf("O melho caminho de '%s' para '%s' leva: %d minutos.\n",
               nomesEstacoes[origem], nomesEstacoes[destino], dist[destino]);
        
        // Imprime o caminho 
        printf("Caminho a seguir: ");
        imprimirCaminho(antecessor, destino, nomesEstacoes);
        printf("\n");

    } else {
        printf("Não existe um caminho de '%s' para '%s'.\n",
               nomesEstacoes[origem], nomesEstacoes[destino]);
    }
}

int main() {
    // nome estacoes
    const char* nomesEstacoes[V] = {
        "Terminal Cabral",
        "Estacao Central",
        "Shopping Estacao",
        "Terminal Campina",
        "Terminal Comprido",
        "Universidade"
    };

    // grafo do transporde
    int grafo[V][V] = {
        // De:                 Para: 0, 1, 2, 3, 4, 5
        /* 0: Terminal Cabral */   {0, 10, 0, 15, 0, 0},
        /* 1: Estacao Central */   {10, 0, 8, 0, 0, 0},
        /* 2: Shopping Estacao */  {0, 8, 0, 6, 2, 0},
        /* 3: Terminal Campina */  {15, 0, 6, 0, 5, 0},
        /* 4: Terminal Comprido */ {0, 0, 2, 5, 0, 12},
        /* 5: Universidade */      {0, 0, 0, 0, 12, 0}
    };

    int origem, destino;

    
    for (int i = 0; i < V; i++) {
        printf("%d: %s\n", i, nomesEstacoes[i]);
    }

    printf("\nNumero da estacao de partida: ");
    scanf("%d", &origem);

    printf("Numero da estacao de destino: ");
    scanf("%d", &destino);

    dijkstra(grafo, origem, destino, nomesEstacoes);

    return 0;
}