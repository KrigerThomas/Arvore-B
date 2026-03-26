#include <stdio.h>
#include <stdlib.h>
#include "arvoreB.h"

No* criarNo() {
    No* novo = malloc(sizeof(No));
    
    novo->qtdChaves = 0;
    
    for (int i = 0; i < ORDEM; i++) {
        novo->chaves[i] = -1;
        novo->posOrigem[i] = -1;
    }
    
    for (int i = 0; i < ORDEM + 1; i++){
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

    Retorno r = inserirRec(raiz, chave, pos);

    if (r.promove) {
        No* novaRaiz = criarNo();

        novaRaiz->chaves[0] = r.chave;
        novaRaiz->posOrigem[0] = r.pos;

        novaRaiz->filhos[0] = raiz;
        novaRaiz->filhos[1] = r.novoFilho;

        novaRaiz->qtdChaves = 1;

        return novaRaiz;
    }

    return raiz;
}

Retorno inserirRec(No* no, int chave, int pos) {

    Retorno r;
    r.promove = 0;

    int i = no->qtdChaves - 1;

    // CASO FOLHA
    if (no->filhos[0] == NULL) {

        inserirOrdenado(no, chave, pos);

        // overflow de chaves 
        if (no->qtdChaves == ORDEM) {

            r.promove = 1;

            int meio = (ORDEM - 1) / 2;

            No* novo = criarNo();

            r.chave = no->chaves[meio];
            r.pos = no->posOrigem[meio];

            novo->qtdChaves = no->qtdChaves - meio - 1;

            for (int j = 0; j < novo->qtdChaves; j++) {
                novo->chaves[j] = no->chaves[j + meio + 1];
                novo->posOrigem[j] = no->posOrigem[j + meio + 1];
            }

            no->qtdChaves = meio;
            
            for (int j = meio; j < ORDEM; j++) { //Limpa as chaves que sobraram 
            no->chaves[j] = -1;
            no->posOrigem[j] = -1;
}

            r.novoFilho = novo;
        }

        return r;
    }

    // CASO INTERNO
    while (i >= 0 && chave < no->chaves[i])
        i--;

    i++;

    Retorno filho = inserirRec(no->filhos[i], chave, pos);

    if (!filho.promove)
        return r;

    // inserir chave promovida
    for (int j = no->qtdChaves - 1; j >= i; j--) {
        no->chaves[j + 1] = no->chaves[j];
        no->posOrigem[j + 1] = no->posOrigem[j];
    }

    for (int j = no->qtdChaves; j >= i + 1; j--) {
        no->filhos[j + 1] = no->filhos[j];
    }

    no->chaves[i] = filho.chave;
    no->posOrigem[i] = filho.pos;
    no->filhos[i + 1] = filho.novoFilho;

    no->qtdChaves++;

    // overflow
    if (no->qtdChaves == ORDEM) {

        r.promove = 1;

        int meio = (ORDEM - 1) / 2;

        No* novo = criarNo();

        r.chave = no->chaves[meio];
        r.pos = no->posOrigem[meio];

        novo->qtdChaves = no->qtdChaves - meio - 1;

        for (int j = 0; j < novo->qtdChaves; j++) {
            novo->chaves[j] = no->chaves[j + meio + 1];
            novo->posOrigem[j] = no->posOrigem[j + meio + 1];
        }

        for (int j = meio; j < ORDEM; j++) { //Limpando as chaves que foram copiadas 
            no->chaves[j] = -1;
            no->posOrigem[j] = -1;
        }
        
        // filhos
        for (int j = 0; j <= novo->qtdChaves; j++) {
            novo->filhos[j] = no->filhos[j + meio + 1];
            
            no->filhos[j + meio + 1] = NULL; //Limpando meus filhos
        }

        no->qtdChaves = meio;

        r.novoFilho = novo;
    }

    return r;
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

