#include <ctype.h>

typedef struct listaInt{
	int n;
	struct listaInt *prox;
}listaInt;

/*cada elemento da lista tem uma chave e uma lista de ocorrencias*/
typedef struct listaChave{
	listaInt *ocorrencias;
	char *str;
	struct listaChave *prox;
}listaChave;

listaInt* inserelistaInt(listaInt *l, int n);
//retira do topo
listaInt* retiralistaInt(listaInt *l);
listaInt* crialistaInt();
listaInt* liberalistaInt(listaInt *l);
void imprimelistaInt(listaInt *l);

listaChave* inserelistaChave(listaChave *l, char *str, listaInt *li);
//insere ordenado por str
listaChave* inserelistaChaveOrdem(listaChave *l, char *str, listaInt *li);
//retira do topo
listaChave* retiralistaChave(listaChave *l);
listaChave* crialistaChave();
listaChave* liberalistaChave(listaChave *l);


typedef struct Index{
	//vetor de lista
	listaChave **key;
	char *textFile;
	int size;
}Index;

long int hash(char *str);
long int hashKey(char *str, int size);
Index* criaIndex(int size, char *textFile);
void liberaIndex(Index* ind);
//retorna -1 se a chave n existe
int buscaKey(Index *idx, int h, char* key);
int criaOcorrencias(char *textFile, char *key, listaInt **ocorrencias);
int index_put(Index *idx, char *key);
int index_createfrom(char *keyFile, char *textFile, Index **idx);
int index_get(Index *idx, char* key, int **ocurrences, int *num_ocurrences);
int index_print(Index *idx);