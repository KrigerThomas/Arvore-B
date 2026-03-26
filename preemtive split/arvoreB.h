#define ORDEM 6

typedef struct No {
    
	int qtdChaves;
	int chaves[ORDEM -1];
	int posOrigem[ORDEM - 1];
	struct No* filhos[ORDEM];
	
} No;

No* criarNo();

No* carregarArvore(char* nomeArquivo);

No* inserir(No* raiz, int chave, int pos); //Implementação da versao preventiva 

void inserirOrdenado(No* raiz, int chave, int pos);

void inserirRec(No* no, int chave, int pos);

void split(No* pai, int i);

void destruirArvore(No* raiz);

No* cadastrar(No* raiz, char* nomeArquivo);

void buscar(No* raiz, char* nomeArquivo);

int buscarChaveArvore(No* raiz, int chave); 

void salvarIndice(No* raiz, char* nomeArquivo);

void escreverNo(FILE* f, No* no);
