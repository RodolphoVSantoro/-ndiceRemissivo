#include <stdlib.h>
#include <string.h>
#include<stdio.h>
#include "index.h"

listaInt* inserelistaInt(listaInt *l, int n){
	listaInt *novo = (listaInt*)malloc(sizeof(listaInt));
	novo->n=n;
	novo->prox=l;
	return novo;
}

void imprimelistaInt(listaInt *l){
	listaInt *aux;
	aux = l;
		printf("Lista de posicoes:\n");
		if(aux!=NULL){
			
			while(aux->prox!=NULL){
				printf("%d\n", aux->n);
				aux = aux->prox;
			}
		}

}

//retira do topo
listaInt* retiralistaInt(listaInt *l){
	if(l!=NULL){
		listaInt *tmp = l;
		l=l->prox;
		free(tmp);
	}
	return l;
}

listaInt* crialistaInt(){
	return NULL;
}

listaInt* liberalistaInt(listaInt *l){
	while(l!=NULL)
		l=retiralistaInt(l);
	return l;
}

listaChave* inserelistaChave(listaChave *l, char *str, listaInt *li){
	listaChave *novo = (listaChave*)malloc(sizeof(listaChave));
	novo->str = (char*)malloc(sizeof(char)*strlen(str));
	strcpy(novo->str, str);
	novo->ocorrencias = li;
	novo->prox=l;
	return novo;
}

//insere ordenado por str
listaChave* inserelistaChaveOrdem(listaChave *l, char *str, listaInt *li){
	listaChave *novo = (listaChave*)malloc(sizeof(listaChave));
	novo->str = (char*)malloc(sizeof(char)*strlen(str));
	strcpy(novo->str, str);
	novo->ocorrencias = li;
	novo->prox=NULL;
	listaChave *tmp = l, *ant = NULL;
	if(tmp==NULL){
		l = novo;
	}
	else{
		while(strcmp(str, tmp->str)>0){
			ant = tmp;
			tmp=tmp->prox;
			if(tmp==NULL)
				break;
		}
		if(ant==NULL){
			novo->prox=l;
			l=novo;
		}
		else{
			ant->prox=novo;
			novo->prox=tmp;
		}
	}
	return l;
}

//retira do topo
listaChave* retiralistaChave(listaChave *l){
	if(l!=NULL){
		listaChave *tmp = l;
		l=l->prox;
		free(tmp->str);
		liberalistaInt(tmp->ocorrencias);
		free(tmp);
	}
	return l;
}

listaChave* crialistaChave(){
	return NULL;
}

listaChave* liberalistaChave(listaChave *l){
	while(l!=NULL)
		l=retiralistaChave(l);
	return l;
}
