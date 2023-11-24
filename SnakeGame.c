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

void recomecaJogoAtual(Cobrinha *cobrinha, Comida *comida)
{
    if (pontuacao > maiorPontuacao) 
    {
        maiorPontuacao = pontuacao;
    }
    pontuacao = 0;
    removerNosExcetoCabeca(cobrinha);
    resetaPosicao(cobrinha);
    novaComida(comida);
}

void capturarEntrada(Cobrinha *cobrinha, Comida *comida) 
{
    if (_kbhit()) 
    {
        switch (_getch()) 
        {
            case 'w':
                if (cobrinha->direcao != 2)
                    cobrinha->direcao = 1;
                break;
            case 's':
                if (cobrinha->direcao != 1)
                    cobrinha->direcao = 2;
                break;
            case 'a':
                if (cobrinha->direcao != 4)
                    cobrinha->direcao = 3;
                break;
            case 'd':
                if (cobrinha->direcao != 3)
                    cobrinha->direcao = 4;
                break;
            case 'r':
                recomecaJogoAtual(cobrinha, comida);
                break;
        }
    }
}

void mover(Cobrinha *cobrinha) 
{
    int prevX = cobrinha->cabeca->x;
    int prevY = cobrinha->cabeca->y;
    ParteCobra *atual = cobrinha->cabeca->proximo;

    int tempX, tempY;
    while (atual != NULL) 
    {
        tempX = atual->x;
        tempY = atual->y;
        atual->x = prevX;
        atual->y = prevY;
        prevX = tempX;
        prevY = tempY;
        atual = atual->proximo;
    }

    switch (cobrinha->direcao) 
    {
        case 1:
            cobrinha->cabeca->y--;
            break;
        case 2:
            cobrinha->cabeca->y++;
            break;
        case 3:
            cobrinha->cabeca->x--;
            break;
        case 4:
            cobrinha->cabeca->x++;
            break;
    }
}

bool verificarColisao(Cobrinha *cobrinha) 
{
    if (cobrinha->cabeca->x < 0 || cobrinha->cabeca->x >= LARGURA || cobrinha->cabeca->y < 0 || cobrinha->cabeca->y >= ALTURA)
        return true;

    ParteCobra *atual = cobrinha->cabeca->proximo;
    while (atual != NULL) 
    {
        if (atual->x == cobrinha->cabeca->x && atual->y == cobrinha->cabeca->y)
            return true;
        atual = atual->proximo;
    }

    return false;
}

bool verificarColisaoComida(Cobrinha *cobrinha, Comida *comida) {
    if (cobrinha->cabeca->x == comida->x && cobrinha->cabeca->y == comida->y) {
        pontuacao += 1;
        return true;
    }
    return false;
}

bool atualizar(Cobrinha *cobrinha, Comida *comida) {
    if (verificarColisaoComida(cobrinha, comida)) {
        // A cobrinha comeu a comida
        adicionarNo(cobrinha, comida->x, comida->y);
        cobrinha->comprimento++;
        novaComida(comida);
    }

    mover(cobrinha);

    if (verificarColisao(cobrinha)) {
        printf("\n\tGAME OVER!\n");
        if (pontuacao > maiorPontuacao) {
            // TODO: gravar pontuacao em um arquivo (?)
            maiorPontuacao = pontuacao;
            printf("\tNOVO RECORDE!\n");
        }
        printf("\tSua pontuacao foi: %d\n\n", pontuacao);
        return true;
    }

    return false;
}

void comecarJogo() {
    Cobrinha cobrinha;
    Comida comida;
    inicializar(&cobrinha, &comida);
    pontuacao = 0;
    while (1) {
        mostrarJogo(&cobrinha, &comida);
        capturarEntrada(&cobrinha, &comida);
        if(atualizar(&cobrinha, &comida)) {
            return;
        }
        Sleep(42); // Delay para controle de velocidade da serpente
    }
}

void mostraInformacoes() {
    printf("\n\nO jogo da cobrinha, ou Snake, e um classico onde os jogadores controlam uma\n"
       "serpente que cresce ao coletar alimentos, desviando-se das bordas do campo e do\n"
       "proprio corpo. Os controles, usando as teclas WASD, direcionam a serpente para\n"
       "frente, esquerda, baixo ou direita. A medida que a cobra aumenta de tamanho, a\n"
       "dificuldade aumenta, exigindo raciocinio rapido para evitar colisoes. O desafio\n"
       "constante entre coletar alimentos e evitar obstaculos torna o Snake um jogo\n"
       "simples e viciante.\n\n");
}


int main(){

    return 0;
}
