//Modelo (Estruturas, Variaveis Globais, etc..).

#define VAZIO '.'
#define VIVO 'O'
#define MORTO '*'

typedef struct celula{
    int linha, coluna;
	struct celula *next;
} tipoCelula;

int tamanhoMatriz = 1; //Tamanho da matriz do jogo.

int vivasCont = 0;
tipoCelula *vivas; //Lista de celulas vivas.

int mortasCont = 0;
tipoCelula *mortas; //Lista de celulas mortas.

int vivasAuxCont = 0;
tipoCelula *vivasAux; //Lista auxiliar de celulas vivas.

char matriz[100][100]; //Vetor da matriz do jogo.

int mostrarCelulasMortas = 0; //Variavel auxiliar para selecao do jogador caso queira mostras as celulas mortas.
int mostrarCoordenadasCelulas = 0; //Variavel auxiliar para selecao do jogador caso queira mostras as coordenadas das celulas.

typedef struct salvarLis{
	int tamanhoMatriz;
	int geracaoCont;
	tipoCelula *geracao;
} tipoSalvar;

int geracoesSalvasCont = 0; //Quantidade de geracoes salvas.
tipoSalvar geracoesSalvas[100]; //Lista de geracoes salvas.
