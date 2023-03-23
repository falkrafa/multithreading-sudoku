#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_SIZE 10000
typedef struct{
    int linhas;
    int colunas;
    int linhaSub;
    int colunaSub;
    int **matriz;
    int verdadeiro;
    int verdadeiro2;
    int verdadeiro3;
}Sudoku;

typedef struct {
    int valorId;
    // int comeco;
    // int fim;
    Sudoku* sudoku;
} ThreadParametros;

void *checandoValoresLinha(void* args) {

    ThreadParametros* thread_parametros = (ThreadParametros*) args;

    for (int linhaNum = 0; linhaNum < thread_parametros->sudoku->linhas; linhaNum++) {
        for (int colunasNum = 0; colunasNum < thread_parametros->sudoku->colunas; colunasNum++) {
            for (int copia = colunasNum + 1; copia < thread_parametros->sudoku->colunas; copia++) {
                if (thread_parametros->sudoku->matriz[linhaNum][colunasNum] == thread_parametros->sudoku->matriz[linhaNum][copia]) {
                    thread_parametros->sudoku->verdadeiro = 1; 
                }
            }
        }
    }
}

void *checandoValoresColuna(void* args) {
    ThreadParametros* thread_parametros = (ThreadParametros*) args;

    for (int colunasNum = 0; colunasNum < thread_parametros->sudoku->colunas; colunasNum++) {
        for (int linhasNum = 0; linhasNum < thread_parametros->sudoku->linhas; linhasNum++) {
            for (int copia = linhasNum + 1; copia < thread_parametros->sudoku->linhas; copia++) {
                if (thread_parametros->sudoku->matriz[linhasNum][colunasNum] == thread_parametros->sudoku->matriz[copia][colunasNum]) {
                    thread_parametros->sudoku->verdadeiro2= 1;
                }
            }
        }
    }
}

void *checandoValoresSub(void* args) {
    ThreadParametros* thread_parametros = (ThreadParametros*) args;

    for (int subLinhasNum = 0; subLinhasNum < thread_parametros->sudoku->linhas; subLinhasNum += thread_parametros->sudoku->linhaSub) {
        for (int subColunasNum = 0; subColunasNum < thread_parametros->sudoku->colunas; subColunasNum += thread_parametros->sudoku->colunaSub) {
            for (int copiaSubLinhas = subLinhasNum; copiaSubLinhas < subLinhasNum + thread_parametros->sudoku->linhaSub; copiaSubLinhas++) {
                for (int copiaSubColunas = subColunasNum; copiaSubColunas < subColunasNum + thread_parametros->sudoku->colunaSub; copiaSubColunas++) {
                    for (int finalCopiaLinha = copiaSubLinhas; finalCopiaLinha < subLinhasNum + thread_parametros->sudoku->linhaSub; finalCopiaLinha++) {
                        for (int finalCopiaColuna = copiaSubColunas + 1; finalCopiaColuna < subColunasNum + thread_parametros->sudoku->colunaSub; finalCopiaColuna++) {
                            if (thread_parametros->sudoku->matriz[copiaSubLinhas][copiaSubColunas] == thread_parametros->sudoku->matriz[finalCopiaLinha][finalCopiaColuna]) {
                                thread_parametros->sudoku->verdadeiro3= 1;
                            }
                        }
                    }
                }
            }
        }
    }
}

int main(int argc, char **argv) {

    if (argv[1] == 0 || argc > 2) {
        printf("Invalid number of parameters");
        exit(0);
    }

    FILE *input = fopen(argv[1], "r");
    char valorLinha[MAX_SIZE];

    if(input == NULL){
        printf("Error to read file");
        exit(0);
    }

    char *pegaLinha = fgets(valorLinha, MAX_SIZE, input);

    if(pegaLinha == NULL){
        printf("File out of format");
        exit(0);
    }

    int linhas, colunas, linhaSub, colunaSub;
    char *linha1 = strchr(valorLinha, 'x');

    if(linha1 == NULL || sscanf(valorLinha, "%d x %d", &linhas, &colunas) != 2){
        printf("File out of format");
        exit(0);
    }
    *linha1 = '\0';
    linha1++;

    if(sscanf(linha1, "%d", &colunas) != 1){
        printf("File out of format");
        exit(0);
    }
    pegaLinha = fgets(valorLinha, MAX_SIZE, input);
    if(pegaLinha == NULL || sscanf(valorLinha, "%d x %d", &linhaSub, &colunaSub) != 2){
        printf("File out of format");
        exit(0);
    }
    if(linhas != colunas){
        printf("File out of format");
        exit(0);
    }
    if((linhaSub * colunaSub) != linhas){
        printf("File out of format");
        exit(0);
    }
    Sudoku sudoku = {linhas,colunas,linhaSub,colunaSub, NULL,0,0,0};

    sudoku.matriz = malloc(linhas * sizeof(int *));
    for (int i = 0; i < linhas; i++) {
        sudoku.matriz[i] = malloc(linhas * sizeof(int *));
    }

    for (int i = 0; i < linhas; i++) {
        pegaLinha = fgets(valorLinha, MAX_SIZE, input);
        if (pegaLinha == NULL) {
            printf("File out of format");
            exit(0);
        }
        char *tok = strtok(valorLinha, " ");
        for (int j = 0; j < colunas; j++) {
            if(tok == NULL){
                printf("File out of format");
                exit(0);
            }
            if(sscanf(tok, "%d", &sudoku.matriz[i][j]) != 1){
                printf("File out of format");
                exit(0);
            }
            if(sudoku.matriz[i][j]<=0 || sudoku.matriz[i][j] > linhas){
                printf("File out of format");
                exit(0);
            }
            tok = strtok(NULL, " ");
        }

        if(tok != NULL){
            printf("File out of format");
            exit(0);
        }
    }
    int cont = 0;
    int teste;
    teste = fgetc(input);
    if(teste != EOF){
        printf("File out of format");
        exit(0);
    }

    sudoku.linhas = linhas;
    sudoku.linhaSub = linhaSub;
    sudoku.colunas = colunas;
    sudoku.colunaSub = colunaSub;
    int contaThread = 0;
    pthread_t threadsLinha[sudoku.linhas];
    pthread_t threadsColuna[sudoku.colunas];
    pthread_t threadsSub[sudoku.linhas];
    ThreadParametros* thread_parametros = malloc(sudoku.linhas * sizeof(ThreadParametros));

    
    for(int t = 0; t < 9; t++){
        thread_parametros[t].sudoku = &sudoku;
        thread_parametros[t].valorId = t;
        pthread_create(&threadsLinha[t], NULL, checandoValoresLinha, &thread_parametros[t]);
        pthread_create(&threadsColuna[t], NULL, checandoValoresColuna, &thread_parametros[t]);
        pthread_create(&threadsSub[t], NULL, checandoValoresSub, &thread_parametros[t]);
        // contaThread+=3;
    }

    for (int t = 0; t < 9; t++){
        pthread_join(threadsLinha[t], NULL);
        pthread_join(threadsColuna[t], NULL);
        pthread_join(threadsSub[t], NULL);
    }

    char *output = "sudoku_rmf2.out";
    FILE *outputFile = fopen(output, "w");
    int verdadeiro = sudoku.verdadeiro;
    int verdadeiro2 = sudoku.verdadeiro2;
    int verdadeiro3 = sudoku.verdadeiro3;
    if(verdadeiro == 1 || verdadeiro2 == 1 || verdadeiro3 == 1){
         fprintf(outputFile, "FAIL");
    }
    else{
        fprintf(outputFile, "SUCCESS");
    }
    // printf("%d", contaThread);
    free(sudoku.matriz);
    fclose(input);
    free(thread_parametros);

    return 0;
}