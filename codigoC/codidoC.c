#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define NUM_SALAS 10
#define NUM_DIAS 5
#define NUM_HORAS 20-8+1
#define NOME_UC 10


// DEFENICAO DE TIPOS
typedef enum {LIVRE,OCUPADA} estadoHora;

typedef struct hora{
    estadoHora estado;
    char UC[NOME_UC];
}HORA;

typedef HORA OcupacaoSalaDia[5][NUM_HORAS];

typedef OcupacaoSalaDia complexo ;


// DECLARACAO DE FUNCOES
void imprimeSalas();
void iniciarComplexoLivre(complexo *c);
void criarFicheiro(char* nomeFich);
void colocarDadosFicheiro(complexo *c, char *nomeFicheiro);
void lerDadosFicheiro(complexo *c, char *nomeFicheiro);
void creditos();
int menu();
int esperar();
void verHorarioSemanalSala(complexo *c,int nSala);
void verSalasLivresHora(complexo *c);
void verEstadoHora(complexo *c);
void inserirUC(complexo *c);
int subMenu5defenicoes();

//FUNCAO MAIN
int main(){
    complexo c[NUM_SALAS]; // c é o complexo com NUM_SALAS. c[sala][dia][hora].LIVRE/OCUPADA
    char *nomeFicheiro="salas.txt";
	int nOpcao, nSala, hora,n;
	char sala[2+1];

    printf("Se for a primeira vez que utiliza este programa, deve instala lo.\n\n");
	printf("Prima <1> para instalar o programa. Prima <2> para ir para o Menu Inicial: ");
	scanf(" %d",&n);
	if(n==1){
		iniciarComplexoLivre(c);  // Inicia o complexo com o valor LIVRE. c[SALA][DIA][HORA]=LIVRE
		criarFicheiro(nomeFicheiro);  // cria ficheiro
		colocarDadosFicheiro(c,nomeFicheiro);
		system("cls");
		printf("\nPROGRAMA INSTALADO COM SUCESSO!\n\n");
		esperar();
	}
	else{
        lerDadosFicheiro(c,nomeFicheiro);
	}

	while(1){
		system("cls");  // Limpa tela do monitor
		nOpcao=menu();
		system("cls");

		if(nOpcao==0){
            puts("Programa terminado com sucesso!");
            puts("Obrigado.");
            printf("\n\n");
            creditos();
            exit(10);
        }

		if(nOpcao==1){
            do{
                imprimeSalas();
                putchar('\n');
                printf("1 MENU INICIAL --> 1.1 VER ESTADO DE UM [SALA] [DIA] [HORA]\n\n");
                verEstadoHora(c);
                printf("<1> para ver estado de outra sala. <2> para continuar: ");
                scanf(" %d",&n);
                system("cls");
            }
            while(n==1);
		}

		if(nOpcao==2){
            imprimeSalas();
            putchar('\n');
            printf("1 MENU INICIAL --> 1.2 VER SALAS LIVRES A UMA DADA HORA\n\n");
            verSalasLivresHora(c);
        }

		if(nOpcao==3){
            do{
				imprimeSalas();
				printf("1 MENU INICIAL --> 1.3 INSERIR UNIDADE CURRICULAR\n\n");
				putchar('\n');
                inserirUC(c);
                printf("<1> para adicionar nova unidade curricular. <2> para continuar: ");
                scanf(" %d",&n);
				system("cls");
			}
            while(n==1);
		}

		if(nOpcao==4){
            imprimeSalas();
            putchar('\n');
            printf("1 MENU INICIAL --> 1.4 VER HORARIO SEMANAL POR SALA\n\n");
            printf("Insira a sala: "); scanf(" %s",sala);
            nSala=(int) sala[1]-'0';
            verHorarioSemanalSala(c,nSala);
        }

		if(nOpcao==5){
			n=subMenu5defenicoes();
			if(n==1){
				system("cls");
				FILE *fp;
				char ch;
				if((fp=fopen("defenicoes.txt","r"))==NULL){
					puts("Erro ao abrir ficheiro 'defenicoes'.\n");
						continue;
				}
				while((ch=fgetc(fp))!=EOF)
					putchar(ch);
				fclose(fp);
				putchar('\n');
			}
			if(n==2){
				iniciarComplexoLivre(c);
				printf("PROGRAMA RESTAURADO COM SUCESSO.\n\n");
			}
		}

		colocarDadosFicheiro(c,nomeFicheiro); // guarda dados no ficheiro

		nOpcao=esperar();
        if(nOpcao==0){
			system("cls");			// limpa monitor
            puts("Programa terminado com sucesso!");
            puts("Obrigado\n");
            creditos();
            exit(10);
        }
	}
}


// DEFENICAO DAS FUNCOES UTILIZADAS
void iniciarComplexoLivre(complexo *c){    // Inicia o complexo com o valor LIVRE em todas as horas/dias/salas
	int i,j,k;
	for(i=0;i<NUM_SALAS;i++){
		for(j=0;j<NUM_DIAS;j++){
			for(k=0;k<NUM_HORAS;k++){
				c[i][j][k].estado=LIVRE;
				strcpy(c[i][j][k].UC,"LIVRE");
			}
		}
	}
}

void criarFicheiro(char* nomeFich){			// Criar ficheiro se ainda nao existir
    FILE *fp;
	if((fp=fopen(nomeFich,"wb"))==NULL){				// Se nao criar, dá erro nnumero 1 - exit(1)
        puts("Erro ao criar ficheiro.\n");
        exit(1);
    }
    fclose(fp);
}

void colocarDadosFicheiro(complexo *c, char *nomeFicheiro){  		// guarda os dados em modo binario no ficheiro salas.txt
    FILE *fp;
	int i,j,k;
	char ch;
    if((fp=fopen(nomeFicheiro,"wb"))==NULL){
        puts("Erro ao copiar dados para o ficheiro.\n");
        exit(2);
    }
    for(i=0;i<NUM_SALAS;i++){
        for(j=0;j<NUM_DIAS;j++){
            for(k=0;k<NUM_HORAS;k++){
                if(c[i][j][k].estado==LIVRE){
                    ch='L';
                    fwrite(&ch,1,1,fp);
                    fwrite(c[i][j][k].UC,NOME_UC,1,fp);
                }
                else{
                    ch='O';
                    fwrite(&ch,1,1,fp);
                    fwrite(c[i][j][k].UC,NOME_UC,1,fp);
                }
            }
        }
    }
    fclose(fp);
}

void lerDadosFicheiro(complexo *c, char *nomeFicheiro){				// lê os dados do ficheiro para o programa
	FILE *fp;
	char ch;
	int i,j,k;
	if((fp=fopen(nomeFicheiro,"rb"))==NULL){
        puts("Erro ao abrir dados do ficheiro.\n");
        exit(3);
    }
	for(i=0;i<NUM_SALAS;i++){
        for(j=0;j<NUM_DIAS;j++){
            for(k=0;k<NUM_HORAS;k++){
                fread(&ch,1,1,fp);
                if(ch=='L'){
                    fread(c[i][j][k].UC,NOME_UC,1,fp);
                    c[i][j][k].estado=LIVRE;
                }
                if(ch=='O'){
                    fread(c[i][j][k].UC,NOME_UC,1,fp);
                    c[i][j][k].estado=OCUPADA;
                }
            }
        }
    }
    fclose(fp);
}

void creditos(){
	printf("\t**********************************************\n");
	printf("\n");
	printf("\t             GESTAO DE SALAS \n");
	printf("\t                   NUM\n");
	printf("\t            COMPLEXO PEDAGOGICO\n");
	printf("\n");
	printf("\n");
	printf("\t Copyright c 2018 Todos os direitos reservado\n");
	printf("\t            realizado por : Bruno e Catarina\n");
	printf("\t                        universidade do minho\n");
	printf("\n");
	printf("\t***********************************************\n");
}

int menu(){					// mostra as opcoes do menu
    int n;
    puts("\t******************************** 1 MENU INICIAL **********************************");
    putchar('\n');
    imprimeSalas();
    printf("\n\n");
    printf("\t\t\t 1. Ver estado de uma sala\n");
    printf("\t\t\t 2. Ver salas livres a uma dada hora\n");
    printf("\t\t\t 3. Inserir Unidade Curricular\n");
    printf("\t\t\t 4. Ver mapa de horas por sala\n");
	printf("\t\t\t 5. Definicoes\n\n");
	printf("\t\t\t 0. Terminar o Programa\n\n");
    puts("\t******************************** 1 MENU INICIAL **********************************");
    putchar('\n');
    do{
        printf("Insira a opcao: ");
        scanf(" %d",&n);
    }
    while((n<1 || n>5) && n!=0);
    return n;
}

int subMenu5defenicoes(){
	int n;
    puts("\t******************************** 1.5 DEFENICOES **********************************");
    putchar('\n');
    printf("\n\n");
    printf("\t\t\t 1. como usar o programa\n");
    printf("\t\t\t 2. restaurar o programa - colocar todas as salas livres\n");
	 printf("\n\n");
    puts("\t******************************** 1.5 DEFENICOES **********************************");
    putchar('\n');
    do{
        printf("Insira a opcao: ");
        scanf(" %d",&n);
    }
    while((n<1 || n>2));
    return n;
}

int esperar(){
            char c='a';
			do{
				printf("Prima <M> para o Menu Inicial. Prima <0> para sair do programa: ");
                scanf(" %c",&c);
			}
            while(toupper(c)!='M' && c!='0');
            if(c=='0')
				return 0;
            else
                return 1;
}

void imprimeSalas(){		// imprime uma lista com as salas existentes no complexo
    puts("\t\t salas existentes no complexo: S0 S1 S2 S3 S4 S5 S6 S7 S8 S9");
}

void verHorarioSemanalSala(complexo *c,int nSala){			// mostra mapa semanal de uma sala
	int dia, hora;
    printf("\t\t\t\t\t\tSALA S%d\n\n",nSala);
    printf("       horas        SEGUNDA         TERCA           QUARTA          QUINTA          SEXTA\n\n");
    for(hora=0;hora<NUM_HORAS;hora++){
		printf("\t%2d ",8+hora);
		for(dia=0;dia<5;dia++){
			printf("\t%10s ",c[nSala][dia][hora].UC);
		}
		putchar('\n');
	}
	printf("\n\n");
}

void verSalasLivresHora(complexo *c){						// salas livres a uma dada hora e um dado dia da semana
	int hora,dia,i;
	printf("Insira (hora 8h-20h) (dia da Semana): ");
	scanf("%d %d",&hora,&dia);
	hora-=8;
	dia-=2;
	printf("Salas Livres: ");
	for(i=0;i<NUM_SALAS;i++){
		if(c[i][dia][hora].estado==LIVRE){
			printf("S%d ",i);
		}
	}
	printf("\n\n");
}

void verEstadoHora(complexo *c){
	int hora,nSala,diaSemana;
	char sala[2+1];
    printf("Insira: (sala) (Dia da semana) (hora 8h-20h): ");
    scanf("%s %d %d",sala,&diaSemana,&hora);
    nSala=sala[1]-'0';
	hora-=8;                                        // indices variam entre 0 e 12
	diaSemana-=2;
	if(c[nSala][diaSemana][hora].estado==OCUPADA)
		printf("OCUPADA: %s\n",c[nSala][diaSemana][hora].UC);
	else
		printf("LIVRE\n");
	putchar('\n');
}

void inserirUC(complexo *c){
	char sala[2+1];
	int nSala, diaSemana, hora;
	printf("Insira: (sala) (Dia da semana) (hora 8h-20h): ");
    scanf("%s %d %d",sala,&diaSemana,&hora);
    nSala=sala[1]-'0';
    hora-=8;    			// indices variam entre 0 e 12
	diaSemana-=2;
	if(c[nSala][diaSemana][hora].estado==OCUPADA){
        printf("Hora ja ocupada: %s\n",c[nSala][diaSemana][hora].UC);

	}
	else{
		printf("Insira a UC (UC-Curso): ");
        scanf("%s",c[nSala][diaSemana][hora].UC);
        c[nSala][diaSemana][hora].estado=OCUPADA;
        printf("UC inserida com sucesso.\n");
	}
	putchar('\n');
}
