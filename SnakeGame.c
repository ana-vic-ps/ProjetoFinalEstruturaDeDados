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

int main(){

}