#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#define LINHAS 20
#define COLUNAS 8
int enderecoGERAL=0;
int grau_max = (COLUNAS>LINHAS) ? COLUNAS:LINHAS ;


//ESTRUTURAS 
typedef struct{
	int linha;
	int coluna;
}Celula;
typedef struct{
	int nro_vertices;
	int **pesos;
	int ***ponteiro;
} Grafo;
typedef struct{
	int inicio;
	int fim;
	int resultado;
}Operacao;
typedef struct{
	Celula A;
	Celula B;
}Intervalo;


//FUNÇÕES UTILIZADAS PARA GERENCIAR GRAFO
Grafo *cria_Grafo(int nro_vertices){
	Grafo *gr;
	gr = (Grafo *)malloc(sizeof(Grafo));

	if (gr != NULL){
		int i;
		gr->nro_vertices = nro_vertices;
		gr->pesos = (int **)malloc(nro_vertices * sizeof(int *));
		gr->ponteiro = (int ***)malloc( nro_vertices* sizeof(int **));

		for (int j = 0; j < nro_vertices; j++){
			gr->pesos[j] = (int *)malloc(grau_max * sizeof(int));
			gr->ponteiro[j] = (int **)malloc(grau_max * sizeof(int*));
			
			for(int z=0; z<grau_max; z++)
				gr->ponteiro[j][z] = &enderecoGERAL;
		}
		
	}
	return gr;
}
void liberar_Grafo(Grafo *gr){
	if (gr != NULL){
		for (int i = 0; i < gr->nro_vertices; i++){
			for(int z=0; z < grau_max; z++)
				free(gr->ponteiro[i][z]);
			free(gr->ponteiro[i]);
			free(gr->pesos[i]);
		}
		free(gr->pesos);
		free(gr->ponteiro);
		free(gr);
	}
	printf("GRAFO LIBERADO\n");
}


//FUNÇÕES PEGAR COLUNA/LINHA/TIPO DE OPERAÇÃO/INTERVALO
int getColun(char *comando){
	int letra;
	letra = comando[0]-65;
	//printf("Coluna (%s) == %d\n",comando, letra );
	return letra;
}
int getLine(char *comando){
	char letra[2];
	letra[0] = comando[1];
	letra[1] = comando[2];
	//printf("Linha (%s) == %d \n",comando, atoi(letra) );
	return atoi(letra)-1;
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
Intervalo getIntervalo(char *comando){
	Intervalo aux;
	int x;
	char letra[2];
	for(x=1; comando[x-1]!='('; x++);
	aux.A.coluna = comando[x++]-65;
	if(comando[x+1]!='.'){
		letra[0] = comando[x];
		letra[1] = comando[x+1];
		aux.A.linha = atoi(letra);
	}else{
		letra[0] = comando[x];
		aux.A.linha = atoi(letra);
	}
	x+=3;
	strcpy(letra, "");
	aux.B.coluna = comando[x++]-65;
	if(comando[x+1]!=')'){
		letra[0] = comando[x];
		letra[1] = comando[x+1];
		aux.B.linha = atoi(letra);
	}else{
		letra[0] = comando[x];
		aux.B.linha = atoi(letra);
	}
	return aux;
}


//FUNÇÕES PARA CALCULAR RESULTADO
int getValor(char *comando){
	char numero[10], cont = 0;

	int ini = 0;
	if(getLine(comando)>9){
		ini = 1;
	}

	for(int x=3+ini; comando[x]!='\0'; x++){
		numero[cont++] = comando[x];
	}
	printf("VALOR: %d\n", atoi(numero));
	return atoi(numero);
}
int getSoma(Grafo *gr,char *comando){
	
	Intervalo aux = getIntervalo(comando);
	
	int soma = 0;
	for(int x = aux.A.linha; x <= aux.B.linha; x++){
		for(int y = aux.A.coluna; y <= aux.B.coluna; y++){
			soma += gr->pesos[x-1][y];

		}
	}
	printf("SOMA: %d\n", soma);
	return soma;
}
int getMax(Grafo *gr,char *comando){
	
	Intervalo aux = getIntervalo(comando);
	
	int max = gr->pesos[aux.A.linha-1][aux.A.coluna];

	for(int x = aux.A.linha; x <= aux.B.linha; x++){
		for(int y = aux.A.coluna; y <= aux.B.coluna; y++){
			if( gr->pesos[x-1][y] > max){
				max = gr->pesos[x-1][y];
			}

		}
	}
	printf("MAX: %d\n", max);
	return max;
}
int getMin(Grafo *gr,char *comando){
	
	Intervalo aux = getIntervalo(comando);
	
	int min = INFINITY;

	for(int x = aux.A.linha; x <= aux.B.linha; x++){
		for(int y = aux.A.coluna; y <= aux.B.coluna; y++){
			if( gr->pesos[x-1][y] < min){
				min = gr->pesos[x-1][y];
			}

		}
	}
	printf("MIN: %d\n", min);
	return min;
}
int getMedia(Grafo *gr,char *comando){
	Intervalo aux = getIntervalo(comando);
	
	int soma = 0, cont=0;
	for(int x = aux.A.linha; x <= aux.B.linha; x++){
		for(int y = aux.A.coluna; y <= aux.B.coluna; y++){
			soma += gr->pesos[x-1][y];
			if(gr->pesos[x-1][y] != 0)
				cont++;
		}
	}
	printf("MÉDIA: %d\n", (int)soma/cont);
	return (int)soma/cont;
}
int getReferencia(Grafo *gr,char *comando){
	
	int lineA = getLine(comando);
	int colunA = getColun(comando);
	int x, lineB, colunB;
	char letra[3];

	for(x=1; comando[x-1]!='='; x++);
	
	colunB = comando[x++]-65;

	if(comando[x+1]>=30 && comando[x+1]<=39){
		letra[0] = comando[x];
		letra[1] = comando[x+1];
		lineB = atoi(letra);
	}else{
		letra[0] = ' ';
		letra[1] = comando[x];
		lineB = atoi(letra);
	}

	gr->ponteiro[lineA][colunA] = &(gr->pesos[lineB][colunB]);
}


//FUNÇÕES PARA TRATAR ENTRADA
int setDADO(Grafo *gr, char *comando, int *referenciaCelula){
	int resultado=0;
	int op = getFunction(comando);
	/*
	0 = Valor 	1 - getSoma 	2 - MAX
	3 - MIN 	4 - Media 		5 - REFERENCIA
	*/
	if( op == 0)
		resultado = getValor(comando);
	else if( op == 1)
		resultado = getSoma(gr,comando);
	else if( op == 2)
		resultado = getMax(gr,comando);
	else if( op == 3)
		resultado = getMin(gr,comando);
	else if( op == 4)
		resultado = getMedia(gr,comando);
	else if( op == 5){
		*referenciaCelula = 1;
	}
	return resultado;
}
void processarEntrada(Grafo *gr, char *comando){
	int auxReferencia=0;
	int valorCelula = setDADO(gr, comando, &auxReferencia);
	int line = getLine(comando);
	int colun = getColun(comando);

	if(auxReferencia == 0){
		gr->pesos[line][colun] = valorCelula;
		gr->ponteiro[line][colun] = &enderecoGERAL;
	}else{
		getReferencia(gr,comando);
	}
}


//FUNÇÃO PARA MOSTRAR TABELA
void mostrarPesos(Grafo *gr){
	for (int i = 0; i < LINHAS; i++){
		printf("%2d |",i+1 );
		for(int x=0; x< COLUNAS; x++){
			if(gr->ponteiro[i][x] == &enderecoGERAL){
				printf("%7d|",gr->pesos[i][x]);
			}else{
				printf("%7d|",*(gr->ponteiro[i][x]));
			}
		}
		printf("\n");
	}
}

int main(){

	int op;
	char entrada[100];
	Grafo *gr = NULL;
	gr = cria_Grafo(LINHAS+COLUNAS);

	processarEntrada(gr, "A1 10\0");
	processarEntrada(gr, "B1 20\0");
	processarEntrada(gr, "A2 30\0");
	processarEntrada(gr, "C1 @soma(A1..B2)\0");
	processarEntrada(gr, "D1 =C1\0");

	while (0==0){
		printf("\n--------- MENU ---------\n");
		printf(" [1]-Reiniciar Tabela\n");
		printf(" [2]-Inserir Comando\n");
		printf(" [3]-Mostrar Tabela de dados\n");
		printf(" [0]-SAIR\n");
		printf("----------------------\n");
		printf("Escolha uma opção: ");
		scanf("%d", &op);
		switch (op){
			case 1:
	
				gr = cria_Grafo(LINHAS+COLUNAS);

				printf("\n- - Grafo CRIADO - -\n");
				
				break;
			case 2:
				setbuf(stdin, NULL);
				scanf("%[^\n]s", entrada);
				processarEntrada(gr, entrada);
				printf("\n- - comando EFETUADO - -\n");
				break;

			case 3:
				printf("\n- - TABELA DADOS - -\n");
				printf("___|___A___|___B___|___C___|___D___|___E___|___F___|___G___|___H___|\n");
				mostrarPesos(gr);
				break;
			case 0:
				exit(0);
				break;
		}
	}
	return 0;
}