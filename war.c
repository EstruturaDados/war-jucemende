#include <stdio.h>
#include <string.h>

#define QT_TERRITORIOS 5

typedef struct {
    char nome[50];
    char corExercito[30];
    int tropas;
} Territorio;

// Remover \n do fgets
void removerQuebraLinha(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main() {
    Territorio mapa[QT_TERRITORIOS];

    printf("=== Cadastro de Territorios ===\n\n");

    for (int i = 0; i < QT_TERRITORIOS; i++) {
        printf("Territorio %d:\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        removerQuebraLinha(mapa[i].nome);

        printf("Cor do Exército: ");
        fgets(mapa[i].corExercito, sizeof(mapa[i].corExercito), stdin);
        removerQuebraLinha(mapa[i].corExercito);

        printf("Quantidade de Tropas: ");
        scanf("%d", &mapa[i].tropas);

        getchar(); // limpa buffer após o scanf

        printf("\n");
    }

    // ------------------------
    // Exibir o mapa completo
    // ------------------------
    printf("\n=== Estado Atual do Mapa ===\n");

    for (int i = 0; i < QT_TERRITORIOS; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", mapa[i].nome);
        printf("Cor do Exército: %s\n", mapa[i].corExercito);
        printf("Tropas: %d\n", mapa[i].tropas);
    }

    return 0;
}
