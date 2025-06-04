//-------------------------------------------------------
// Aluno: Vitor G. J. de Carvalho                       |
// RGM: 33046794                                        |
// Disciplina: Estruturas de Dados avancadas            |
// Professor: Marcelo T. Uemura                         |
// Data: 03/06/2025                                     |
//-------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_USERS 100       // Número máximo de usuários
#define MAX_NAME_LENGTH 50  // Tamanho máximo do nome

// Estrutura para um nó da lista de adjacência (amigos)
typedef struct Node {
    int user_id;            // ID do amigo
    struct Node* next;      // Próximo amigo na lista
} Node;

// Estrutura para um usuário
typedef struct {
    char name[MAX_NAME_LENGTH]; // Nome do usuário
    Node* friends;          // Lista de amigos (lista de adjacência)
} User;

// Estrutura principal da rede social
typedef struct {
    User users[MAX_USERS];  // Array de usuários
    int num_users;          // Contador de usuários
} SocialNetwork;

// Inicializa a rede social
void initNetwork(SocialNetwork* sn) {
    sn->num_users = 0;
    for (int i = 0; i < MAX_USERS; i++) {
        sn->users[i].friends = NULL;
    }
}

// Adiciona um novo usuário
int addUser(SocialNetwork* sn, const char* name) {
    if (sn->num_users >= MAX_USERS) return -1; // Verifica limite
    
    strncpy(sn->users[sn->num_users].name, name, MAX_NAME_LENGTH);
    sn->users[sn->num_users].friends = NULL;
    return sn->num_users++; // Retorna o ID do novo usuário
}

// Adiciona amizade entre dois usuários
void addFriend(SocialNetwork* sn, int id1, int id2) {
    // Verifica IDs válidos
    if (id1 == id2 || id1 < 0 || id2 < 0 || 
        id1 >= sn->num_users || id2 >= sn->num_users) return;

    // Verifica se já são amigos
    for (Node* n = sn->users[id1].friends; n != NULL; n = n->next) {
        if (n->user_id == id2) return;
    }

    // Adiciona amizade (grafo não direcionado - ambas direções)
    Node* new1 = (Node*)malloc(sizeof(Node));
    new1->user_id = id2;
    new1->next = sn->users[id1].friends;
    sn->users[id1].friends = new1;

    Node* new2 = (Node*)malloc(sizeof(Node));
    new2->user_id = id1;
    new2->next = sn->users[id2].friends;
    sn->users[id2].friends = new2;
}

// Mostra os amigos de um usuário
void showFriends(SocialNetwork* sn, int id) {
    if (id < 0 || id >= sn->num_users) return;
    
    printf("\nAmigos de %s:\n", sn->users[id].name);
    for (Node* n = sn->users[id].friends; n != NULL; n = n->next) {
        printf("- %s\n", sn->users[n->user_id].name);
    }
}

// Busca em Largura (BFS) - mostra amigos por nível
void BFS(SocialNetwork* sn, int start_id) {
    if (start_id < 0 || start_id >= sn->num_users) return;

    bool visited[MAX_USERS] = {false};
    int distance[MAX_USERS] = {0};
    int queue[MAX_USERS];
    int front = 0, rear = 0;

    queue[rear++] = start_id;
    visited[start_id] = true;

    printf("\nRede de contatos de %s:\n", sn->users[start_id].name);

    while (front < rear) {
        int current = queue[front++];
        
        // Mostra amigos por nível de distância
        if (distance[current] > 0) {
            printf("Nível %d: %s\n", distance[current], sn->users[current].name);
        }

        // Visita todos os amigos
        for (Node* n = sn->users[current].friends; n != NULL; n = n->next) {
            if (!visited[n->user_id]) {
                visited[n->user_id] = true;
                distance[n->user_id] = distance[current] + 1;
                queue[rear++] = n->user_id;
            }
        }
    }
}

// Função auxiliar para DFS
void DFS_visit(SocialNetwork* sn, int id, bool visited[]) {
    visited[id] = true;
    printf("- %s\n", sn->users[id].name);

    for (Node* n = sn->users[id].friends; n != NULL; n = n->next) {
        if (!visited[n->user_id]) {
            DFS_visit(sn, n->user_id, visited);
        }
    }
}

// Busca em Profundidade (DFS)
void DFS(SocialNetwork* sn, int start_id) {
    if (start_id < 0 || start_id >= sn->num_users) return;

    bool visited[MAX_USERS] = {false};
    printf("\nDFS a partir de %s:\n", sn->users[start_id].name);
    DFS_visit(sn, start_id, visited);
}

// Verifica conexão entre dois usuários
bool areConnected(SocialNetwork* sn, int id1, int id2) {
    if (id1 == id2) return true;
    
    bool visited[MAX_USERS] = {false};
    int stack[MAX_USERS];
    int top = 0;

    stack[top++] = id1;
    visited[id1] = true;

    while (top > 0) {
        int current = stack[--top];

        for (Node* n = sn->users[current].friends; n != NULL; n = n->next) {
            if (n->user_id == id2) return true;
            if (!visited[n->user_id]) {
                visited[n->user_id] = true;
                stack[top++] = n->user_id;
            }
        }
    }
    return false;
}

// Sugere novos amigos (amigos de amigos não conectados)
void suggestFriends(SocialNetwork* sn, int id) {
    if (id < 0 || id >= sn->num_users) return;

    bool friends_of_friends[MAX_USERS] = {false};
    bool direct_friends[MAX_USERS] = {false};

    // Marca amigos diretos
    for (Node* n = sn->users[id].friends; n != NULL; n = n->next) {
        direct_friends[n->user_id] = true;
    }

    printf("\nSugestões para %s:\n", sn->users[id].name);
    bool has_suggestions = false;

    // Procura amigos de amigos
    for (Node* n = sn->users[id].friends; n != NULL; n = n->next) {
        for (Node* f = sn->users[n->user_id].friends; f != NULL; f = f->next) {
            if (!direct_friends[f->user_id] && f->user_id != id && !friends_of_friends[f->user_id]) {
                printf("- %s\n", sn->users[f->user_id].name);
                friends_of_friends[f->user_id] = true;
                has_suggestions = true;
            }
        }
    }

    if (!has_suggestions) {
        printf("Nenhuma sugestão encontrada.\n");
    }
}

// Libera memória alocada
void freeNetwork(SocialNetwork* sn) {
    for (int i = 0; i < sn->num_users; i++) {
        Node* current = sn->users[i].friends;
        while (current != NULL) {
            Node* temp = current;
            current = current->next;
            free(temp);
        }
    }
}

// Menu principal
void showMenu() {
    printf("\n=== REDE SOCIAL ===\n");
    printf("1. Adicionar usuario\n");
    printf("2. Adicionar amizade\n");
    printf("3. Ver amigos\n");
    printf("4. Busca em largura (BFS)\n");
    printf("5. Busca em profundidade (DFS)\n");
    printf("6. Verificar conexao\n");
    printf("7. Sugerir amigos\n");
    printf("8. Sair\n");
    printf("Escolha: ");
}

int main() {
    SocialNetwork sn;
    initNetwork(&sn);

    int choice;
    do {
        showMenu();
        scanf("%d", &choice);
        getchar(); // Limpa buffer
        
        switch (choice) {
            case 1: {
                char name[MAX_NAME_LENGTH];
                printf("Nome: ");
                fgets(name, MAX_NAME_LENGTH, stdin);
                name[strcspn(name, "\n")] = '\0'; // Remove \n
                addUser(&sn, name);
                break;
            }
            case 2: {
                int id1, id2;
                printf("ID1: ");
                scanf("%d", &id1);
                printf("ID2: ");
                scanf("%d", &id2);
                addFriend(&sn, id1, id2);
                break;
            }
            case 3: {
                int id;
                printf("ID do usuario: ");
                scanf("%d", &id);
                showFriends(&sn, id);
                break;
            }
            case 4: {
                int id;
                printf("ID inicial: ");
                scanf("%d", &id);
                BFS(&sn, id);
                break;
            }
            case 5: {
                int id;
                printf("ID inicial: ");
                scanf("%d", &id);
                DFS(&sn, id);
                break;
            }
            case 6: {
                int id1, id2;
                printf("ID1: ");
                scanf("%d", &id1);
                printf("ID2: ");
                scanf("%d", &id2);
                if (areConnected(&sn, id1, id2)) {
                    printf("\nConectados!\n");
                } else {
                    printf("\nNao conectados!\n");
                }
                break;
            }
            case 7: {
                int id;
                printf("ID do usuario: ");
                scanf("%d", &id);
                suggestFriends(&sn, id);
                break;
            }
            case 8:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
        printf("\nPressione Enter para continuar...");
        getchar();
    } while (choice != 8);

    freeNetwork(&sn);
    return 0;
}