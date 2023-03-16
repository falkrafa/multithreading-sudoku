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
    int verdadeiro;
    int verdadeiro2;
    int verdadeiro3;
}Sudoku;

typedef struct {
    Sudoku* sudoku;
    int valorId;
} ThreadParametros;

// Função para verificar se as linhas possuem valores iguais
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

// Função para verificar se as colunas possuem valores iguais
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

// Função para verificar se as subgrades possuem valores iguais
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
    Sudoku sudoku = {linhas,colunas,linhaSub,colunaSub, NULL,0,0,0};

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
    pthread_t threadsLinha[sudoku.linhas];
    pthread_t threadsColuna[sudoku.colunas];
    pthread_t threadsSub[sudoku.linhas];
    ThreadParametros thread_parametros[sudoku.linhas];

    int contaThread =0;
    
    for(int t = 0; t < sudoku.linhas; t++){
        thread_parametros[t].sudoku = &sudoku;
        thread_parametros[t].valorId = t;
        pthread_create(&threadsLinha[t], NULL, checandoValoresLinha, &thread_parametros[t]);
        pthread_create(&threadsColuna[t], NULL, checandoValoresColuna, &thread_parametros[t]);
        pthread_create(&threadsSub[t], NULL, checandoValoresSub, &thread_parametros[t]);
        contaThread += 3;
    }
    for (int t = 0; t < sudoku.linhas; t++){
        pthread_join(threadsLinha[t], NULL);
        pthread_join(threadsColuna[t], NULL);
        pthread_join(threadsSub[t], NULL);
    }
    char *output = "output.txt";
    FILE *outputFile = fopen(output, "w");
    int verdadeiro = sudoku.verdadeiro;
    int verdadeiro2 = sudoku.verdadeiro2;
    int verdadeiro3 = sudoku.verdadeiro3;
    if(verdadeiro == 1 || verdadeiro2 == 1 || verdadeiro3 == 1){
         fprintf(outputFile, "FAIL\n");
    }
    else{
        fprintf(outputFile, "SUCCESS\n");
    }
    printf("%d", contaThread);
    free(sudoku.matriz);
    fclose(input);

    return 0;
}