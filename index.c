#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"

#define SIZE 1000
/*2^16, por causa do limite
 de caracteres
*/
#define NMAX 32768

static size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while(c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;

}

long int hash(char *str){
	long int key=0, n=1;
	for(int i=0;str[i]!=0;i++){
		key += n * str[i];
		n++;
	}
	return key;
}

long int hashKey(char *str, int size){
	return hash(str) % size;
}

Index* criaIndex(int size, char *textFile){
	Index* ind = (Index*)malloc(sizeof(Index));
	ind->textFile = textFile;
	ind->size=size;
	ind->key = (listaChave**)malloc(sizeof(listaChave*)*size);
	for(int i=0;i<size;i++)
		ind->key[i] = crialistaChave();
	return ind;
}

void liberaIndex(Index* ind){
	for(int i=0;i<ind->size;i++){
		ind->key[i]=liberalistaChave(ind->key[i]);
	}
	free(ind->key);
	free(ind);
}

//retorna -1 se a chave n existe
int buscaKey(Index *idx, int h, char* key){
	listaChave *tmp = idx->key[h];
	int r = -1, i=0;
	while(tmp!=NULL){
		if(strcmp(key, tmp->str)==0){
			r=i;
			break;
		}
		i++;
		tmp=tmp->prox;
	}
	return r;
}

static char pontuacao(char c){
	return (c >= '!' && c <= '/') ||
		(c >= ':' && c <= '?') || 
		c == 96 || c==13||
		(c>='{' && c <= '}');
}

void minuscula(char * string){
	int i;
	for(i=0; string[i]!='\0';i++)	
		string[i]=tolower(string[i]);
}


int criaOcorrencias(char *textFile, char *key, listaInt **ocorrencias){
	FILE *arq = fopen(textFile, "r");
	if(arq == NULL){
		return 3;
	}
	int l=1;
	char *linha=NULL;
	int read;
	size_t len;
	while ((read = getline(&linha, &len, arq) )!= -1) {
	    char *palavra = (char*)malloc(sizeof(char)*strlen(linha));
	    int i = 0, k = 0;
	    minuscula(linha);
	  //  printf("Linha: %s", linha);
	    while(i<strlen(linha)){
	    	if((linha[i]==' ' || linha[i]=='\n' || pontuacao(linha[i])) && k>0){
	    		palavra[k] = '\0';
	    		//printf("Achei as palavra: %s\n", palavra);
	    		
	    			if(strcmp(key, palavra) == 0){
	    				(*ocorrencias) = inserelistaInt((*ocorrencias), l);
	    			}

	    		k=0;
	    	}
	    	else{
	    		if(!((linha[i]==' ')||(linha[i]=='\n')|| pontuacao(linha[i]))){
	    			palavra[k] = linha[i];
	    			k++;
	    		}
	    		
	    	}
	    	i++;
	    }
	    free(palavra);
	    l++;
	}
	fclose(arq);
	return 0;
}

int index_put(Index *idx, char *key){
	long int h = hashKey(key, idx->size);
	listaInt *ocorrencias = crialistaInt();
	int e = criaOcorrencias(idx->textFile, key, &ocorrencias);
	
	//printf("indice hash: %ld\n", h );
	if(e!=0)
		return e;
	//chave n esta na lista com hash h
	if(buscaKey(idx, h, key)==-1){
		idx->key[h] = inserelistaChave(idx->key[h], key, ocorrencias);
	}
	else{
		liberalistaInt(idx->key[h]->ocorrencias);
		idx->key[h]->ocorrencias = ocorrencias;
	}
	//imprimelistaInt(ocorrencias);
	return 0;
}


char* tira(char* linha){
	int i,j;	
	int tamLinha = 1000000;	 
	char *string=(char*)malloc(sizeof(char)*tamLinha);		 
	for(j=0,i=0;j<tamLinha && linha[j]!='\0';j++){
		
                if( ( !isalpha(linha[j]) && linha[j]!='-') || linha[j]=='\n' ){		    
			break;
		}
		else{
			string[i]=tolower(linha[j]);
			i++;				         	  
		}
	}
	string[i]='\0';
	return string;  	  
}

int index_createfrom(char *keyFile, char *textFile, Index **idx){
	*idx = criaIndex(SIZE, textFile);
	FILE *arq = fopen(keyFile, "r");
	//erro ao ler arquivo de keys
	if(arq == NULL){
		return 1;
	}
	/*no arquivo de keys, cada linha e uma chave.
	ao inserir a chave, ja busca no de texto as ocorrencias
	*/
	//char *linha = NULL;
	//size_t len;
	int len=100000000;
	char *linha=(char*)malloc(len*sizeof(char));

	//printf("Lendo arquivo keys\n\n");
	
    while(fgets(linha,len,arq) != NULL){
	//while (getline(&linha, &len, arq) != -1) {
	//	printf("Estou lendo: ");
	//	printf("%s\n", linha);
		linha = tira(linha);

		//if(strlen(linha)>1)
		//linha[strlen(linha)-1]='\0';
		//printf("%s\n", linha);
	    int e = index_put(*idx, linha);
	    if(e!=0)
	    	return e;
	}
//	printf("Fim do Lendo arquivo keys\n\n");
	fclose(arq);
	return 0;
}

int index_get(Index *idx, char* key, int **ocurrences, int *num_ocurrences){
	int h = hashKey(key, idx->size);
	int k = buscaKey(idx, h, key);
	if(k==-1)
		return 1;
	listaChave *tmp = idx->key[h];
	for(int i=0;i<k;i++){
		if(tmp!=NULL)
			tmp=tmp->prox;
		else
			return 2;
	}
	listaInt *li = tmp->ocorrencias;
	int n_ocorrencia=0;
	while(li!=NULL){
		li=li->prox;
		n_ocorrencia++;
	}
	*num_ocurrences = n_ocorrencia;
	li = tmp->ocorrencias;
	int *oc = (int*)malloc(sizeof(int)*n_ocorrencia);
	for(int i=0;li!=NULL;i++){
		oc[i]=li->n;
		li=li->prox;
	}
	*ocurrences = oc;
	return 0;
}

void print_recursivo(listaInt *oc, int primeiro){

	if(oc->prox!=NULL){
		print_recursivo(oc->prox, -1);
		if(oc->n==primeiro){
			printf("%d\n ", oc->n);	
		}
		else{
			printf("%d, ", oc->n);
		}
	}
	else
		if(oc->n==primeiro){
			printf("%d\n ", oc->n);	
		}
		else{
			printf("%d, ", oc->n);
		}
	
}

int index_print(Index *idx){
	listaChave *l = crialistaChave();
	for(int i=0;i<idx->size;i++){
		listaChave *tmp = idx->key[i];
		while(tmp!=NULL){
			l = inserelistaChaveOrdem(l, tmp->str, tmp->ocorrencias);
			tmp=tmp->prox;
		}
	}

	listaChave *tmp = l;
	while(tmp!=NULL){
		l=l->prox;
		printf("%s: ", tmp->str);
		listaInt *oc = tmp->ocorrencias;

		if(oc==NULL)printf("\n");
		else
			print_recursivo(oc, oc->n);
		/*while(oc!=NULL){
			if(oc->prox!=NULL)
				printf("%d, ", oc->n);
			else
				printf("%d\n", oc->n);
			oc=oc->prox;
		}*/
		free(tmp->str);
		free(tmp);
		tmp=l;
	}
	return (0);
}

