#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define QTD_TERRITORIOS 3

typedef struct {
    char nome[50];
    char corExercito[30];
    int tropas;
} Territorio;

// Remover \n do fgets
void removeQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

// Cadastrar territórios
void cadastrarTerritorios(Territorio *mapa) {
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        removeQuebraLinha(mapa[i].nome);

        printf("Cor do Exército: ");
        fgets(mapa[i].corExercito, sizeof(mapa[i].corExercito), stdin);
        removeQuebraLinha(mapa[i].corExercito);

        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar(); // limpa buffer
    }
}

// Mostra mapa atual
void exibirMapa(Territorio *mapa) {
    printf("\n=== ESTADO ATUAL DO MAPA ===\n");
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor: %s\n", mapa[i].corExercito);
        printf("Tropas: %d\n", mapa[i].tropas);
    }
}

// Simula ataque entre dois territórios
void simularBatalha(Territorio *atacante, Territorio *defensor) {
    int dadoAtk = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("\n>>> BATALHA <<<\n");
    printf("%s (A) rolou: %d\n", atacante->nome, dadoAtk);
    printf("%s (D) rolou: %d\n", defensor->nome, dadoDef);

    // Empates favorecem o atacante
    if (dadoAtk >= dadoDef) {
        printf("Atacante venceu! %s perde 1 tropa.\n", defensor->nome);
        defensor->tropas--;

        if (defensor->tropas <= 0) {
            printf("\n*** TERRITÓRIO CONQUISTADO! ***\n");
            defensor->tropas = 1; // atacante sempre entra com 1 tropa
            strcpy(defensor->corExercito, atacante->corExercito);
            printf("%s agora pertence ao exército %s.\n",
                   defensor->nome, defensor->corExercito);
        }
    } else {
        printf("Defensor venceu! Nenhuma tropa perdida pelo atacante.\n");
    }
}

int main() {
    srand(time(NULL));

    // Alocação dinâmica com calloc
    Territorio *mapa = (Territorio *) calloc(QTD_TERRITORIOS, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    printf("=== WAR Estruturado – Nível Aventureiro ===\n");

    cadastrarTerritorios(mapa);
    exibirMapa(mapa);

    int atk, def;

    while (1) {
        printf("\nDigite o território atacante (1-5) ou 0 para sair: ");
        scanf("%d", &atk);
        if (atk == 0) break;

        printf("Digite o território defensor (1-5): ");
        scanf("%d", &def);
        getchar();

        if (atk < 1 || atk > 5 || def < 1 || def > 5 || atk == def) {
            printf("Escolha inválida!\n");
            continue;
        }

        if (mapa[atk - 1].tropas <= 1) {
            printf("O atacante precisa ter mais de 1 tropa para atacar!\n");
            continue;
        }

        simularBatalha(&mapa[atk - 1], &mapa[def - 1]);
        exibirMapa(mapa);
    }

    free(mapa);

    printf("\nJogo encerrado. Obrigado por jogar!\n");
    return 0;
}
