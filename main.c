#include <stdio.h>
#include "index.h"

int main( int argc, char **argv ) {
	if( argc < 3 ) {
		printf("Erro: numero insuficiente de parametros:\n");
		printf("Sintaxe: %s "
			"key_file_name txt_file_name\n", argv[0]);
		return 1;
	}
	Index *idx;
	if( index_createfrom( argv[1], argv[2], &idx ) ) {
		printf( "Erro: criacao do indice\n" );
		return 1;
	}
	char keyword[17];
	printf( "Qual a palavra-chave a procurar?\n" );
	scanf( " %16[^\n]", keyword );
	int *occurrences;
	int n_occurrences;
	if(index_get(idx, keyword, &occurrences, &n_occurrences))
		printf( "Erro: palavra nao pertence ao indice\n" );
	else {
	if( n_occurrences <= 0 )
		printf( "Nao ha ocorrencias de %s\n", keyword );
	else {
		printf( "%d ocorrencias de %s: ", n_occurrences, keyword );
		int i;
		for( i=0; i<n_occurrences-1; i++ )
			printf( "%d, ", occurrences[i] );
			printf( "%d\n", occurrences[n_occurrences-1] );
		}
	}
	printf( "Indice completo:\n" );
	if( index_print( idx ) ) {
		printf( "Erro: impressao do indice\n" );
		return 1;
	}
	char new_keyword[17];
	printf( "Qual a palavra-chave a inserir?\n" );
	scanf( " %16[^\n]", new_keyword );
	if( index_put( idx, new_keyword ) ) {
		printf( "Erro: insercao no indice\n" );
		return 1;
	}
	printf( "Novo indice completo:\n" );
	if( index_print( idx ) ) {
		printf( "Erro: impressao do novo indice\n" );
		return 1;
	}
	return 0;
}