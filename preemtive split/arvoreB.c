#include <stdio.h>
#include <stdlib.h>
#include "arvoreB.h"

No* criarNo() {
    No* novo = malloc(sizeof(No));
    
    novo->qtdChaves = 0;
    
    for (int i = 0; i < ORDEM - 1; i++) {
        novo->chaves[i] = -1;
        novo->posOrigem[i] = -1;
    }
    
    for (int i = 0; i < ORDEM; i++){
        novo->filhos[i] = NULL;
    }

    return novo;
}

No* carregarArvore(char* nomeArquivo) {

    FILE* f = fopen(nomeArquivo, "r");

    if (f == NULL) {
        return NULL;
    }

    No* raiz = NULL;
    char linha[100];

    while (1) {
        
        int pos = ftell(f);

        if (fgets(linha, sizeof(linha), f) == NULL)
            break;
            
        int id;
        char nome[50];
        char telefone[20];
        
        sscanf(linha, "%d %s %s", &id, nome, telefone); //string scan formatted = sscanf

        //if (raiz == NULL)
            //raiz = criarNo();

        raiz = inserir(raiz, id, pos);
    }

    fclose(f);
    return raiz;
}

No* inserir(No* raiz, int chave, int pos) {
    if (raiz == NULL) {
        raiz = criarNo();
        inserirOrdenado(raiz, chave, pos);
        return raiz;
    }

    // se raiz estiver cheia faz o split 
    if (raiz->qtdChaves == ORDEM - 1) {
        No* novaRaiz = criarNo();

        novaRaiz->filhos[0] = raiz;

        split(novaRaiz, 0);

        inserirRec(novaRaiz, chave, pos);

        return novaRaiz;
    }

    inserirRec(raiz, chave, pos);
    return raiz;
}

void inserirRec(No* no, int chave, int pos) {

    int i = no->qtdChaves - 1;

    // caso 1: nó é folha
    if (no->filhos[0] == NULL) {
        inserirOrdenado(no, chave, pos);
        return;
    }

    // caso 2: nó interno → achar filho
    while (i >= 0 && chave < no->chaves[i])
        i--;

    i++; // posição do filho

    // se filho estiver cheio faz o split antes de descer
    if (no->filhos[i]->qtdChaves == ORDEM - 1) {

        split(no, i);

        // decidir qual lado seguir após split
        if (chave > no->chaves[i])
            i++;
    }

    inserirRec(no->filhos[i], chave, pos);
}

void split(No* pai, int i) {

    No* cheio = pai->filhos[i];
    No* novo = criarNo();

    int maxChaves = ORDEM - 1;
    int meio = (ORDEM - 1) / 2; // para ordem 4 → 1

    novo->qtdChaves = (ORDEM - 1) - meio - 1; //Dependendo da ordem, o numero chaves pode começar com 0

    for (int j = 0; j < novo->qtdChaves; j++) { //Copia (atribui) os elementos do meio para frente do Cheio para o novo
        novo->chaves[j] = cheio->chaves[j + meio + 1];
        novo->posOrigem[j] = cheio->posOrigem[j + meio + 1];
    }

    // se não for folha, copia filhos
    if (cheio->filhos[0] != NULL) {
        for (int j = 0; j <= meio; j++) {
            novo->filhos[j] = cheio->filhos[j + meio + 1];
            cheio->filhos[j + meio + 1] = NULL;
        }
    }

    cheio->qtdChaves = meio;

    // abrir espaço no pai (Só entra se i != 0)
    for (int j = pai->qtdChaves; j > i; j--) {
        pai->filhos[j + 1] = pai->filhos[j];
    }

    pai->filhos[i + 1] = novo;

    // mover chaves no pai (Só entra se i != 0)
    for (int j = pai->qtdChaves - 1; j >= i; j--) {
        pai->chaves[j + 1] = pai->chaves[j];
        pai->posOrigem[j + 1] = pai->posOrigem[j];
    }

    // subir chave do meio
    pai->chaves[i] = cheio->chaves[meio];
    pai->posOrigem[i] = cheio->posOrigem[meio];

    pai->qtdChaves++;
    
    for (int j = meio; j < ORDEM - 1; j++){ //Limpa o meu No cheio novamente  
        cheio->chaves[j] = -1;
        cheio->posOrigem[j] = -1;
    }
}

void inserirOrdenado(No* no, int chave, int pos){ //Segue a mesma ideia do Insertion Sort
    int i = no->qtdChaves - 1;
    
    // move as chaves maiores para a direita
    while(i >= 0 && chave < no->chaves[i]){
        no->chaves[i + 1] = no->chaves[i];
        no->posOrigem[i + 1] = no->posOrigem[i];
        
        i--;
    }
    
    // insere na posição correta
    no->chaves[i + 1] = chave;
    no->posOrigem[i + 1] = pos;
    
    no->qtdChaves++;
}

void destruirArvore(No* raiz) {
    if (raiz == NULL)
        return;

    for (int i = 0; i <= raiz->qtdChaves; i++) {
        destruirArvore(raiz->filhos[i]);
    }

    free(raiz);
}

No* cadastrar(No* raiz, char* nomeArquivo){
    
    FILE* f = fopen(nomeArquivo, "a+"); //a+ significa leitura e acréscimo (append)
    
    if (f == NULL) {
        printf("Erro ao abrir arquivo.\n");
        return raiz;
    }
    
    int id; //matricula
    char nome[50];
    char telefone[20];
    
    printf("Digite a matricula: ");
    scanf("%d", &id);

    printf("Digite o nome: ");
    scanf("%s", nome);
    
    printf("Digite o telefone: ");
    scanf("%s", telefone);
    
    fseek(f, 0, SEEK_END); //Corrige o bug de impressão garantindo que o buffer esteja sempre no fim do arquivo
    int pos = ftell(f); //Retorna posição atual do cursor no arquivo(em bytes desde o início)
    
    fprintf(f, "%d %s %s\n", id, nome, telefone);

    fclose(f);
    
    raiz = inserir(raiz, id, pos);

    return raiz;
}

int buscarChaveArvore(No* raiz, int chave){
    if (raiz == NULL)
        return -1;
        
    int i = 0;

    while (i < raiz->qtdChaves && chave > raiz->chaves[i])
        i++;
        
    if (i < raiz->qtdChaves && chave == raiz->chaves[i])
        return raiz->posOrigem[i];
        
    if (raiz->filhos[i] == NULL) //Funciona sem esse if pois a condição de parada já trata se o filho for nulo mas isso evita uma chamada recursiva extra 
        return -1;
        
    return buscarChaveArvore(raiz->filhos[i], chave);
}

void buscar(No* raiz, char* nomeArquivo) {
    int chave;
    printf("Digite a chave para buscar: ");
    scanf("%d", &chave);

    int pos = buscarChaveArvore(raiz, chave);
    
    if (pos == -1) {
        printf("Registro nao encontrado.\n");
        return;
    }

    FILE* f = fopen(nomeArquivo, "r");

    fseek(f, pos, SEEK_SET); //fseek(fp, offset, origem)

    char linha[100];
    fgets(linha, sizeof(linha), f);

    printf("Registro encontrado: %s\n", linha);

    fclose(f);
}

void salvarIndice(No* raiz, char* nomeArquivo) {
    FILE* f = fopen(nomeArquivo, "w");

    if (f == NULL) {
        printf("Erro ao criar indice.txt\n");
        return;
    }

    if (raiz != NULL) {
        fprintf(f, "Endereço do No Raiz: %p\n\n", raiz);
        fprintf(f, "Árvore de ordem: %d\n", ORDEM);
        escreverNo(f, raiz);
    }

    fclose(f);
}

void escreverNo(FILE* f, No* no) {

    if (no == NULL)
        return;

    fprintf(f, "%p | ", no);

    fprintf(f, "%d | ", no->qtdChaves);

    for (int i = 0; i < ORDEM - 1; i++) {
        fprintf(f, "%d | ", no->chaves[i]);
    }

    for (int i = 0; i <= ORDEM - 1; i++) {
        if (no->filhos[i] != NULL)
            fprintf(f, "%p | ", no->filhos[i]);
        else
            fprintf(f, "NULL | ");
    }

    fprintf(f, "\n");

    for (int i = 0; i <= no->qtdChaves; i++) {
        escreverNo(f, no->filhos[i]);
    }
}

