#define ORDEM 3

typedef struct No {
    
	int qtdChaves;
	int chaves[ORDEM];
	int posOrigem[ORDEM];
	struct No* filhos[ORDEM + 1];
	
} No;

typedef struct {
    int promove; //booleano
    int chave;
    int pos;
    No* novoFilho;
} Retorno;

No* criarNo();

No* carregarArvore(char* nomeArquivo);

No* inserir(No* raiz, int chave, int pos); //Implementação da abordagem bottom-top

void inserirOrdenado(No* raiz, int chave, int pos);

Retorno inserirRec(No* no, int chave, int pos);

void destruirArvore(No* raiz);

No* cadastrar(No* raiz, char* nomeArquivo);

void buscar(No* raiz, char* nomeArquivo);

int buscarChaveArvore(No* raiz, int chave); 

void salvarIndice(No* raiz, char* nomeArquivo);

void escreverNo(FILE* f, No* no);
