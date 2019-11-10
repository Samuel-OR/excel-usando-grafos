#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

typedef struct{
	int nro_vertices;
	int **arestas;
	int eh_ponderado;
	float **pesos;
	int *grau;
	int grau_max;
} Grafo;
typedef struct{
	int inicio;
	int fim;
	int resultado;
}Operacao;
typedef struct{
	int linha;
	int coluna;
}Celula;

//COLETAR TEMPO EM MICROSEGUNDOS
long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}


//FUNÇÕES UTILIZADAS PARA GERENCIAR GRAFO
Grafo *cria_Grafo(int nro_vertices, int grau_max, int eh_ponderado){
	Grafo *gr;
	gr = (Grafo *)malloc(sizeof(Grafo));

	if (gr != NULL){
		int i;
		gr->nro_vertices = nro_vertices;
		gr->grau_max = grau_max;
		gr->eh_ponderado = (eh_ponderado != 0) ? 1 : 0;
		gr->grau = (int *)calloc(nro_vertices, sizeof(int));

		gr->arestas = (int **)malloc(nro_vertices * sizeof(int *));
		for (i = 0; i < nro_vertices; i++){
			gr->arestas[i] = (int *)malloc(grau_max * sizeof(int *));
		}
		if (gr->eh_ponderado == 1){
			gr->pesos = (float **)malloc(nro_vertices * sizeof(float *));
			for (int j = 0; j < nro_vertices; j++){
				gr->pesos[j] = (float *)malloc(grau_max * sizeof(float));
			}
		}
	}
	return gr;
}
int insereAresta(Grafo *gr, int orig, int dest, int eh_digrafo, float peso){
	int resul = 0;
	if (gr != NULL){
		if ((orig >= 0 && orig < gr->nro_vertices) || (dest >= 0 && dest < gr->nro_vertices)){
			gr->arestas[orig][gr->grau[orig]] = dest;
			if (gr->eh_ponderado)
				gr->pesos[orig][gr->grau[orig]] = peso;
			gr->grau[orig]++;
			if (eh_digrafo == 0)
				insereAresta(gr, dest, orig, 1, peso);
			resul = 1;
		}
	}
	return resul;
}
void liberar_Grafo(Grafo *gr){
	if (gr != NULL){
		int i;
		for (i = 0; i < gr->nro_vertices; i++)
			free(gr->arestas[i]);
		free(gr->arestas);

		if (gr->eh_ponderado == 1){
			for (i = 0; i < gr->nro_vertices; i++)
				free(gr->pesos[i]);
			free(gr->pesos);
		}
		free(gr->grau);
		free(gr);
	}
}


int inputComando(char *comando){
	int x;
}

int getColun(char *comando){
	int letra;
	letra = comando[0]-65;
	printf("Coluna (%s) == %d\n",comando, letra );
	return letra;
}
int getLine(char *comando){
	char letra[2];
	letra[0] = comando[1];
	letra[1] = comando[2];
	printf("Linha (%s) == %d \n",comando, atoi(letra) );
	return atoi(letra);
}
Celula getCelula(char *comando){
	Celula aux;
	aux.linha = getLine(comando);
	aux.coluna = getColun(comando);
	return aux;
}

int getFunction(char *comando){
	int resul = -1;
	int ini = 0;
	if(getLine(comando)>9){
		ini = 1;
	}

	if(comando[3+ini]=='@'){
		if(comando[4+ini]=='s'){
			//1 - getSoma
			resul = 1;
		}else if(comando[4+ini]=='m'){
			if(comando[5+ini]=='a'){
				//2 - MAX
				resul = 2;
			}else if(comando[5+ini]=='i'){
				//3 - MIN
				resul = 3;
			}else if(comando[5+ini]=='e'){
				//4 - Media
				resul = 4;
			}
		}
	}else if(comando[3+ini] =='='){
		//5 - REFERENCIA
		resul = 5;
	}else{
		//0 = Valor
		resul = 0;
	}
	return resul;
}

int getValor(char *comando){
	char numero[10], cont = 0;

	int ini = 0;
	if(getLine(comando)>9){
		ini = 1;
	}

	for(int x=3+ini; comando[x]!='\0'; x++){
		numero[cont++] = comando[x];
	}
	printf("VALOR: %d", atoi(numero));
	return atoi(numero);
}
int getSoma(char *comando){
	
	printf("TEM QUE FAZER\n");
}
int getMax(char *comando){
	
	printf("TEM QUE FAZER\n");
}
int getMin(char *comando){
	
	printf("TEM QUE FAZER\n");
}
int getMedia(char *comando){
	
	printf("TEM QUE FAZER\n");
}
int getReferencia(char *comando){
	
	printf("TEM QUE FAZER\n");
}
int setDADO(char *comando, int *referenciaCelula){
	int resultado;
	int op = getFunction(comando);
	/*
	0 = Valor 	1 - getSoma 	2 - MAX
	3 - MIN 	4 - Media 		5 - REFERENCIA
	*/
	if( op == 0)
		resultado = getValor(comando);
	else if( op == 1)
		resultado = getSoma(comando);
	else if( op == 2)
		resultado = getMax(comando);
	else if( op == 3)
		resultado = getMin(comando);
	else if( op == 4)
		resultado = getMedia(comando);
	else if( op == 5){
		resultado = getReferencia(comando);
		*referenciaCelula = 1;
	}
	return resultado;
}


int main(){
	int op, eh_ponderado, grau_max, nro_vertices;
	int orig, dest, eh_digrafo, peso, Inicial;
	int *visitados;
	float *visitados_pesos, valorP;
	struct timeval tempo_inicial, tempo_final;
	float tempo_total;

	Grafo *gr = NULL;

	while (1==0){
		printf("\n--------- MENU ---------\n");
		printf(" [1]-Criar/Reiniciar Grafo\n");
		printf(" [2]-Inserir Aresta\n");
		//printf(" [3]-Mostrar\n");
		//printf("[4]-Remover\n");
		//printf(" [5]-Busca Profundidade\n");
		//printf(" [6]-Busca Largura\n");
		printf(" [7]-Liberar\n");
		printf(" [0]-SAIR\n");
		printf("----------------------\n");
		printf("Escolha uma opção: ");
		scanf("%d", &op);
		switch (op){
			case 1:

				printf("Numero de vertices: ");
				scanf("%d", &nro_vertices);

				printf("Numero grau máximo: ");
				scanf("%d", &grau_max);

				printf("Grafo Ponderado? (1-Sim |0-Não): ");
				scanf("%d", &eh_ponderado);

				if (gr != NULL)
					liberar_Grafo(gr);
				gr = cria_Grafo(nro_vertices+1, grau_max, eh_ponderado);
				printf("\n- - Grafo CRIADO - -\n");
				break;
				

			case 2:
				
				/*
				//INSERINDO INDIVIDUALMENTE
				printf("Vertice de Origem: ");
				scanf("%d", &orig);

				printf("Vertice de Destino: ");
				scanf("%d", &dest);

				printf("É Digrafo? (1-Sim |0-Não): ");
				scanf("%d", &eh_digrafo);

				printf("Peso da relação: ");
				scanf("%d", &peso);
				
				insereAresta(gr, orig, dest, eh_digrafo, peso);
				printf("Aresta INSERIDA com Sucesso.\n");
				*/
							
				printf("\n- - Arestas INSERIDAS --\n");
				break;

			/*
			case 5:
				printf("Vertice Inicial: ");
				scanf("%d", &Inicial);
				printf("Informe o valor que deseja pagar: ");
				scanf("%f", &valorP);

				visitados = (int *)calloc(nro_vertices, sizeof(int));
				visitados_pesos = (float *)calloc(nro_vertices, sizeof(float));
				Pilha *maior_p = (Pilha *)malloc(sizeof(Pilha));
				maior_p->qtd_elementos = 0;
				maior_p->caminho = NULL;

				gettimeofday(&tempo_inicial, NULL);
				buscaProfundidade_Grafo(gr, Inicial, visitados, visitados_pesos, valorP, maior_p);
				gettimeofday(&tempo_final, NULL);

				tempo_total = (tempo_final.tv_sec - tempo_inicial.tv_sec) * (int)1e6 + (tempo_final.tv_usec - tempo_inicial.tv_usec);
				printf("\nTEMPO TOTAL: %.3f microsegundos\n", tempo_total);
			
				printf("\nCaminho Prof: [ ");	
				mostrarPilha(maior_p->caminho);
				printf("]\nQuantidade de cidades: %d\n",maior_p->qtd_elementos-1);
				break;


			case 6:
				printf("Vertice Inicial: ");
				scanf("%d", &Inicial);
				printf("Informe o valor que deseja pagar: ");
				scanf(" %f", &valorP);

				visitados = (int *)calloc(nro_vertices, sizeof(int));
				visitados_pesos = (float *)calloc(nro_vertices, sizeof(float));
				
				maior_p->qtd_elementos = 0;
				maior_p->caminho = NULL;

				gettimeofday(&tempo_inicial, NULL);
				buscaLargura_Grafo(gr, Inicial, visitados, maior_p, valorP);
				gettimeofday(&tempo_final, NULL);

				tempo_total = (tempo_final.tv_sec - tempo_inicial.tv_sec) * (int)1e6 + (tempo_final.tv_usec - tempo_inicial.tv_usec);
				printf("\nTEMPO TOTAL: %.3f microsegundos\n", tempo_total);

				printf("\nCaminho Larg: [ ");
				mostrarPilha(maior_p->caminho);
				printf("]\nQuantidade de cidades: %d\n", maior_p->qtd_elementos);

				break;

			*/

			case 7:
				if (gr != NULL)
					liberar_Grafo(gr);
				printf("\n- - Grafo LIBERADO - -\n");
				break;

			
			case 0:
				exit(0);
				break;
		}
	}
	int *aux;
	getCelula("A5");
	setDADO("A1 ", aux);
	return 0;
}