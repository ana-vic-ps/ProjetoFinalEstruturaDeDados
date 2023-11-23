#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

#define LARGURA 40 // Largura da área de jogo
#define ALTURA 20 // Altura da área de jogo

unsigned int pontuacao = 0;
unsigned int maiorPontuacao = 0;
// Estrutura para um "nó" da cobrinha
struct ParteCobra {
    int x, y; // Coordenadas
    struct ParteCobra *proximo; // Ponteiro para o próximo "nó"
};

typedef struct ParteCobra ParteCobra;

// Estrutura para a cobrinha
struct Cobrinha {
    ParteCobra *cabeca; // Cabeça da cobrinha
    int comprimento; // Comprimento da cobrinha
    int direcao; // Direção da cobrinha (1 - cima, 2 - baixo, 3 - esquerda, 4 - direita)
};

typedef struct Cobrinha Cobrinha;

// Estrutura para a comida
struct Comida {
    int x, y; // Coordenadas da comida
};

typedef struct Comida Comida;

ParteCobra *criarNo(int x, int y) {
    ParteCobra *no = (ParteCobra *)malloc(sizeof(ParteCobra));
    no->x = x;
    no->y = y;
    no->proximo = NULL;
    return no;
}
// Estrutura para adicionar um "nó" na cobrinha.
void adicionarNo(Cobrinha *cobrinha, int x, int y) {
    ParteCobra *novoNo = criarNo(x, y);
    novoNo->proximo = cobrinha->cabeca;
    cobrinha->cabeca = novoNo;
    cobrinha->comprimento++;
}

void novaComida(Comida *comida) {
    // Verificar se a comida gerada está na posicao da cobra ou nas bordas
    comida->x = rand() % LARGURA;
    comida->y = rand() % ALTURA;
}

void inicializar(Cobrinha *cobrinha, Comida *comida) {
    cobrinha->cabeca = criarNo(LARGURA / 2, ALTURA / 2);
    cobrinha->comprimento = 1;
    cobrinha->direcao = 4; // Começa indo para a direita

    // Posiciona a comida em algum lugar aleatório
    srand(time(NULL));
    novaComida(comida);
}

void removerNosExcetoCabeca(Cobrinha *cobrinha) {
    if (cobrinha == NULL || cobrinha->cabeca == NULL) {
        return; // Verificação para evitar operações inválidas em casos nulos ou listas vazias
    }

    ParteCobra *atual = cobrinha->cabeca->proximo;
    while (atual != NULL) {
        ParteCobra *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }

    // Deixe a cabeça como o único nó na lista
    cobrinha->cabeca->proximo = NULL;
    cobrinha->comprimento = 1;
}

void resetaPosicao(Cobrinha *cobrinha) {
    if (cobrinha == NULL) {
        return;
    }
    cobrinha->cabeca->x = LARGURA / 2;
    cobrinha->cabeca->y = ALTURA / 2;
    cobrinha->direcao = (rand() % 4) + 1;
}

void mostrarJogo(Cobrinha *cobrinha, Comida *comida) {
    system("cls"); // Limpa a tela

    // Desenha a área de jogo
    for (int i = 0; i < LARGURA + 2; i++)
        printf("-");
    printf("\n");

    for (int i = 0; i < ALTURA; i++) {
        for (int j = 0; j < LARGURA; j++) {
            if (j == 0)
                printf("|"); // Parede esquerda

            bool ehParteCorpo = false;
            ParteCobra *atual = cobrinha->cabeca;
            while (atual != NULL) {
                if (atual->x == j && atual->y == i) {
                    if (atual == cobrinha->cabeca) {
                        printf("@");
                    } else {
                        printf("o");
                    }
                    // Corpo da cobrinha
                    ehParteCorpo = true;
                }
                atual = atual->proximo;
            }

            if (j == LARGURA - 1 && !ehParteCorpo)
                printf("|"); // Parede direita
            else if (!ehParteCorpo && i == comida->y && j == comida->x)
                printf("F"); // Comida
            else if (!ehParteCorpo)
                printf(" ");
        }
        printf("\n");
    }

    for (int i = 0; i < LARGURA + 2; i++)
        printf("#");
    printf("\n");
}

int main(){

}
