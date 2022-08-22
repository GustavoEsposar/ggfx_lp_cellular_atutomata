//Bibliotecas proprias.

#include "JVIDA2-GGFX_Model.c"
#include "JVIDA2-GGFX_View.c"

//Funcoes.
void InicializarJogo(); //Inicia o jogo.
void DesabilitarMortos(); //Desabilitar Mortos.
void CriarMatriz(); //Cria a matriz do jogo.
void LimparMatriz(); //Limpa a matriz.
void AdicionarCelula(); //Adiciona uma nova celula na matriz.
void RemoverCelula(); //Remove uma celular da matriz.
void PegarCelulasVivasEMortas(); //Monta a matriz de celulas vivas e mortas.
void GameLoop(); //Loop principal do jogo;
void GerarProximaGeracao(); //Gera a proxima geracao.
void CarregarMatriz(); //Carrega as matriz salvas.
int SalvarGeracao(); //Salva a geracao atual.
void SalvarGeracoes(); //Salvar todas as geracoes.
void CarregarGeracoes(); //Carrega as geracoes.
void CarregaListaVivas(int ii, int jj); //Adiciona uma celula a lista vivas.
void CarregaListaMortas(int ii, int jj); //Adiciona uma celula a lista mortas.
void CarregaListaVivasAux(int ii, int jj); //Adiciona uma celula a lista vivasAux.
void CarregaListaGeracao(int j, int ii, int jj); //Adiciona uma celula a lista geracoes.

void InicializarJogo(){ //Inicia o jogo.
    char resposta = MenuGeral();

    switch(resposta){
        case '1': //Criar Matriz.
            CriarMatriz();
            break;
        case '2': //Carregar Matriz
            CarregarMatriz();
            break;
        case '3': //Mostrar Matriz.
            MostrarMatriz(1);
            break;
        case '4': //Adicionar Celula.
            AdicionarCelula();
            break;
        case '5': //Remover Celula.
            RemoverCelula();
            break;
        case '6': //Salvar Geracoes
            if(SalvarGeracao())
            	SalvarGeracoes();
            break;
        case '7': //Iniciar simulacao.
            PegarCelulasVivasEMortas();
            GameLoop();
            break;
        case '8': //Limpar Matriz.
            LimparMatriz();
            break;
        case '9': //Sair.
            return;
        default:
            break;
    }

    InicializarJogo(); //Chama novamente o menu.
}

void CriarMatriz(){ //Cria a matriz do jogo.
    tamanhoMatriz = EscolherTamanhoMatriz(); //Escolhe o tamanho da matriz.
    LimparMatriz(); //Limpa a matriz.
    MostrarMatriz(1);
}

void LimparMatriz(){ //Limpa a matriz.
    int x, y;
    for(x = 0; x < tamanhoMatriz; x++){
        for(y = 0; y < tamanhoMatriz; y++){
            matriz[x][y] = VAZIO;
        }
    }

    vivasCont = 0; //Reseta o numero de celulas vivas.
    mortasCont = 0; //Reseta o numero de celulas mortas.

    //MostrarMatriz(1);
}

void DesabilitarMortos(){ //Desabilitar Mortos.
    int i;

    mostrarCelulasMortas = 0;
    mostrarCoordenadasCelulas = 0;

    tipoCelula *aux;
    for(i = 0; i < mortasCont; i++){ //Deixa vazio as partes da geracao.
        if(i == 0) aux = mortas;
        matriz[aux->linha][aux->coluna] = VAZIO;
        aux = aux->next;
    }
    
    mortasCont = 0;
}

void AdicionarCelula(){ //Adiciona uma nova celula na matriz.
    int x, y;
    MostrarMatriz(0);
    do{
        do{ //Garante que a posicao esteja dentro da matriz.
            EscolherPosicaoMatriz(&x, &y);
        }while((x < -1 || x > (tamanhoMatriz - 1)) || (y < -1 || y > (tamanhoMatriz - 1)));

        if(x != -1 && y != -1){
            if(matriz[x][y] == VAZIO){
                CarregaListaVivas(x, y);
                matriz[x][y] = VIVO;
            }

            MostrarMatriz(0);
        }
    }while(x != -1 && y != -1);
}

void RemoverCelula(){ //Remove uma celular da matriz.
    int x, y;
    MostrarMatriz(0);
    do{
        do{ //Garante que a posicao esteja dentro da matriz.
            EscolherPosicaoMatriz(&x, &y);
        }while((x < -1 || x > (tamanhoMatriz - 1)) || (y < -1 || y > (tamanhoMatriz - 1)));

        if(x != -1 && y != -1){ //Caso nao tenha pedido para sair, remove a celular e tira uma de vivas.cont.
            if(matriz[x][y] == VIVO){ //Apenas tira de vivas.cont caso a celula estivesse viva.
                vivasCont--;
                matriz[x][y] = VAZIO;
            }

            MostrarMatriz(0);
        }
    }while(x != -1 && y != -1);
}

void PegarCelulasVivasEMortas(){ //Monta a matriz de celulas vivas e mortas.
    int i, x, y;
    vivasCont = 0;
    for(x = 0; x < tamanhoMatriz; x++){ //Roda pela matriz e pega as celulas vivas.
        for(y = 0; y < tamanhoMatriz; y++){
            if(matriz[x][y] == VIVO){
                CarregaListaVivas(x, y);
            }
        }
    }

    tipoCelula *aux;
    for(i = 0; i < mortasCont; i++){ //Deixa vazio as partes da geracao.
        if(i == 0) aux = mortas;
        matriz[aux->linha][aux->coluna] = VAZIO;
        aux = aux->next;
    }

    mortasCont = 0;
    for(i = 0; i < vivasCont; i++){ //Roda pelas celulas vivas.
        if(i == 0) aux = vivas;

        for(x = -1; x <= 1; x++){ //Checa todos seus lados.
            for(y = -1; y <= 1; y++){
                if(x == 0 && y == 0) continue; //Caso nao seja a propria celula.
                if(aux->linha + x < 0 || aux->linha + x >= tamanhoMatriz) continue; //Caso a posicao nao esteja fora da matriz.
                if(aux->coluna + y < 0 || aux->coluna + y >= tamanhoMatriz) continue; //Caso a posicao nao esteja fora da matriz.
                
                char posAux = matriz[aux->linha + x][aux->coluna + y];
                if(posAux == VAZIO){ //Caso esteja vazio, adicionar a lista de mortos.
                    CarregaListaMortas(aux->linha + x, aux->coluna + y);
                    matriz[aux->linha + x][aux->coluna + y] = MORTO;
                }
            }
        }

        aux = aux->next;
    }
}

void GameLoop(){ //Loop principal do jogo. (Em desenvolvimento)
    int vezes = VezesSimulacao();
    if(vezes == 0){
        return;
    }

    int segundos = SegundosSimulacao();
    int porEnter = (segundos == 0);

    IniciarSimulacao(); //Iniciar simulacao.
    MostrarMatriz(porEnter);
    sleep(segundos);
    while(vezes > 0){
        GerarProximaGeracao();
        MostrarMatriz(porEnter);
        sleep(segundos);
        vezes--;
        if(vivasCont == 0 && mortasCont == 0)
            vezes = 0;
    }
    DesabilitarMortos();
}

void GerarProximaGeracao(){
    int i, x, y;
    tipoCelula *aux;

    int vizinhosQuant = 0;
    vivasAuxCont = 0;

    for(i = 0; i < vivasCont; i++){
        if(i == 0) aux = vivas;

        vizinhosQuant = 0;
        for(x = -1; x <= 1; x++){ //Checa todos seus lados.
            for(y = -1; y <= 1; y++){
                if(x == 0 && y == 0) continue; //Caso nao seja a propria celula.
                if(aux->linha + x < 0 || aux->linha + x >= tamanhoMatriz) continue; //Caso a posicao nao esteja fora da matriz.
                if(aux->coluna + y < 0 || aux->coluna + y >= tamanhoMatriz) continue; //Caso a posicao nao esteja fora da matriz.

                char posAux = matriz[aux->linha + x][aux->coluna + y];
                if(posAux == VIVO){ //Checa se eh uma celula viva, e aumenta o contador do auxiliar.
                    vizinhosQuant++;
                }
            }
        }

        if(vizinhosQuant > 1 && vizinhosQuant <= 3){ //Caso seja 2 ou 3 vizinhos, adiciona a lista auxiliar.
            CarregaListaVivasAux(aux->linha, aux->coluna);
        }

        aux = aux->next;
    }

    for(i = 0; i < mortasCont; i++){
        if(i == 0) aux = mortas;

        vizinhosQuant = 0;
        for(x = -1; x <= 1; x++){ //Checa todos seus lados.
            for(y = -1; y <= 1; y++){
                if(x == 0 && y == 0) continue; //Caso nao seja a propria celula.
                if(aux->linha + x < 0 || aux->linha + x >= tamanhoMatriz) continue; //Caso a posicao nao esteja fora da matriz.
                if(aux->coluna + y < 0 || aux->coluna + y >= tamanhoMatriz) continue; //Caso a posicao nao esteja fora da matriz.

                char posAux = matriz[aux->linha + x][aux->coluna + y];
                if(posAux == VIVO){ //Checa se eh uma celula viva, e aumenta o contador do auxiliar.
                    vizinhosQuant++;
                }
            }
        }
        if(vizinhosQuant == 3){ //Caso seja 2 ou 3 vizinhos, adiciona a lista auxiliar.
            CarregaListaVivasAux(aux->linha, aux->coluna);
        }

        aux = aux->next;
    }

    for(i = 0; i < vivasCont; i++){ //Deixa vazio as partes da geracao passada.
        if(i == 0) aux = vivas;

        matriz[aux->linha][aux->coluna] = VAZIO;

        aux = aux->next;
    }

    for(i = 0; i < mortasCont; i++){ //Deixa vazio as partes da geracao passada.
        if(i == 0) aux = mortas;

        matriz[aux->linha][aux->coluna] = VAZIO;

        aux = aux->next;
    }

    for(i = 0; i < vivasAuxCont; i++){ //Preenche os vivos atuais.
        if(i == 0) aux = vivasAux;

        matriz[aux->linha][aux->coluna] = VIVO;

        aux = aux->next;
    }

    mortasCont = 0;
    for(i = 0; i < vivasAuxCont; i++){ //Roda pelas celulas vivas.
        if(i == 0) aux = vivasAux;

        for(x = -1; x <= 1; x++){ //Checa todos seus lados.
            for(y = -1; y <= 1; y++){
                if(x == 0 && y == 0) continue; //Caso nao seja a propria celula.
                if(aux->linha + x < 0 || aux->linha + x >= tamanhoMatriz) continue; //Caso a posicao nao esteja fora da matriz.
                if(aux->coluna + y < 0 || aux->coluna + y >= tamanhoMatriz) continue; //Caso a posicao nao esteja fora da matriz.
                
                char posAux = matriz[aux->linha + x][aux->coluna + y];
                if(posAux == VAZIO){ //Caso esteja vazio, adicionar a lista de mortos.
                    CarregaListaMortas(aux->linha + x, aux->coluna + y);
                    matriz[aux->linha + x][aux->coluna + y] = MORTO;
                }
            }
        }

        aux = aux->next;
    }

    vivasCont = vivasAuxCont;
    vivas = vivasAux;
}

void CarregarMatriz(){
    if(geracoesSalvasCont == 0){
        printf("\nNao existem geracoes salvas.\n");
        printf("Pressione ENTER para continuar.");
        getchar();
        return;
    }

    int i;
    int selecao = 0;

    tipoCelula *aux;

    while(selecao + 1 != 0){ //Roda enquanto a selecao for diferente de 0.

    	if(selecao < geracoesSalvasCont){ //Caso a escolha seja valida, mostra a matriz.
	        tamanhoMatriz = geracoesSalvas[selecao].tamanhoMatriz;
	        LimparMatriz();
	        vivasCont = geracoesSalvas[selecao].geracaoCont;
	        vivas = geracoesSalvas[selecao].geracao;

            for(i = 0; i < vivasCont; i++){ //Deixa vazio as partes da geracao passada.
                if(i == 0) aux = vivas;
                matriz[aux->linha][aux->coluna] = VIVO;
                printf("%d %d\n", aux->linha, aux->coluna);
                aux = aux->next;
            }

	        MostrarMatriz(0);
	    }
        selecao = EscolhaMatrizCarregada() - 1;
    }
}

int inicial = 0;
int SalvarGeracao(){
    int i;
    int salvar = SalvarMatrizComo() - 1; //Escolhe onde o usuario quer salvar a geracao.
    
    if(salvar == -1) return 0; //Caso queria voltar retorna.
	
    geracoesSalvas[salvar].tamanhoMatriz = tamanhoMatriz; //Salva a lista de vivas atual.
    geracoesSalvas[salvar].geracaoCont = vivasCont;
    geracoesSalvas[salvar].geracao = vivas;

	if(salvar == geracoesSalvasCont) //Caso seja uma nova aumenta a quantidade.
    	geracoesSalvasCont++;
    
    return 1;
}

void SalvarGeracoes(){
    int i, j;
	FILE *fp;
	
	if((fp = fopen("ARQGERACOES", "w")) == NULL){ //Cria o arquivo ARQPECAS
		printf("\nHouve um erro para salvar o arquivo. (ARQGERACOES)\n");
		return;
	}

    if(fwrite(&geracoesSalvasCont, sizeof(geracoesSalvasCont), 1, fp) != 1){
        printf("\nErro CONT\n");
        return;
    }

    tipoCelula *aux;
    for(i = 0; i < geracoesSalvasCont; i++){ //Adiciona ao ARQGERACOES o tamanho da matriz.
        if(fwrite(&geracoesSalvas[i].tamanhoMatriz, sizeof(geracoesSalvas[i].tamanhoMatriz), 1, fp) != 1){
            printf("\nErro TAMANHO MATRIZ\n");
            return;
        }

        if(fwrite(&geracoesSalvas[i].geracaoCont, sizeof(geracoesSalvas[i].geracaoCont), 1, fp) != 1){
            printf("\nErro CONT\n");
            return;
        }

        for(j = 0; j < geracoesSalvas[i].geracaoCont; j++){ //Adiciona ao ARQGERACOES a lista de vivas.
            if(j == 0) aux = geracoesSalvas[i].geracao;

            if(fwrite(&(aux->linha), sizeof(int), 1, fp) != 1){
                printf("\nErro GERACOES\n");
                return;
            }

            if(fwrite(&(aux->coluna), sizeof(int), 1, fp) != 1){
                printf("\nErro GERACOES\n");
                return;
            }

            aux = aux->next;
        }
	}

    printf("\nSalva com sucesso!\n");

    fclose(fp);
}

void CarregarGeracoes(){
    int i, j;
	FILE *fp;
	
	if((fp = fopen("ARQGERACOES", "r")) == NULL){ //Cria o arquivo ARQPECAS
		return;
	}

    if(fread(&geracoesSalvasCont, sizeof(geracoesSalvasCont), 1, fp) != 1){
        printf("\nErro CONT\n");
        return;
    }

    tipoCelula *aux;
    for(i = 0; i < geracoesSalvasCont; i++){ //Adiciona ao ARQGERACOES o tamanho da matriz.
        if(fread(&geracoesSalvas[i].tamanhoMatriz, sizeof(geracoesSalvas[i].tamanhoMatriz), 1, fp) != 1){
            printf("\nErro TAMANHO MATRIZ\n");
            return;
        }

        if(fread(&geracoesSalvas[i].geracaoCont, sizeof(geracoesSalvas[i].geracaoCont), 1, fp) != 1){
            printf("\nErro CONT\n");
            return;
        }

        for(j = 0; j < geracoesSalvas[i].geracaoCont; j++){ //Adiciona ao ARQGERACOES a lista de vivas.
            int a, b;
            if(fread(&a, sizeof(int), 1, fp) != 1){
                printf("\nErro GERACOES\n");
                return;
            }

            if(fread(&b, sizeof(int), 1, fp) != 1){
                printf("\nErro GERACOES\n");
                return;
            }
            
            CarregaListaGeracao(i, a, b);
        }
        inicial = 0;
	}

    fclose(fp);
}

void CarregaListaVivas(int ii, int jj)
{
    tipoCelula *aux = malloc(sizeof(tipoCelula)); //alocação dinâmica
    if (aux == NULL)
    {
    printf("Sem espaço na memoria para inclusao de celula viva\n");
    return;
    }

    aux->linha = ii;
    aux->coluna = jj;
    //sempre inclui no início da lista
    if(vivasCont == 0) //se a lista está vazia
    {
        vivas = aux;
        vivas->next = NULL;
    }
    else //lista não vazia
    {
        aux->next = vivas;
        vivas = aux; //o inicio da lista passa a ser a nova celula
    }
    vivasCont++;
}

void CarregaListaMortas(int ii, int jj)
{
    tipoCelula *aux = malloc(sizeof(tipoCelula)); //alocação dinâmica
    if (aux == NULL)
    {
    printf("Sem espaço na memoria para inclusao de celula viva\n");
    return;
    }

    aux->linha = ii;
    aux->coluna = jj;

    //sempre inclui no início da lista
    if(mortasCont == 0) //se a lista está vazia
    {
        mortas = aux;
        mortas->next = NULL;
    }
    else //lista não vazia
    {
        aux->next = mortas;
        mortas = aux; //o inicio da lista passa a ser a nova celula
    }
    mortasCont++;
}

void CarregaListaVivasAux(int ii, int jj)
{
    tipoCelula *aux = malloc(sizeof(tipoCelula)); //alocação dinâmica
    if (aux == NULL)
    {
    printf("Sem espaço na memoria para inclusao de celula viva\n");
    return;
    }

    aux->linha = ii;
    aux->coluna = jj;
    //sempre inclui no início da lista
    if(vivasAuxCont == 0) //se a lista está vazia
    {
        vivasAux = aux;
        vivasAux->next = NULL;
    }
    else //lista não vazia
    {
        aux->next = vivasAux;
        vivasAux = aux; //o inicio da lista passa a ser a nova celula
    }
    vivasAuxCont++;
}

void CarregaListaGeracao(int j, int ii, int jj)
{
    tipoCelula *aux = malloc(sizeof(tipoCelula));
    if (aux == NULL)
    {
        printf("Sem espaco inclusao de celula viva\n");
        return;
    }

    aux->linha = ii;
    aux->coluna = jj;
    
    if(inicial == 0){
        geracoesSalvas[j].geracao = aux;
        geracoesSalvas[j].geracao->next = NULL;
        inicial = 1;
    }else{
        aux->next = geracoesSalvas[j].geracao;
        geracoesSalvas[j].geracao = aux;
    }
}



//TESTES

/*void GameLoop(){ //Loop principal do jogo. (Em desenvolvimento)
    IniciarSimulacao(); //Iniciar simulacao.
    int vezes = 1;
    do{
        int vezes = PassarGeracao() - 48;

        printf("Vezes: %d", vezes);

        if(vezes > 0){
            int segundos = 0;
            if(vezes > 1){
                segundos = QuantSegundos();
                while(vezes > 1){
                    GerarProximaGeracao();
                    MostrarMatriz(0);
                    sleep(segundos);
                    vezes--;
                }
            }else{
                GerarProximaGeracao();
                MostrarMatriz(0);
            }
        }
    }while(vezes > 0);

    InicializarJogo();
}*/
