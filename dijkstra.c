#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_CIDADES 20
#define TAM_NOME 50

typedef struct {
    int destino;
    int distancia;
    struct Rota* proxima;
} Rota;

typedef struct {
    char nome[TAM_NOME];
    Rota* rotas;
} Cidade;

Cidade cidades[MAX_CIDADES];
int numCidades = 0;

// Função para encontrar o índice de uma cidade pelo nome
int encontrarIndiceCidade(const char* nome) {
    for (int i = 0; i < numCidades; i++) {
        if (strcmp(cidades[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1; // Não encontrado
}

// Função para adicionar uma nova cidade
void adicionarCidade() {
    if (numCidades >= MAX_CIDADES) {
        printf("Limite de cidades atingido!\n");
        return;
    }
    
    printf("Digite o nome da cidade: ");
    scanf("%s", cidades[numCidades].nome);
    cidades[numCidades].rotas = NULL;
    numCidades++;
    printf("Cidade adicionada com sucesso!\n");
}

// Função para adicionar uma rota entre cidades
void adicionarRota() {
    char origem[TAM_NOME], destino[TAM_NOME];
    int distancia;
    
    printf("Digite a cidade de origem: ");
    scanf("%s", origem);
    printf("Digite a cidade de destino: ");
    scanf("%s", destino);
    printf("Digite a distancia entre elas: ");
    scanf("%d", &distancia);
    
    int indiceOrigem = encontrarIndiceCidade(origem);
    int indiceDestino = encontrarIndiceCidade(destino);
    
    if (indiceOrigem == -1 || indiceDestino == -1) {
        printf("Cidade de origem ou destino não encontrada!\n");
        return;
    }
    
    // Adiciona rota de origem para destino
    Rota* novaRota = (Rota*)malloc(sizeof(Rota));
    novaRota->destino = indiceDestino;
    novaRota->distancia = distancia;
    novaRota->proxima = cidades[indiceOrigem].rotas;
    cidades[indiceOrigem].rotas = novaRota;
    
    // Adiciona rota de destino para origem (rota bidirecional)
    novaRota = (Rota*)malloc(sizeof(Rota));
    novaRota->destino = indiceOrigem;
    novaRota->distancia = distancia;
    novaRota->proxima = cidades[indiceDestino].rotas;
    cidades[indiceDestino].rotas = novaRota;
    
    printf("Rota adicionada com sucesso!\n");
}

// Função para imprimir todas as cidades e rotas
void listarCidadesRotas() {
    printf("\n--- Cidades e Rotas ---\n");
    for (int i = 0; i < numCidades; i++) {
        printf("%s -> ", cidades[i].nome);
        Rota* atual = cidades[i].rotas;
        while (atual != NULL) {
            printf("%s (%d) ", cidades[atual->destino].nome, atual->distancia);
            atual = atual->proxima;
            if (atual != NULL) printf("-> ");
        }
        printf("\n");
    }
}

// Função auxiliar para encontrar a cidade com menor distância não visitada
int encontrarMinimaDistancia(int distancias[], int visitados[]) {
    int min = INT_MAX, min_index;
    
    for (int v = 0; v < numCidades; v++) {
        if (!visitados[v] && distancias[v] <= min) {
            min = distancias[v];
            min_index = v;
        }
    }
    
    return min_index;
}

// Função para imprimir o caminho encontrado
void imprimirCaminho(int predecessores[], int destino) {
    if (predecessores[destino] == -1) {
        printf("%s", cidades[destino].nome);
        return;
    }
    imprimirCaminho(predecessores, predecessores[destino]);
    printf(" -> %s", cidades[destino].nome);
}

// Implementação do algoritmo de Dijkstra
void encontrarMenorCaminho() {
    char origem[TAM_NOME], destino[TAM_NOME];
    
    printf("Digite a cidade de origem: ");
    scanf("%s", origem);
    printf("Digite a cidade de destino: ");
    scanf("%s", destino);
    
    int indiceOrigem = encontrarIndiceCidade(origem);
    int indiceDestino = encontrarIndiceCidade(destino);
    
    if (indiceOrigem == -1 || indiceDestino == -1) {
        printf("Cidade de origem ou destino não encontrada!\n");
        return;
    }
    
    int distancias[MAX_CIDADES];
    int visitados[MAX_CIDADES];
    int predecessores[MAX_CIDADES];
    
    // Inicialização
    for (int i = 0; i < numCidades; i++) {
        distancias[i] = INT_MAX;
        visitados[i] = 0;
        predecessores[i] = -1;
    }
    distancias[indiceOrigem] = 0;
    
    // Encontrar o menor caminho para todos os vértices
    for (int count = 0; count < numCidades - 1; count++) {
        int u = encontrarMinimaDistancia(distancias, visitados);
        visitados[u] = 1;
        
        Rota* atual = cidades[u].rotas;
        while (atual != NULL) {
            int v = atual->destino;
            if (!visitados[v] && distancias[u] != INT_MAX && 
                distancias[u] + atual->distancia < distancias[v]) {
                distancias[v] = distancias[u] + atual->distancia;
                predecessores[v] = u;
            }
            atual = atual->proxima;
        }
    }
    
    // Imprimir o resultado
    if (distancias[indiceDestino] == INT_MAX) {
        printf("Não existe caminho entre %s e %s\n", origem, destino);
    } else {
        printf("Menor distancia entre %s e %s: %d\n", origem, destino, distancias[indiceDestino]);
        printf("Caminho: ");
        imprimirCaminho(predecessores, indiceDestino);
        printf("\n");
    }
}

// Função principal com menu
int main() {
    int opcao;
    
    do {
        printf("\n--- Sistema de Rotas Otimizadas ---\n");
        printf("1. Adicionar cidade\n");
        printf("2. Adicionar rota\n");
        printf("3. Listar cidades e rotas\n");
        printf("4. Encontrar menor caminho\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                adicionarCidade();
                break;
            case 2:
                adicionarRota();
                break;
            case 3:
                listarCidadesRotas();
                break;
            case 4:
                encontrarMenorCaminho();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);
    
    return 0;
}