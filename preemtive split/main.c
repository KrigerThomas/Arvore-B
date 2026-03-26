#include <stdio.h>
#include <stdlib.h>
#include "arvoreB.h"

int main(int argc, char *argv[])
{
    if (argc != 3){
        printf("Passe os nomes dos arquivos de fonte e indice como parametros validos");
        return -1;
    }
    
    char* arquivoFonte = argv[1];
    char* arquivoIndice = argv[2];
    
    No* raiz = carregarArvore(arquivoFonte);
    
    
    int opcao;
	do {
	    printf("Digite 1 para cadastrar\n");
	    printf("Digite 2 para pesquisar\n");
	    printf("Digite 3 para gravar\n");
	    printf("Digite 4 para encerrar\n");
	    
	    scanf("%d", &opcao);
	    
	    switch (opcao){
	        case 1:
	            raiz = cadastrar(raiz, arquivoFonte);
	            break;
	        case 2:
	            buscar(raiz, arquivoFonte);
	            break;
	        case 3:
	            printf("Gravando o conteúdo da árvore B no arquivo '%s'...\n", arquivoIndice);
	            salvarIndice(raiz, arquivoIndice);
	            break;
	       case 4:
	            destruirArvore(raiz);
	            break;
	       default:
	            printf("Insira uma opção válida.\n");
	    }
	} while (opcao != 4);

	return 0;
}



