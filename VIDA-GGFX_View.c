//View - Interfaces.

//Bibliotecas padrao.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


//Declaracoes funcoes.
void LimparBuffer(); //Limpa o buffer do teclado.

char MenuGeral(){ //Menu geral do jogo;
    printf("\n   _____    _____   ______  __   __\n"
            "  / ____|  / ____| |  ____| \\ \\ / /\n"
            " | |  __  | |  __  | |__     \\ V / \n"
            " | | |_ | | | |_ | |  __|     > <  \n"
            " | |__| | | |__| | | |       / . \\ \n"
            "  \\_____|  \\_____| |_|      /_/ \\_\\\n"
            "                                   \n");

    char resposta;

    //1 - Criar Matriz.
    //2 - Carregar Matriz.
    //3 - Mostrar Matriz.
    //4 - Adicionar Celula.
    //5 - Remover Celula.
    //6 - Salvar Geracao.
    //7 - Iniciar simulacao.
    //8 - Limpar Matriz.
    //9 - Sair.
    printf("1 - Criar Matriz\n2 - Carregar Matriz (%d Carregadas)\n3 - Mostrar Matriz\n4 - Adicionar Celula\n5 - Remover Celula\n6 - Salvar Matriz\n7 - Iniciar Simulacao\n8 - Limpar Matriz\n9 - Sair\n-> ", geracoesSalvasCont);

    scanf(" %c", &resposta);
    LimparBuffer();

    return resposta;
}

int EscolherTamanhoMatriz(){ //Escolher o tamanho da matriz.
    int resposta;

    printf("Tamanho da matriz (Entre 1 e 100).\n-> ");

    do{ //Garante que o tamanho fica entre 1 e 100;
        scanf("%d", &resposta);
        LimparBuffer();
    }while(resposta < 1 || resposta > 100);

    return resposta;
}

void MostrarMatriz(int enterParaContinuar){ //Mostra a matriz.
    int x, y;

    printf("   ");
    for(x = 0; x < tamanhoMatriz; x++)
        printf("%02d ", x);

    for(x = 0; x < tamanhoMatriz; x++){
        printf("\n%02d", x);
        for(y = 0; y < tamanhoMatriz; y++){
                            //Caso seja uma celula morta e nao queira mostrar celulas mortas printa VAZIO, caso queira printa normal.
            printf("  %c", (matriz[x][y] == MORTO && !mostrarCelulasMortas) ? VAZIO : matriz[x][y]);
        }
    }

    tipoCelula *aux;

    printf("\nVIVOS (%d)", vivasCont);
    if(mostrarCoordenadasCelulas){
    	printf(": ");
	    for(x = 0; x < vivasCont; x++){
            if(x == 0) aux = vivas;
	        printf("[%d;%d] ", aux->linha, aux->coluna);
            aux = aux->next;
        }
	}

    printf("\nMORTOS (%d)", mortasCont);
    
    if(mostrarCoordenadasCelulas){
    	printf(": ");
	    for(x = 0; x < mortasCont; x++){
            if(x == 0) aux = mortas;
	        printf("[%d;%d] ", aux->linha, aux->coluna);
            aux = aux->next;
        }
	}

    printf("\n");

    if(enterParaContinuar){
        printf("\nAperte ENTER para continuar.");
        getchar();
    }
}

void EscolherPosicaoMatriz(int *x, int *y){ //Escolher uma posicao da matriz.
    printf("\nCoordenada da celula: (Exemplo: 1;2)(-1 para sair).\n");
    *x = 0; //Reseta as coordenadas.
    *y = 0; //Reseta as coordenadas.

    char aux; //Caracter auxilixar para nao ter caracter especifico entre as coordenadas.
    scanf("%d", x);
    if(*x != -1){ //Caso tenha escolhido sair (-1).
        scanf("%c", &aux);
        scanf("%d", y);
        
        if((*x < 0 || *x > (tamanhoMatriz - 1)) || (*y < 0 || *y > (tamanhoMatriz - 1))){ //Checa se a posicao escolhida esta fora da matriz.
        	printf("Posicao fora da matriz, tente novamente!");
		}
    }

    LimparBuffer();
}

int VezesSimulacao(){
    int selecao;

    do{
        printf("\nQuantas vezes voce quer rodar a simulacao. (0 para voltar)\n-> ");
        scanf("%d", &selecao);
    }while(selecao < 0);

    return selecao;
}

int SegundosSimulacao(){
    int selecao;

    do{
        printf("\nDe quantos em quantos segundos voce quer rodar a simulacao. (0 para voce comandar)\n-> ");
        scanf("%d", &selecao);
    }while(selecao < 0);

    return selecao;
}

void IniciarSimulacao(){
    int i;
    char selecao;

    do{ //Roda enquanto o jogador nao escolher s ou n.
        printf("\nVoce quer destacar as celulas mortas com (*)? (s/n)\n-> ");
        scanf(" %c", &selecao);
        selecao = tolower(selecao);

        LimparBuffer();
    }while(selecao != 's' && selecao != 'n');

    switch(selecao){
        case 's':
            mostrarCelulasMortas = 1;
            break;
        case 'n':
            mostrarCelulasMortas = 0;
            break;
    }
    
    do{ //Roda enquanto o jogador nao escolher s ou n.
        printf("\nVoce quer mostrar as coordenadas das celulas? (Exemplo: [5;2])? (s/n)\n-> ");
        scanf(" %c", &selecao);
        selecao = tolower(selecao);

        LimparBuffer();
    }while(selecao != 's' && selecao != 'n');

    switch(selecao){
        case 's':
            mostrarCoordenadasCelulas = 1;
            break;
        case 'n':
            mostrarCoordenadasCelulas = 0;
            break;
    }
}

int EscolhaMatrizCarregada(){ //Mostra a matriz.
    int selecao;
    int i;

    do{
        printf("Qual matriz voce quer ver? (Possibilidades: ");
        for(i = 0; i < geracoesSalvasCont; i++)
            printf("%d ", i + 1);
        printf(") (0 para selecionar a atual)\n-> ");

        scanf("%d", &selecao);

        LimparBuffer();
    }while(selecao < 0);

    return selecao;
}

int SalvarMatrizComo(){ //Escolhe onde quer salvar a matriz.
    int selecao;
    int i;

    do{
        printf("Voce quer salvar essa matriz como:\n");
        for(i = 0; i < geracoesSalvasCont; i++)
            printf("- %d (Sobrescrever)\n", i + 1);
            
        printf("- %d (Novo)\n", geracoesSalvasCont + 1);
        printf("- 0 (Sair)\n-> ");

        scanf("%d", &selecao);

        LimparBuffer();
    }while(selecao < 0 || selecao > (geracoesSalvasCont + 1));

    return selecao;
}


void LimparBuffer(){ //Limpar buffer do teclado.
    char c;
    while ((c = fgetc(stdin)) != EOF && c != '\n'){}
}
