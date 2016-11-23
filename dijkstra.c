/*   
* Este programa implementa o algoritmo de Dijkstra para o problema do 
* caminho de custo minimo em grafos dirigidos com custos positivos nas
* arestas.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


int destino, origem, vertices = 0;
int custo, *custos = NULL; 
FILE *f;

void dijkstra(int vertices,int origem,int destino,int *custos)
{
	//printf("chamou o dijkstra!!!\n");
	int i,v, cont = 0;
	int *ant, *tmp;  
	int *z;     /* vertices para os quais se conhece o caminho minimo */
	double min;
	double dist[vertices]; /* vetor com os custos dos caminhos */


	/* aloca as linhas da matriz */
	ant = calloc (vertices, sizeof(int *));
	tmp = calloc (vertices, sizeof(int *));
	if (ant == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		exit(-1);
	}

	z = calloc (vertices, sizeof(int *));
	if (z == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		exit(-1);
	}

	for (i = 0; i < vertices; i++) {
		if (custos[(origem - 1) * vertices + i] !=- 1) {
			ant[i] = origem - 1;
			dist[i] = custos[(origem-1)*vertices+i];
		}
		else {
			ant[i]= -1;
			dist[i] = HUGE_VAL;
		}
		z[i]=0;
	}
	z[origem-1] = 1;
	dist[origem-1] = 0;

	/* Laco principal */
	do {

		/* Encontrando o vertice que deve entrar em z */
		min = HUGE_VAL;
		for (i=0;i<vertices;i++)
			if (!z[i])
				if (dist[i]>=0 && dist[i]<min) {
					min=dist[i];v=i;
				}

		/* Calculando as distancias dos novos vizinhos de z */
		if (min != HUGE_VAL && v != destino - 1) {
			z[v] = 1;
			for (i = 0; i < vertices; i++)
				if (!z[i]) {
					if (custos[v*vertices+i] != -1 && dist[v] + custos[v*vertices+i] < dist[i]) {
					   	dist[i] = dist[v] + custos[v*vertices+i];
						ant[i] =v;
				   	}
        		}
		}
	} while (v != destino - 1 && min != HUGE_VAL);

	/* Mostra o Resultado da busca */
	printf("\tDe %c para %c: \t", origem+64, destino+64);
	if (min == HUGE_VAL) {
		printf("Nao Existe\n");
		printf("\tCusto: \t- \n");
	}
	else {
		i = destino;
		i = ant[i-1];
		while (i != -1) {
		//	printf("<-%d",i+1);
			tmp[cont] = i+1;
			cont++;
			i = ant[i];
		}
		
		for (i = cont; i > 0 ; i--) {
			printf("%c -> ", tmp[i-1]+64);
		}
		printf("%c", destino+64);
		
		printf("\n\tCusto:  %d\n",(int) dist[destino-1]);
	}
}

void limpar(void)
{
     printf("\033[2J"); /* limpa a tela */
     printf("\033[1H"); /* poe o curso no topo */
}

void cabecalho(void)
{
 	//limpar();
	printf("Implementacao do Algoritmo de Dijasktra\n");
	printf("Comandos:\n");
	printf("\t d - Adicionar um Grafo\n"
	  	   "\t r - Procura Os Menores Caminhos no Grafo\n"
	  	   "\t CTRL+c - Sair do programa\n");
	printf(">>> ");
}

void add(void)
{
	int i, j;
	
	do {
		printf("\n-> Lendo numero de vertices...");
		fscanf(f,"%d",&vertices);
	} while (vertices < 2 );

	if (!custos)
		free(custos);
	custos = (int *) malloc(sizeof(int)*vertices*vertices);
	for (i = 0; i <= vertices * vertices; i++)
		custos[i] = -1;

	int numeroArestas;

	fscanf(f,"%d",&numeroArestas);
	printf("\n-> Lendo numero de arestas...");

	for(i = 0; i < numeroArestas; i++){
		fscanf(f, "%d %d %d", &origem, &destino, &custo);
		printf("\n%c -> %c [%d]", origem+64, destino+64, custo);
		custos[(origem-1) * vertices + destino - 1] = custo;
	}
}

void procurar(void)
{
	int i, j;
 
	/* Azul */
	printf("\033[36;1m");
	printf("\n\nLista dos Menores Caminhos no Grafo Dado: \n");
		 	
	
	//quero ir do 1 até o 8  (A -> H)
	i = 1;
	j = 8;

	dijkstra(vertices, i,j, custos);

	/* Volta cor nornal */
	printf("\033[m");
}

int main(int argc, char **argv) {
	int i, j; 
	char opcao[3], l[50]; 

	f = (FILE *)fopen("grafo.txt","r");


	//Cria grafo
	add();

	//Procura menor caminho
	procurar();


	return 0;
}
