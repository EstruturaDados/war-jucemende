#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QTD_TERRITORIOS 5

// Struct que representa um território no mapa
typedef struct {
    char nome[50];
    char cor[30];
    int tropas;
} Territorio;

// Tipos de missão que o jogador pode receber
typedef enum {
    MISSAO_DESTRUIR_VERDE,
    MISSAO_CONQUISTAR_3
} Missao;

// ---------------------------------------------------------------------------
// Função para remover o '\n' que vem no final quando usamos fgets
// ---------------------------------------------------------------------------
void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

// ---------------------------------------------------------------------------
// Função que sorteia um número entre 1 e 6 (simula um dado)
// ---------------------------------------------------------------------------
int dado() {
    return (rand() % 6) + 1;
}

// ---------------------------------------------------------------------------
// Função para criar os territórios automaticamente
// ---------------------------------------------------------------------------
void inicializarMapa(Territorio *mapa) {
    // Listas com nomes e cores prontas
    const char *nomes[QTD_TERRITORIOS] = {
        "Alaska", "Alberta", "Brasil", "Egito", "Japão"
    };

    const char *cores[QTD_TERRITORIOS] = {
        "Azul", "Vermelho", "Verde", "Preto", "Amarelo"
    };

    // Preenche o mapa
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);    // Copia nome
        strcpy(mapa[i].cor, cores[i]);     // Copia cor
        mapa[i].tropas = (rand() % 5) + 3; // Tropas entre 3 e 7
    }
}

// ---------------------------------------------------------------------------
// Mostra o mapa inteiro na tela
// ---------------------------------------------------------------------------
void exibirMapa(const Territorio *mapa) {
    printf("\n=== ESTADO ATUAL DO MAPA ===\n");

    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Exército: %s\n", mapa[i].cor);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

// ---------------------------------------------------------------------------
// Sorteia a missão do jogador (50% de chance para cada)
// ---------------------------------------------------------------------------
Missao sortearMissao() {
    return rand() % 2 == 0 ? MISSAO_DESTRUIR_VERDE : MISSAO_CONQUISTAR_3;
}

// ---------------------------------------------------------------------------
// Mostra o texto da missão na tela
// ---------------------------------------------------------------------------
void exibirMissao(const Missao missao) {
    printf("\n=== SUA MISSÃO ===\n");

    if (missao == MISSAO_DESTRUIR_VERDE)
        printf("Destruir o exército VERDE.\n");
    else
        printf("Conquistar 3 territórios.\n");
}

// ---------------------------------------------------------------------------
// Conta quantos territórios pertencem ao jogador (mesma cor)
// ---------------------------------------------------------------------------
int contarConquistadosPelaCor(const Territorio *mapa, const char *corJogador) {
    int total = 0;

    for (int i = 0; i < QTD_TERRITORIOS; i++)
        if (strcmp(mapa[i].cor, corJogador) == 0)
            total++;

    return total;
}

// ---------------------------------------------------------------------------
// Verifica se ainda existe algum território verde
// ---------------------------------------------------------------------------
int exercitoVerdeExiste(const Territorio *mapa) {
    for (int i = 0; i < QTD_TERRITORIOS; i++)
        if (strcmp(mapa[i].cor, "Verde") == 0)
            return 1; // Existe

    return 0; // Não existe mais
}

// ---------------------------------------------------------------------------
// Verifica se a missão já foi cumprida
// ---------------------------------------------------------------------------
int verificarMissao(const Territorio *mapa, Missao m, const char *corJogador) {
    // Missão 1: destruir exército verde
    if (m == MISSAO_DESTRUIR_VERDE)
        return !exercitoVerdeExiste(mapa);

    // Missão 2: conquistar 3 territórios
    return contarConquistadosPelaCor(mapa, corJogador) >= 3;
}

// ---------------------------------------------------------------------------
// Simula uma batalha entre atacante e defensor
// ---------------------------------------------------------------------------
void simularBatalha(Territorio *atacante, Territorio *defensor) {
    printf("\n>>> BATALHA ENTRE %s (A) E %s (D) <<<\n",
           atacante->nome, defensor->nome);

    int atkDado = dado();
    int defDado = dado();

    printf("Ataque rolou: %d\n", atkDado);
    printf("Defesa rolou: %d\n", defDado);

    // Atacante ganha empates
    if (atkDado >= defDado) {
        printf("Atacante venceu! %s perde 1 tropa.\n", defensor->nome);
        defensor->tropas--;

        // Verifica se o defensor perdeu todas as tropas
        if (defensor->tropas <= 0) {
            printf("\n*** TERRITÓRIO CONQUISTADO! ***\n");

            // Muda o exército dono do território
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1; // entra com 1 tropa
        }
    } else {
        printf("Defensor venceu! Nada acontece com o atacante.\n");
    }
}

// ---------------------------------------------------------------------------
// Função principal de ataque, onde o jogador escolhe quem ataca e defende
// ---------------------------------------------------------------------------
void realizarAtaque(Territorio *mapa, const char *corJogador) {
    int atk, def;

    printf("\nEscolha o território ATACANTE (1-5): ");
    scanf("%d", &atk);

    printf("Escolha o território DEFENSOR (1-5): ");
    scanf("%d", &def);

    getchar(); // limpa buffer

    // Verifica se a entrada é válida
    if (atk < 1 || atk > 5 || def < 1 || def > 5 || atk == def) {
        printf("Entrada inválida!\n");
        return;
    }

    Territorio *a = &mapa[atk - 1];
    Territorio *d = &mapa[def - 1];

    // Jogador só pode atacar com seu próprio exército
    if (strcmp(a->cor, corJogador) != 0) {
        printf("Você só pode atacar usando seus territórios!\n");
        return;
    }

    // Ataque só pode acontecer se houver mais de 1 tropa
    if (a->tropas <= 1) {
        printf("O território atacante precisa ter mais de 1 tropa!\n");
        return;
    }

    // Chama a batalha
    simularBatalha(a, d);
}

// ---------------------------------------------------------------------------
// Menu principal do jogo
// ---------------------------------------------------------------------------
void menu(Territorio *mapa, Missao missao, const char *corJogador) {
    int opcao;

    while (1) {
        printf("\n=== MENU PRINCIPAL ===\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missão\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                realizarAtaque(mapa, corJogador);
                exibirMapa(mapa);
                break;

            case 2:
                exibirMissao(missao);

                if (verificarMissao(mapa, missao, corJogador)) {
                    printf("\n🎉 PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO! 🎉\n");
                    return;
                } else {
                    printf("\nMissão ainda não cumprida.\n");
                }
                break;

            case 0:
                printf("Saindo do jogo...\n");
                return;

            default:
                printf("Opção inválida!\n");
        }
    }
}

// ---------------------------------------------------------------------------
// Função principal
// ---------------------------------------------------------------------------
int main() {
    srand(time(NULL)); // Gera números aleatórios diferentes a cada execução

    // Aloca o mapa na memória
    Territorio *mapa = calloc(QTD_TERRITORIOS, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    // Cor fixa do jogador (pode ser expandido)
    const char *corJogador = "Azul";

    printf("\n=== WAR Estruturado – Nível Mestre ===\n");
    printf("Você é o exército: %s\n\n", corJogador);

    // Inicializa o mapa
    inicializarMapa(mapa);

    // Sorteia missão
    Missao missao = sortearMissao();
    exibirMissao(missao);

    // Mostra o mapa
    exibirMapa(mapa);

    // Inicia o menu do jogo
    menu(mapa, missao, corJogador);

    // Libera memória
    free(mapa);

    return 0;
}
