#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da struct Territorio
typedef struct {
    char nome[30];
    char cor[10]; // Ex: "vermelho", "azul"
    int tropas;
} Territorio;

// Função para cadastrar os territórios
void cadastrarTerritorios(Territorio* mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("\nCadastro do território %d:\n", i + 1);
        printf("Nome do território: ");
        scanf(" %[^\n]", mapa[i].nome);

        printf("Cor do exército (ex: vermelho, azul): ");
        scanf(" %[^\n]", mapa[i].cor);

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// Função para exibir os territórios
void exibirTerritorios(Territorio* mapa, int tamanho) {
    printf("\n===== ESTADO ATUAL DOS TERRITÓRIOS =====\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d. %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Função de ataque entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n⚠️ Você não pode atacar um território da mesma cor!\n");
        return;
    }

    if (atacante->tropas <= 1) {
        printf("\n⚠️ O território atacante precisa ter mais de 1 tropa para atacar.\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n🎲 Dado do atacante: %d", dadoAtacante);
    printf("\n🎲 Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\n✅ Ataque bem-sucedido! O território %s foi conquistado.\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas -= atacante->tropas / 2;
    } else {
        printf("\n❌ Ataque mal-sucedido! O território %s resistiu.\n", defensor->nome);
        atacante->tropas -= 1;
    }
}

// Atribui missão aleatória para o jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Exibe missão no início do jogo
void exibirMissao(char* missao) {
    printf("\n🎯 Sua missão estratégica: %s\n", missao);
}

// Verifica se a missão foi cumprida (lógica simples)
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if (strstr(missao, "Conquistar 3 territórios seguidos")) {
        int consecutivos = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0) {
                consecutivos++;
                if (consecutivos >= 3) return 1;
            } else {
                consecutivos = 0;
            }
        }
    } else if (strstr(missao, "Eliminar todas as tropas da cor vermelha")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0 && mapa[i].tropas > 0)
                return 0;
        }
        return 1;
    }
    // Outras missões podem ser adicionadas com novas condições
    return 0;
}

// Libera a memória alocada
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    free(mapa);
    free(missaoJogador);
}

int main() {
    srand(time(NULL));

    int numTerritorios;
    printf("🌍 Quantos territórios deseja cadastrar? ");
    scanf("%d", &numTerritorios);

    Territorio* mapa = (Territorio*) calloc(numTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, numTerritorios);

    // Vetor de missões
    char* missoes[] = {
        "Conquistar 3 territórios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar ao menos 4 territórios",
        "Ter mais de 20 tropas no total",
        "Manter pelo menos 2 territórios com 5 tropas cada"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Missão alocada dinamicamente
    char* missaoJogador = (char*) malloc(100 * sizeof(char));
    if (missaoJogador == NULL) {
        printf("Erro ao alocar memória para missão.\n");
        free(mapa);
        return 1;
    }

    atribuirMissao(missaoJogador, missoes, totalMissoes);
    exibirMissao(missaoJogador);

    int opcao;
    do {
        exibirTerritorios(mapa, numTerritorios);

        printf("\nDeseja simular um ataque? (1-Sim | 0-Não): ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            int idAtacante, idDefensor;

            printf("\nEscolha o território atacante (1 a %d): ", numTerritorios);
            scanf("%d", &idAtacante);
            printf("Escolha o território defensor (1 a %d): ", numTerritorios);
            scanf("%d", &idDefensor);

            if (idAtacante < 1 || idAtacante > numTerritorios ||
                idDefensor < 1 || idDefensor > numTerritorios ||
                idAtacante == idDefensor) {
                printf("\n⚠️ Escolhas inválidas. Tente novamente.\n");
                continue;
            }

            atacar(&mapa[idAtacante - 1], &mapa[idDefensor - 1]);

            if (verificarMissao(missaoJogador, mapa, numTerritorios)) {
                printf("\n🎉 MISSÃO CUMPRIDA! VOCÊ VENCEU O JOGO!\n");
                break;
            }
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missaoJogador);
    printf("\n✅ Memória liberada e programa finalizado.\n");

    return 0;
}
