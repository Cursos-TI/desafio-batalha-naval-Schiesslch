#include <stdio.h>
#include <string.h>

#define N5 5
#define N10 10

/* ----------------------------------------------------------
   Funções utilitárias
   ----------------------------------------------------------*/

/* Imprime coordenadas de um vetor de pares (x,y). */
void imprimir_coordenadas(const char *nome, int coords[][2], int partes) {
    printf("%s (%d partes):\n", nome, partes);
    for (int i = 0; i < partes; ++i) {
        printf("  Parte %d -> (x=%d, y=%d)\n", i+1, coords[i][0], coords[i][1]);
    }
    putchar('\n');
}

/* Imprime matriz N5 x N5 */
void imprimir_tabuleiro5(int tab[N5][N5]) {
    printf("Tabuleiro 5x5:\n");
    for (int i = 0; i < N5; ++i) {
        for (int j = 0; j < N5; ++j) {
            printf("%d ", tab[i][j]);
        }
        printf("\n");
    }
    putchar('\n');
}

/* Imprime matriz N10 x N10 */
void imprimir_tabuleiro10(int tab[N10][N10]) {
    printf("Tabuleiro 10x10:\n");
    printf("   "); for (int c = 0; c < N10; ++c) printf("%2d ", c); printf("\n");
    for (int i = 0; i < N10; ++i) {
        printf("%2d ", i);
        for (int j = 0; j < N10; ++j) {
            printf("%2d ", tab[i][j]);
        }
        printf("\n");
    }
    putchar('\n');
}

/* Zera uma matriz 10x10 */
void limpar10(int tab[N10][N10]) {
    for (int i = 0; i < N10; ++i)
        for (int j = 0; j < N10; ++j)
            tab[i][j] = 0;
}

/* Zera uma matriz 5x5 */
void limpar5(int tab[N5][N5]) {
    for (int i = 0; i < N5; ++i)
        for (int j = 0; j < N5; ++j)
            tab[i][j] = 0;
}

/* Verifica se posição está dentro do tabuleiro 10x10 */
int dentro10(int x, int y) {
    return x >= 0 && x < N10 && y >= 0 && y < N10;
}

/* Verifica se posição está dentro do tabuleiro 5x5 */
int dentro5(int x, int y) {
    return x >= 0 && x < N5 && y >= 0 && y < N5;
}

/* ----------------------------------------------------------
   Nível Novato: tabuleiro 5x5 com 2 navios (1 vertical, 1 horizontal)
   ----------------------------------------------------------*/

/* Coloca um navio linear (horizontal ou vertical) em tab5 e preenche coords com as partes.
   tipo = 'H' ou 'V'. tamanho = número de células do navio.
   Retorna 1 se ok, 0 se inválido (fora do tabuleiro). */
int posicionar_navio5(int tab[N5][N5], int x, int y, char tipo, int tamanho, int coords[][2]) {
    if (tipo == 'H') {
        for (int j = 0; j < tamanho; ++j) {
            int yy = y + j;
            if (!dentro5(x, yy)) return 0;
            tab[x][yy] = 1;
            coords[j][0] = x; coords[j][1] = yy;
        }
    } else if (tipo == 'V') {
        for (int i = 0; i < tamanho; ++i) {
            int xx = x + i;
            if (!dentro5(xx, y)) return 0;
            tab[xx][y] = 1;
            coords[i][0] = xx; coords[i][1] = y;
        }
    } else {
        return 0;
    }
    return 1;
}

/* ----------------------------------------------------------
   Nível Aventureiro: tabuleiro 10x10 com 4 navios (incluindo diagonais)
   Marca navio com valor 3 conforme enunciado.
   ----------------------------------------------------------*/

/* Posiciona um navio linear em tab10 com marca 'marca' (ex: 3). */
int posicionar_navio10(int tab[N10][N10], int x, int y, char tipo, int tamanho, int marca) {
    if (tipo == 'H') {
        for (int j = 0; j < tamanho; ++j) {
            int yy = y + j;
            if (!dentro10(x, yy)) return 0;
            tab[x][yy] = marca;
        }
    } else if (tipo == 'V') {
        for (int i = 0; i < tamanho; ++i) {
            int xx = x + i;
            if (!dentro10(xx, y)) return 0;
            tab[xx][y] = marca;
        }
    } else if (tipo == 'D') { /* diagonal descendente: (x+i, y+i) */
        for (int i = 0; i < tamanho; ++i) {
            int xx = x + i, yy = y + i;
            if (!dentro10(xx, yy)) return 0;
            tab[xx][yy] = marca;
        }
    } else if (tipo == 'A') { /* diagonal ascendente: (x-i, y+i) */
        for (int i = 0; i < tamanho; ++i) {
            int xx = x - i, yy = y + i;
            if (!dentro10(xx, yy)) return 0;
            tab[xx][yy] = marca;
        }
    } else {
        return 0;
    }
    return 1;
}

/* ----------------------------------------------------------
   Nível Mestre: habilidades com padrões (cone, octaedro, cruz)
   Vamos representar as habilidades como matrizes 3x5 (linhas x colunas)
   e aplicar centradas em uma posição (cx,cy) do tabuleiro 10x10.
   Padrões conforme exemplos do enunciado.
   ----------------------------------------------------------*/

/* Define padrões 3x5 para as habilidades (0/1) */
void obter_padrao_cone(int pad[3][5]) {
    int tmp[3][5] = {
        {0,0,1,0,0},
        {0,1,1,1,0},
        {1,1,1,1,1}
    };
    memcpy(pad, tmp, sizeof(tmp));
}
void obter_padrao_octaedro(int pad[3][5]) {
    int tmp[3][5] = {
        {0,0,1,0,0},
        {0,1,1,1,0},
        {0,0,1,0,0}
    };
    memcpy(pad, tmp, sizeof(tmp));
}
void obter_padrao_cruz(int pad[3][5]) {
    int tmp[3][5] = {
        {0,0,1,0,0},
        {1,1,1,1,1},
        {0,0,1,0,0}
    };
    memcpy(pad, tmp, sizeof(tmp));
}

/* Aplica um padrão 3x5 centrado em (cx,cy) em uma matriz destino dest (10x10).
   Marca com 1 nas células afetadas. Se a parte do padrão cair fora do tabuleiro, ela é ignorada.
   Retorna número de células aplicadas. */
int aplicar_padrao_centroid(int dest[N10][N10], int pad[3][5], int cx, int cy) {
    /* O padrão tem 3 linhas (r) e 5 colunas (c).
       O centro do padrão deve estar alinhado com (cx,cy).
       Vamos assumir o "centro de padrao" na linha 1 (0-based) e coluna 2 (0-based).
       Então o deslocamento de origem do padrão (r0,c0) em relação a (cx,cy) é:
         r0 = cx - 1
         c0 = cy - 2
    */
    int applied = 0;
    int r0 = cx - 1;
    int c0 = cy - 2;
    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 5; ++c) {
            if (pad[r][c] == 1) {
                int tx = r0 + r;
                int ty = c0 + c;
                if (dentro10(tx, ty)) {
                    dest[tx][ty] = 1;
                    ++applied;
                }
            }
        }
    }
    return applied;
}

/* Imprime matriz habil (10x10) contendo 0/1, com cabeçalho */
void imprimir_habilidade10(int tab[N10][N10], const char *nome) {
    printf("Habilidade: %s (0 = não afetado, 1 = afetado)\n", nome);
    imprimir_tabuleiro10(tab);
}

/* ----------------------------------------------------------
   main: monta e exibe os três níveis
   ----------------------------------------------------------*/
int main() {
    /* ================= NÍVEL NOVATO =================
       - Tabuleiro 5x5
       - Dois navios: 1 vertical, 1 horizontal
       - Exibir coordenadas de cada parte dos navios
    */
    int tab5[N5][N5];
    limpar5(tab5);

    /* Defina as posições manualmente aqui (entrada via variáveis no código) */
    /* Exemplo: navio vertical com tamanho 3 começando em (0,1) */
    int nv_x = 0, nv_y = 1, nv_tam = 3; /* vertical */
    /* Exemplo: navio horizontal com tamanho 4 começando em (3,0) */
    int nh_x = 3, nh_y = 0, nh_tam = 3; /* horizontal */

    /* Arrays para armazenar coordenadas para exibição */
    int coordsV[5][2]; /* suporta até 5 partes */
    int coordsH[5][2];

    if (!posicionar_navio5(tab5, nv_x, nv_y, 'V', nv_tam, coordsV)) {
        printf("Erro: posicionamento vertical inválido (Novato).\n");
    }
    if (!posicionar_navio5(tab5, nh_x, nh_y, 'H', nh_tam, coordsH)) {
        printf("Erro: posicionamento horizontal inválido (Novato).\n");
    }

    printf("=== NÍVEL NOVATO ===\n\n");
    imprimir_coordenadas("Navio Vertical", coordsV, nv_tam);
    imprimir_coordenadas("Navio Horizontal", coordsH, nh_tam);
    imprimir_tabuleiro5(tab5);

    /* ================= NÍVEL AVENTUREIRO =================
       - Tabuleiro 10x10
       - Posicionar 4 navios (incluindo 2 na diagonal)
       - Exibir tabuleiro com 0 = vazio, 3 = navio
    */
    int tab10[N10][N10];
    limpar10(tab10);

    /* Posicionamentos manuais (mude as variáveis abaixo conforme desejar):
       Vamos posicionar:
         - navioA: horizontal, tamanho 4, início (1,1)
         - navioB: vertical, tamanho 3, início (4,7)
         - navioC: diagonal descendente, tamanho 4, início (2,5) -> usa tipo 'D'
         - navioD: diagonal ascendente, tamanho 3, início (8,1) -> usa tipo 'A'
       Todos com marca 3 conforme enunciado.
    */
    if (!posicionar_navio10(tab10, 1, 1, 'H', 4, 3)) printf("Erro posicionando navioA\n");
    if (!posicionar_navio10(tab10, 4, 7, 'V', 3, 3)) printf("Erro posicionando navioB\n");
    if (!posicionar_navio10(tab10, 2, 5, 'D', 4, 3)) printf("Erro posicionando navioC (diagonal)\n");
    if (!posicionar_navio10(tab10, 8, 1, 'A', 3, 3)) printf("Erro posicionando navioD (diagonal ascendente)\n");

    printf("=== NÍVEL AVENTUREIRO ===\n\n");
    printf("Legenda: 0 = vazio, 3 = navio\n");
    imprimir_tabuleiro10(tab10);

    /* ================= NÍVEL MESTRE =================
       - Habilidades (cone, octaedro, cruz) como matrizes
       - Aplicar centradas em posições escolhidas e exibir áreas afetadas (0/1)
    */
    int hab_cone[N10][N10];    /* 0/1 */
    int hab_octa[N10][N10];
    int hab_cruz[N10][N10];
    limpar10(hab_cone); limpar10(hab_octa); limpar10(hab_cruz);

    /* Padrões 3x5 */
    int pad_cone[3][5], pad_octa[3][5], pad_cruz[3][5];
    obter_padrao_cone(pad_cone);
    obter_padrao_octaedro(pad_octa);
    obter_padrao_cruz(pad_cruz);

    /* Posições centrais para aplicar as habilidades (entrada via variáveis no código) */
    int centro_cone_x = 5, centro_cone_y = 4;   /* (linha, coluna) */
    int centro_octa_x = 2, centro_octa_y = 7;
    int centro_cruz_x = 7, centro_cruz_y = 2;

    /* Aplica os padrões (marcando com 1 nas matrizes de habilidade) */
    int a1 = aplicar_padrao_centroid(hab_cone, pad_cone, centro_cone_x, centro_cone_y);
    int a2 = aplicar_padrao_centroid(hab_octa, pad_octa, centro_octa_x, centro_octa_y);
    int a3 = aplicar_padrao_centroid(hab_cruz, pad_cruz, centro_cruz_x, centro_cruz_y);

    printf("=== NÍVEL MESTRE ===\n\n");
    printf("Padrão 'cone' aplicado em centro (%d,%d) -> %d células afetadas.\n", centro_cone_x, centro_cone_y, a1);
    imprimir_habilidade10(hab_cone, "Cone");

    printf("Padrão 'octaedro' aplicado em centro (%d,%d) -> %d células afetadas.\n", centro_octa_x, centro_octa_y, a2);
    imprimir_habilidade10(hab_octa, "Octaedro");

    printf("Padrão 'cruz' aplicado em centro (%d,%d) -> %d células afetadas.\n", centro_cruz_x, centro_cruz_y, a3);
    imprimir_habilidade10(hab_cruz, "Cruz");

    /* Observação: se quiser sobrepor navios e habilidades em um único tabuleiro,
       você pode combinar as matrizes (por exemplo, marcar tab10 com 3 para navios e
       uma máscara de habilidades separada). */
    return 0;
}
