#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_SIZE 1000
typedef struct{
    int linhas;
    int colunas;
    int linhaSub;
    int colunaSub;
    int **matriz;
}Sudoku;

typedef struct{
    Sudoku sudoku;
    pthread_t threadId;
    int valorId;
}thread;

int verdadeiro,verdadeiro2,verdadeiro3;
// Função para verificar se as linhas possuem valores iguais
void checandoValoresLinha(Sudoku *sudoku) {
    for (int linhaNum = 0; linhaNum < sudoku->linhas; linhaNum++) {
        for (int colunasNum = 0; colunasNum < sudoku->colunas; colunasNum++) {
            for (int copia = colunasNum + 1; copia < sudoku->colunas; copia++) {
                if (sudoku->matriz[linhaNum][colunasNum] == sudoku->matriz[linhaNum][copia]) {
                    verdadeiro = 1; 
                }
            }
        }
    }
    printf("%d", verdadeiro);
}

// Função para verificar se as colunas possuem valores iguais
void checandoValoresColuna(Sudoku *sudoku) {
    for (int colunasNum = 0; colunasNum < sudoku->colunas; colunasNum++) {
        for (int linhasNum = 0; linhasNum < sudoku->linhas; linhasNum++) {
            for (int copia = linhasNum + 1; copia < sudoku->linhas; copia++) {
                if (sudoku->matriz[linhasNum][colunasNum] == sudoku->matriz[copia][colunasNum]) {
                    verdadeiro2 = 1;
                }
            }
        }
    }
    printf("%d",verdadeiro2);
}

// Função para verificar se as subgrades possuem valores iguais
void checandoValoresSub(Sudoku *sudoku) {
    for (int subLinhasNum = 0; subLinhasNum < sudoku->linhas; subLinhasNum += sudoku->linhaSub) {
        for (int subColunasNum = 0; subColunasNum < sudoku->colunas; subColunasNum += sudoku->colunaSub) {
            for (int copiaSubLinhas = subLinhasNum; copiaSubLinhas < subLinhasNum + sudoku->linhaSub; copiaSubLinhas++) {
                for (int copiaSubColunas = subColunasNum; copiaSubColunas < subColunasNum + sudoku->colunaSub; copiaSubColunas++) {
                    for (int finalCopiaLinha = copiaSubLinhas; finalCopiaLinha < subLinhasNum + sudoku->linhaSub; finalCopiaLinha++) {
                        for (int finalCopiaColuna = copiaSubColunas + 1; finalCopiaColuna < subColunasNum + sudoku->colunaSub; finalCopiaColuna++) {
                            if (sudoku->matriz[copiaSubLinhas][copiaSubColunas] == sudoku->matriz[finalCopiaLinha][finalCopiaColuna]) {
                                verdadeiro3 = 1;
                            }
                        }
                    }
                }
            }
        }
    }
    printf("%d", verdadeiro3);
}

int main(int argc, char **argv) {

    //vendo se passou parametros suficiente
    if (argv[1] == 0) {
        printf("Invalid number of parameters\n");
        exit(0);
    }

    FILE *input = fopen(argv[1], "r");
    char valorLinha[MAX_SIZE];

    //vendo se o arquivo existe
    if(input == NULL){
        printf("Error to read file\n");
        exit(0);
    }
    //Pega a primeira linha do arquivo e caso seja vazia retorna mensagem
    char *pegaLinha = fgets(valorLinha, MAX_SIZE, input);

    if(pegaLinha == NULL){
        printf("File out of format\n");
        exit(0);
    }

    int linhas, colunas, linhaSub, colunaSub;
    char *linha1 = strchr(valorLinha, 'x');

    //aqui ele vai checar para saber se existe um primeiro valor e se esta escrito de maneira correta
    if(linha1 == NULL || sscanf(valorLinha, "%d x %d", &linhas, &colunas) != 2){
        printf("File out of format\n");
        exit(0);
    }
    *linha1 = '\0'; //vai percorrer ate achar o x e vai parar

    linha1++; //continua depois do x
    
    //vai verificar se existe um valor depois do x
    if(sscanf(linha1, "%d", &colunas) != 1){
        printf("File out of format\n");
        exit(0);
    }
    //pega a segunda linha e verificar para ver se esta escrita de maneira correta
    pegaLinha = fgets(valorLinha, MAX_SIZE, input);
    if(pegaLinha == NULL || sscanf(valorLinha, "%d x %d", &linhaSub, &colunaSub) != 2){
        printf("File out of format\n");
        exit(0);
    }
    if(linhas != colunas){
        printf("File out of format");
        exit(0);
    }
    Sudoku sudoku = {linhas,colunas,linhaSub,colunaSub, NULL};

    sudoku.matriz = malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        sudoku.matriz[i] = malloc(linhas * sizeof(int *));
    }


    // aqui vamos pegar os valores a partir da terceira linha
    for (int i = 0; i < linhas; i++) {
        pegaLinha = fgets(valorLinha, MAX_SIZE, input);
        if (pegaLinha == NULL) {
            printf("File out format\n");
            exit(0);
        }
        char *tok = strtok(valorLinha, " ");
        // vendo se tem espaco em branco
        for (int j = 0; j < colunas; j++) {
            if(tok == NULL){
                printf("File out of format\n");
                exit(0);
            }
            if(sscanf(tok, "%d", &sudoku.matriz[i][j]) != 1){
                printf("File out of format\n");
                exit(0);
            }
            if(sudoku.matriz[i][j]<=0 || sudoku.matriz[i][j] > linhas){
                printf("File out of format\n");
                exit(0);
            }
            tok = strtok(NULL, " ");
        }

        if(tok != NULL){
            printf("File out of format\n");
            exit(0);
        }
    }
    int cont = 0;
    char teste;
    teste = fgetc(input);
    if(teste != EOF){
    printf("File out of format\n");
    exit(0);
    }

    sudoku.linhas = linhas;
    sudoku.linhaSub = linhaSub;
    sudoku.colunas = colunas;
    sudoku.colunaSub = colunaSub;


    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%d", sudoku.matriz[i][j]);
        }
        printf("\n");
    }
    checandoValoresLinha(&sudoku);
    checandoValoresColuna(&sudoku);
    checandoValoresSub(&sudoku);
    
    if(verdadeiro == 1 || verdadeiro2 == 1 || verdadeiro3 == 1){
        printf("valores iguais");
    }
    else{
        printf("valores diferentes");
    }
    
    free(sudoku.matriz);
    fclose(input);

    return 0;
}