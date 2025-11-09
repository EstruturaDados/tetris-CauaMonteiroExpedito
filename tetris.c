#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3
#define MAX_PECAS 1000

// Definição da estrutura Peça
typedef struct {
    char nome;
    int id;
} Peca;

// Definição da estrutura Fila (circular)
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente;
    int tras;
    int tamanho;
} Fila;

// Definição da estrutura Pilha
typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo;
    int tamanho;
} Pilha;

// Protótipos das funções
void inicializarFila(Fila *fila);
void inicializarPilha(Pilha *pilha);
Peca gerarPeca(int *contador_id);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);
int enqueue(Fila *fila, Peca peca);
Peca dequeue(Fila *fila);
int push(Pilha *pilha, Peca peca);
Peca pop(Pilha *pilha);
void exibirEstado(Fila *fila, Pilha *pilha);
void mostrarMenu();

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contador_id = 0;  // Contador global para IDs únicos
    
    // Inicializar random seed
    srand(time(NULL));
    
    // Inicializar estruturas
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    printf("=== Tetris Stack - Gerenciador de Pecas ===\n\n");
    
    // Preencher a fila inicial com 5 peças
    while (!filaCheia(&fila)) {
        enqueue(&fila, gerarPeca(&contador_id));
    }
    
    do {
        exibirEstado(&fila, &pilha);
        mostrarMenu();
        
        printf("\nDigite sua opcao: ");
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 0:
                printf("Saindo do jogo...\n");
                break;
                
            case 1: // Jogar peça (remover da frente da fila)
                if (!filaVazia(&fila)) {
                    Peca pecaRemovida = dequeue(&fila);
                    printf("Peca jogada: [%c %d]\n", pecaRemovida.nome, pecaRemovida.id);
                    
                    // Adicionar nova peça automaticamente para manter fila cheia
                    if (!filaCheia(&fila)) {
                        enqueue(&fila, gerarPeca(&contador_id));
                        printf("Nova peca adicionada a fila!\n");
                    }
                } else {
                    printf("Erro: Fila vazia! Nao ha pecas para jogar.\n");
                }
                break;
                
            case 2: // Reservar peça (mover da fila para a pilha)
                if (!filaVazia(&fila)) {
                    if (!pilhaCheia(&pilha)) {
                        Peca pecaReservada = dequeue(&fila);
                        push(&pilha, pecaReservada);
                        printf("Peca reservada: [%c %d]\n", pecaReservada.nome, pecaReservada.id);
                        
                        // Adicionar nova peça automaticamente para manter fila cheia
                        if (!filaCheia(&fila)) {
                            enqueue(&fila, gerarPeca(&contador_id));
                            printf("Nova peca adicionada a fila!\n");
                        }
                    } else {
                        printf("Erro: Pilha de reserva cheia! Nao e possivel reservar mais pecas.\n");
                    }
                } else {
                    printf("Erro: Fila vazia! Nao ha pecas para reservar.\n");
                }
                break;
                
            case 3: // Usar peça reservada (remover do topo da pilha)
                if (!pilhaVazia(&pilha)) {
                    Peca pecaUsada = pop(&pilha);
                    printf("Peca reservada usada: [%c %d]\n", pecaUsada.nome, pecaUsada.id);
                } else {
                    printf("Erro: Pilha de reserva vazia! Nao ha pecas reservadas para usar.\n");
                }
                break;
                
            default:
                printf("Opcao invalida! Tente novamente.\n");
                break;
        }
        
        // Pausa mais simples - removendo o problema do buffer
        if (opcao != 0) {
            printf("\nPressione Enter para continuar...");
            while (getchar() != '\n'); // Limpar buffer
            getchar(); // Esperar Enter
        }
        
    } while (opcao != 0);
    
    return 0;
}

// Inicializa a fila com valores padrão
void inicializarFila(Fila *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Inicializa a pilha com valores padrão
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
    pilha->tamanho = 0;
}

// Gera uma peça aleatória com ID único
Peca gerarPeca(int *contador_id) {
    Peca novaPeca;
    
    // Tipos de peças disponíveis
    char tipos[] = {'I', 'O', 'T', 'L'};
    int numTipos = sizeof(tipos) / sizeof(tipos[0]);
    
    // Escolher tipo aleatório
    novaPeca.nome = tipos[rand() % numTipos];
    
    // Atribuir ID único e incrementar contador
    novaPeca.id = (*contador_id)++;
    
    return novaPeca;
}

// Verifica se a fila está vazia
int filaVazia(Fila *fila) {
    return fila->tamanho == 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *fila) {
    return fila->tamanho == TAMANHO_FILA;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *pilha) {
    return pilha->tamanho == 0;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *pilha) {
    return pilha->tamanho == TAMANHO_PILHA;
}

// Adiciona uma peça ao final da fila
int enqueue(Fila *fila, Peca peca) {
    if (filaCheia(fila)) {
        return 0; // Falha
    }
    
    // Usando fila circular
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->pecas[fila->tras] = peca;
    fila->tamanho++;
    
    return 1; // Sucesso
}

// Remove e retorna a peça da frente da fila
Peca dequeue(Fila *fila) {
    Peca pecaVazia = {' ', -1}; // Peça vazia para retorno em caso de erro
    
    if (filaVazia(fila)) {
        return pecaVazia;
    }
    
    Peca pecaRemovida = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->tamanho--;
    
    return pecaRemovida;
}

// Adiciona uma peça ao topo da pilha
int push(Pilha *pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        return 0; // Falha
    }
    
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    pilha->tamanho++;
    
    return 1; // Sucesso
}

// Remove e retorna a peça do topo da pilha
Peca pop(Pilha *pilha) {
    Peca pecaVazia = {' ', -1}; // Peça vazia para retorno em caso de erro
    
    if (pilhaVazia(pilha)) {
        return pecaVazia;
    }
    
    Peca pecaRemovida = pilha->pecas[pilha->topo];
    pilha->topo--;
    pilha->tamanho--;
    
    return pecaRemovida;
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("=== ESTADO ATUAL ===\n\n");
    
    // Exibir fila de peças
    printf("Fila de pecas:\t");
    if (filaVazia(fila)) {
        printf("Vazia");
    } else {
        int indice = fila->frente;
        for (int i = 0; i < fila->tamanho; i++) {
            printf("[%c %d] ", fila->pecas[indice].nome, fila->pecas[indice].id);
            indice = (indice + 1) % TAMANHO_FILA;
        }
    }
    printf("\n");
    
    // Exibir pilha de reserva (mostrando do topo para base)
    printf("Pilha de reserva (Topo -> Base):\t");
    if (pilhaVazia(pilha)) {
        printf("Vazia");
    } else {
        // Mostrar do topo para a base (mais intuitivo para pilha)
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->pecas[i].nome, pilha->pecas[i].id);
        }
    }
    printf("\n");
}

// Mostra o menu de opções
void mostrarMenu() {
    printf("\n=== OPCOES DE ACAO ===\n");
    printf("Codigo\tAcao\n");
    printf("1\tJogar peca (remover da fila)\n");
    printf("2\tReservar peca (fila -> pilha)\n");
    printf("3\tUsar peca reservada (remover da pilha)\n");
    printf("0\tSair\n");
}