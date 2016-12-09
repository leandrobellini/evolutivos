#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


typedef struct dados{
	int destino, origem, vertices;
	int custo, *custos;
}dados;

typedef struct dadosCaminho{
	int *caminho;
	int custoTempo;
	int custoPerigo;
	int custoPedagio;
	int custo_aux;
}dadosCaminho;


int partida = 1, 
	destinoFinal = 5;


void dijkstra(int vertices,int origem,int destino,int *custos, dadosCaminho *c)
{
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
	//printf("\tDe %c para %c: \t", origem+64, destino+64);
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

		int count = 0;
		
		for (i = cont; i > 0 ; i--) {
			//printf("%c -> ", tmp[i-1]+64);
			c->caminho[count++] = tmp[i-1];
		}
		//printf("%c", destino+64);

		c->caminho[count++] = destino;

		for(i = count; i < vertices; i++)
			c->caminho[i] = -1;
		
		//printf("\n\tCusto:  %d\n",(int) dist[destino-1]);

		c->custo_aux = (int) dist[destino-1];
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

void add(dados *d, FILE *f)
{
	int i, j;
	
	do {
		//printf("\n-> Lendo numero de vertices...");
		fscanf(f,"%d",&(d->vertices));
	} while (d->vertices < 2 );


	if (!d->custos)
		free(d->custos);
	d->custos = (int *) malloc(sizeof(int)*d->vertices*d->vertices);
	for (i = 0; i <= d->vertices * d->vertices; i++)
		(d->custos)[i] = -1;

	int numeroArestas;

	fscanf(f,"%d",&numeroArestas);
	//printf("\n-> Lendo numero de arestas...");

	for(i = 0; i < numeroArestas; i++){
		fscanf(f, "%d %d %d", &(d->origem), &(d->destino), &(d->custo));
		//printf("\n%c -> %c [%d]", (d->origem)+64, (d->destino)+64, d->custo);
		(d->custos)[(d->origem-1) * d->vertices + d->destino - 1] = d->custo;
	}
}

void procurar(dados *d,  dadosCaminho *c)
{
 
	/* Azul */
	//printf("\033[36;1m");
	//printf("\n\nLista dos Menores Caminhos no Grafo Dado: \n");
		 	

	dijkstra(d->vertices, partida, destinoFinal, d->custos, c);

	/* Volta cor nornal */
	//printf("\033[m");
}

int main(int argc, char **argv) {
	/*
	typedef struct dados{
		int destino, origem, vertices;
		int custo, *custos;
		FILE *f;
	}dados;
	*/

	/*
	typedef struc c{
		int *caminho;
		int custoTempo;
		int custoPerigo;
		int custoPedagio;
	}
	*/

	int i;

	FILE *f;

	dados d;
	d.destino = 0;
	d.origem = 0;
	d.vertices = 0;
	d.custo = 0;
	d.custos = NULL;

	//---------------------  TEMPO -----------------------

	f = (FILE *)fopen("tempo.txt","r");


	//Cria grafo
	add(&d, f);

	dadosCaminho cTempo;		//armazena os dados do menor caminho para tempo

	cTempo.caminho = (int *)malloc(d.vertices*sizeof(int));	//salvar caminho aqui

	//Procura o caminho com o menor tempo
	procurar(&d, &cTempo);

	//Agora calculo o Perigo e Pedagio para esse caminho(que tem o menor tempo)


	cTempo.custoTempo = cTempo.custo_aux;
	cTempo.custoPerigo = 4;
	cTempo.custoPedagio = 10;


	//---------------------  Perigo -----------------------
	d.destino = 0;
	d.origem = 0;
	d.vertices = 0;
	d.custo = 0;
	d.custos = NULL;

	f = (FILE *)fopen("perigo.txt","r");

	//Cria grafo
	add(&d, f);

	dadosCaminho cPerigo;	

	cPerigo.caminho = (int *)malloc(d.vertices*sizeof(int));	//salvar caminho aqui

	//Procura o caminho com o menor tempo
	procurar(&d, &cPerigo);

	//Agora calculo o Perigo e Pedagio para esse caminho(que tem o menor tempo)


	cPerigo.custoPerigo = cPerigo.custo_aux;
	cPerigo.custoTempo = 6;
	cPerigo.custoPedagio = 8;



	//---------------------  Pedagio -----------------------

	d.destino = 0;
	d.origem = 0;
	d.vertices = 0;
	d.custo = 0;
	d.custos = NULL;

	f = (FILE *)fopen("pedagio.txt","r");

	//Cria grafo
	add(&d, f);

	dadosCaminho cPegadio;	

	cPegadio.caminho = (int *)malloc(d.vertices*sizeof(int));	//salvar caminho aqui

	//Procura o caminho com o menor tempo
	procurar(&d, &cPegadio);

	//Agora calculo o Perigo e Pedagio para esse caminho(que tem o menor tempo)


	cPegadio.custoPedagio = cPegadio.custo_aux;
	cPegadio.custoTempo = 6;
	cPegadio.custoPerigo = 6;



	//*********************
	printf("\nCaminho Tempo:\n");

	printf("\033[36;1m");
	for(i = 0; cTempo.caminho[i] != -1; i++)
		printf("%d  ", cTempo.caminho[i]);

	printf("\033[m");

	printf("\nTempo: %d, Perigo: %d, Pegadio: %d\n", cTempo.custoTempo, cTempo.custoPerigo, cTempo.custoPedagio);

	//*********************
	printf("\nCaminho Perigo:\n");

	printf("\033[36;1m");
	for(i = 0; cPerigo.caminho[i] != -1; i++)
		printf("%d  ", cPerigo.caminho[i]);

	printf("\033[m");

	printf("\nTempo: %d, Perigo: %d, Pegadio: %d\n", cPerigo.custoTempo, cPerigo.custoPerigo, cPerigo.custoPedagio);

	//*********************
	printf("\nCaminho Pegadio:\n");

	printf("\033[36;1m");
	for(i = 0; cPegadio.caminho[i] != -1; i++)
		printf("%d  ", cPegadio.caminho[i]);

	printf("\033[m");

	printf("\nTempo: %d, Perigo: %d, Pegadio: %d\n", cPegadio.custoTempo, cPegadio.custoPerigo, cPegadio.custoPedagio);


	

	return 0;
}
