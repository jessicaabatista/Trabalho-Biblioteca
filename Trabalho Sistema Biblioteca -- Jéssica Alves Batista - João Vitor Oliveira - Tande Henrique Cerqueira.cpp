/*****************************************************************************
     Estrutura de Dados e Algoritmos
	 Trabalho: Sistema de biblioteca
	 Discentes: Jéssica Alves Batista; João Vitor Oliveira Mendes; Tande Henrique Cerqueira
	 Prof.: Danilo Alves Martins de Faria
******************************************************************************/

# include <stdio.h>
//# include <conio.h>
# include <stdlib.h>
# include <math.h>

typedef struct livros {  // Cada NO da lista tem dois campos: uma informação e um ponteiro para o proximo elemento.
	int codigo; 
	char titulo[30];
	char categoria[30];
	char autor[30];
	int qtd_total;
	int qtd_disp;     
} Tlivros;
	
typedef struct listaLivros *pTlistaLivros; //Definicao de um tipo que é um ponteiro.
	
typedef struct listaLivros { 
	pTlistaLivros Dir;
	Tlivros Info;
	pTlistaLivros Esq;
    pTlistaLivros Pai;
    int Balanco;   // ponteiro que guarda o endereco do proximo NO da lista, na memoria.
} TlistaLivros;  //este tipo Tlista e usado no malloc.

typedef struct emprestimo {  // Cada NO da lista tem dois campos: uma informação e um ponteiro para o proximo elemento.
	int cod_livro;
	pTlistaLivros ponteiro_L; 
	char data_emp [10];
} Temprestimo;
	
typedef struct listaEmprestados *pTlistaEmprestados; //Definicao de um tipo que e um ponteiro.

typedef struct listaEmprestados { 
	pTlistaEmprestados DirE;
	Temprestimo InfoE;
	pTlistaEmprestados  EsqE;
    pTlistaEmprestados PaiE;
    int BalancoE;   // ponteiro que guardar o endereco do proximo NO da lista, na memoria.
} TlistaEmprestados;

typedef struct usuarios {  
	int matricula; 
	char nome[30];
	char telefone[11];
	int qtd_emp; // ponteiro que guarda o endereco do proximo NO da lista, na memoria.
	pTlistaEmprestados ponteiro_emprestados;
} Tusuarios;
	
typedef struct listaUsuarios *pTlistaUsuarios; //Definicao de um tipo que e um ponteiro.

typedef struct listaUsuarios { 
	pTlistaUsuarios DirU;
	Tusuarios InfoU;
	pTlistaUsuarios  EsqU;
    pTlistaUsuarios PaiU;
    int BalancoU;   // ponteiro que guardar o endereco do proximo NO da lista, na memoria.
} TlistaUsuarios;


int mygeti(int *result){ //trata a exceção caso seja digitado um não integer em um campo proprio para integer, fazendo o pedido se repetir
    char buff [ 30 ]; 
    return fgets(buff, sizeof buff, stdin) && sscanf(buff, "%d", result) == 1;
}

#define gotoxy(linha,col)               printf( "\033[%d;%dH", (linha), (col) )

pTlistaLivros localizara_para_remover_Livros(pTlistaLivros L,  int x);
pTlistaUsuarios localizara_para_remover_Usuarios(pTlistaUsuarios L,  int x);
pTlistaEmprestados localizara_para_remover_Emprestados(pTlistaEmprestados E, int x);
void InserirEmprestados(pTlistaLivros L, pTlistaUsuarios U);
pTlistaUsuarios acha_usuarios_para_emprestimo(pTlistaUsuarios U, pTlistaLivros L, int x, int y);
pTlistaLivros acha_livros_para_emprestimo(pTlistaLivros L, pTlistaUsuarios U, int y);
pTlistaUsuarios acha_usuarios_para_devolucao(pTlistaUsuarios U, pTlistaLivros L, int x, int y);
pTlistaLivros acha_livros_para_devolucao(pTlistaLivros L, pTlistaUsuarios U, int y);
pTlistaLivros acha_para_imprimirLivros(pTlistaLivros L, int x);
pTlistaLivros acha_livros_para_adicionar(pTlistaLivros L, int x, int adicionar);
void libera_memoriaE(pTlistaEmprestados E);

////////////////////////////////////////////////////////////////////////////
///////////////////////IMPRIMIR LIVRO/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void imprimirLivro(pTlistaLivros L){
    printf("\n-> Informacoes do livro: \n");
	printf("\nCodigo: %d",L->Info.codigo);
	printf("\nTitulo: %s",L->Info.titulo);
	printf("\nAutor: %s",L->Info.autor);
	printf("\nCategoria: %s",L->Info.categoria);
	printf("\nQuantidade total de volumes: %d",L->Info.qtd_total);
	printf("\nQuantidade total de volumes disponiveis: %d",L->Info.qtd_disp);
	//getchar();
	//getchar();
}

void chama_acha_para_imprimirLivros(pTlistaLivros L){
    int x, test_int;
    
	printf("-------------------------------------------");
	printf("\n# Consulta de livro#\n");
	printf("\nDigite o codigo do livro que deseja consultar: ");
	while ( !mygeti(&test_int) ){
    	fputs("Digite o codigo do livro que deseja consultar: ", stdout);
    	fflush(stdout);
    } 
    
    x = test_int; 
    
    acha_para_imprimirLivros(L, x);
}

pTlistaLivros acha_para_imprimirLivros(pTlistaLivros L, int x) {
    if (L) {    //Verificar se a árvore não está vazia.
        if (L->Info.codigo == x)     //1 - Verificar se o elemento x está na raiz.
            imprimirLivro(L);  
        else {            
            if (x < L->Info.codigo) 
                acha_para_imprimirLivros(L->Esq, x);
            else 
                acha_para_imprimirLivros(L->Dir, x);
        }        
    }
    else 
        printf("Esse livro nao foi encontrado na lista.\n\n\n");
    
    //getchar();
    //getchar();
    return L;
}

////////////////////////////////////////////////////////////////////////////
///////////////////////IMPRIMIR USUARIOS/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void imprime_recursivo(pTlistaEmprestados E){
    printf("\n- Livros emprestados -\n");
    
    if (E != NULL){
        imprime_recursivo(E->EsqE);
        printf("Livro: %d ",E->InfoE.cod_livro);
        printf("Data do emprestimo: %s ",E->InfoE.data_emp);
        imprime_recursivo(E->DirE);
    }else{
        printf("Este usuario nao possui livros emprestados no momento.\n\n\n");
        //getchar();
        //getchar();
    }
}

void imprimirUsuarios(pTlistaUsuarios U){
    pTlistaEmprestados E = U->InfoU.ponteiro_emprestados;
    
    printf("\n-> Informacoes do usuario: \n");
	printf("\nMatricula: %d", U->InfoU.matricula);
	printf("\nNome: %s",U->InfoU.nome);
	printf("\nTelefone: %s",U->InfoU.telefone);
	printf("\nLivros emprestados: %d",U->InfoU.qtd_emp);
	printf("\nLivros emprestados: ");
	
	imprime_recursivo(E);
	//getchar();
	//getchar();
}

pTlistaUsuarios acha_para_imprimirUsuarios(pTlistaUsuarios U) {
    int x, test_int;
	
	printf("-------------------------------------------");
	printf("\n# Consulta de usuario#\n");
	//printf("\nDigite o codigo do livro que deseja consultar: ");
	while ( !mygeti(&test_int) ){
    	fputs("Digite a matricula do usuario que deseja consultar: ", stdout);
    	fflush(stdout);
    } 
    x = test_int; 
    
    if (U) {    //Verificar se a árvore não está vazia.
        if (U->InfoU.matricula == x)     //1 - Verificar se o elemento x está na raiz.
            imprimirUsuarios(U);  
        else {            
            if (x < U->InfoU.matricula) 
                U->EsqU = acha_para_imprimirUsuarios(U->EsqU);
            else 
                U->DirU = acha_para_imprimirUsuarios(U->DirU);
        }        
    }
    else 
        printf("Esse usuario nao foi encontrado na lista.\n\n\n");
    
    //getchar();
    //getchar();
    return U;
}

////////////////////////////////////////////////////////////////////////////
///////////////////////BALANÇO USUARIO/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


int pega_lado_Usuarios(pTlistaUsuarios NoU){
	if (NoU->PaiU){
		if (NoU == NoU->PaiU->EsqU)
			return -1;
		else
			return 1;
	}
	else
		return 0;
}

int pega_altura_Usuarios(pTlistaUsuarios U) {
    int esq, dir;
   
    esq = 0;
    dir = 0;
    
    if ( U ) {   
        esq = pega_altura_Usuarios(U->EsqU);
        dir = pega_altura_Usuarios(U->DirU);
   
        if ( esq > dir )
            return esq + 1;
        else
            return dir + 1;
    }
    else
        return -1;
}

int retorna_balanco_Usuarios(pTlistaUsuarios U){
    return pega_altura_Usuarios(U->DirU) - pega_altura_Usuarios(U->EsqU);
}


void atualiza_balanco_Usuarios(pTlistaUsuarios PaiU, int ladoU){
	pTlistaUsuarios PaiAux;

	PaiU->BalancoU += ladoU;  //atualizo o pai	

	if (PaiU->BalancoU){	//se o pai ficou desbalanceado, confere os ancestrais
        PaiAux = PaiU;

		while (PaiAux->PaiU && (PaiAux->BalancoU != ladoU * 2)){ 
			ladoU = pega_lado_Usuarios(PaiAux);
			PaiAux = PaiAux->PaiU;
			PaiAux->BalancoU += ladoU;
		};
    }
}

void atualiza_pai_Usuarios (pTlistaUsuarios U, pTlistaUsuarios FilhoU){
	int lado;

    lado = pega_lado_Usuarios(U);
    if (lado == -1)
	    U->PaiU->EsqU = FilhoU;
    else
	    U->PaiU->DirU = FilhoU;

    if (FilhoU)
        FilhoU->PaiU = U->PaiU;
}

pTlistaUsuarios pega_maior_valor_Usuarios(pTlistaUsuarios U){
	while (U->DirU)
		U = U->DirU;
	return U;
}

pTlistaUsuarios rotacao_direita_Usuarios(pTlistaUsuarios NoU){
	pTlistaUsuarios Aux;

	Aux = NoU->EsqU;
	NoU->EsqU = Aux->DirU;
	Aux->PaiU = NoU->PaiU;
	Aux->DirU = NoU;
	if (NoU->EsqU)
		NoU->EsqU->PaiU = NoU;
	if (NoU->PaiU){
		if (NoU == NoU->PaiU->EsqU)
			NoU->PaiU->EsqU = Aux;   //No eh filho a esquerda
		else
			NoU->PaiU->DirU = Aux;   //No eh filho a direita
	}
	
	NoU->PaiU = Aux;
	NoU->BalancoU = retorna_balanco_Usuarios(NoU);
	Aux->BalancoU = retorna_balanco_Usuarios(Aux);
    return Aux;
}

pTlistaUsuarios rotacao_esquerda_Usuarios(pTlistaUsuarios NoU){
	pTlistaUsuarios Aux;

	Aux = NoU->DirU;
	NoU->DirU = Aux->EsqU;
	Aux->PaiU = NoU->PaiU;
	Aux->EsqU = NoU;
	if (NoU->DirU)
		NoU->DirU->PaiU = NoU;

	if (NoU->PaiU){
		if (NoU == NoU->PaiU->EsqU)
			NoU->PaiU->EsqU = Aux;   //No eh filho a esquerda
		else
			NoU->PaiU->DirU = Aux;   //No eh filho a direita
	}

	NoU->PaiU = Aux;
	NoU->BalancoU = retorna_balanco_Usuarios(NoU);
	Aux->BalancoU = retorna_balanco_Usuarios(Aux);
    return Aux;
}


//Função que verifica se um nó está balanceado e faz as rotações necessárias.
pTlistaUsuarios corrige_balanceamento_Usuarios(pTlistaUsuarios U) {

    U->BalancoU = retorna_balanco_Usuarios(U);    //Após inserir o nó para esq ou dir, verificar Fator de bal.

    if (U->BalancoU < -1) {
        if (retorna_balanco_Usuarios(U->EsqU) > 0)
            U->EsqU = rotacao_esquerda_Usuarios(U->EsqU);
        U = rotacao_direita_Usuarios(U);
    }
    else if (U->BalancoU > 1) {
        if (retorna_balanco_Usuarios(U->DirU) < 0)
            U->DirU = rotacao_direita_Usuarios(U->DirU);
        U = rotacao_esquerda_Usuarios(U);
    }
    return U;
}

////////////////////////////////////////////////////////////////////////////
///////////////////////BALANÇO LIVROS//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

int pega_lado_Livros(pTlistaLivros No){
	if (No->Pai){
		if (No == No->Pai->Esq)
			return -1;
		else
			return 1;
	}
	else
		return 0;
}

int pega_altura_Livros(pTlistaLivros L) {
    int esq, dir;
   
    esq = 0;
    dir = 0;
    if ( L ) {   
        esq = pega_altura_Livros(L->Esq);
        dir = pega_altura_Livros(L->Dir);
   
        if ( esq > dir )
            return esq + 1;
        else
            return dir + 1;
    }
    else
        return -1;
}

int retorna_balanco_Livros(pTlistaLivros L){
    return pega_altura_Livros(L->Dir) - pega_altura_Livros(L->Esq);
}


void atualiza_balanco_Livros(pTlistaLivros Pai, int lado){
	pTlistaLivros PaiAux;

	Pai->Balanco += lado;  //atualizo o pai	

	if (Pai->Balanco){	//se o pai ficou desbalanceado, confere os ancestrais
        PaiAux = Pai;

		while (PaiAux->Pai && (PaiAux->Balanco != lado * 2)){ 
			lado = pega_lado_Livros(PaiAux);
			PaiAux = PaiAux->Pai;
			PaiAux->Balanco += lado;
		};
    }
}

void atualiza_pai_Livros (pTlistaLivros L, pTlistaLivros Filho){
	int lado;

    lado = pega_lado_Livros(L);
    if (lado == -1)
	    L->Pai->Esq = Filho;
    else
	    L->Pai->Dir = Filho;

    if (Filho)
        Filho->Pai = L->Pai;
}
pTlistaLivros pega_maior_valor_Livros(pTlistaLivros L){
	while (L->Dir)
		L = L->Dir;
	return L;
}

pTlistaLivros rotacao_direita_Livros(pTlistaLivros No){
	pTlistaLivros Aux;

	Aux = No->Esq;
	No->Esq = Aux->Dir;
	Aux->Pai = No->Pai;
	Aux->Dir = No;
	if (No->Esq)
		No->Esq->Pai = No;
	if (No->Pai){
		if (No == No->Pai->Esq)
			No->Pai->Esq = Aux;   //No eh filho a esquerda
		else
			No->Pai->Dir = Aux;   //No eh filho a direita
	}
	
	No->Pai = Aux;
	No->Balanco = retorna_balanco_Livros(No);
	Aux->Balanco = retorna_balanco_Livros(Aux);
    return Aux;
}

pTlistaLivros rotacao_esquerda_Livros(pTlistaLivros No){
	pTlistaLivros Aux;

	Aux = No->Dir;
	No->Dir = Aux->Esq;
	Aux->Pai = No->Pai;
	Aux->Esq = No;
	if (No->Dir)
		No->Dir->Pai = No;

	if (No->Pai){
		if (No == No->Pai->Esq)
			No->Pai->Esq = Aux;   //No eh filho a esquerda
		else
			No->Pai->Dir = Aux;   //No eh filho a direita
	}

	No->Pai = Aux;
	No->Balanco = retorna_balanco_Livros(No);
	Aux->Balanco = retorna_balanco_Livros(Aux);
    return Aux;
}


//Função que verifica se um nó está balanceado e faz as rotações necessárias.
pTlistaLivros corrige_balanceamento_Livros(pTlistaLivros L) {

    L->Balanco = retorna_balanco_Livros(L);    //Após inserir o nó para esq ou dir, verificar Fator de bal.

    if (L->Balanco < -1) {
        if (retorna_balanco_Livros(L->Esq) > 0)
            L->Esq = rotacao_esquerda_Livros(L->Esq);
        L = rotacao_direita_Livros(L);
    }
    else if (L->Balanco > 1) {
        if (retorna_balanco_Livros(L->Dir) < 0)
            L->Dir = rotacao_direita_Livros(L->Dir);
        L = rotacao_esquerda_Livros(L);
    }
    return L;
}


////////////////////////////////////////////////////////////////////////////
///////////////////////INSERE LIVROS///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

pTlistaLivros insere_no_Livros(pTlistaLivros Pai, pTlistaLivros L, Tlivros X){
	if (!L){
		if ((L = (pTlistaLivros) malloc(sizeof(TlistaLivros))) != NULL){
			L->Info = X;
			L->Esq = NULL;
			L->Dir = NULL;
			L->Balanco = 0;
			L->Pai = Pai;

			if (Pai)
				if ( X.codigo < (Pai)->Info.codigo){
					(Pai)->Esq = L;
					atualiza_balanco_Livros(Pai,-1);
				}
				else{
					(Pai)->Dir = L;
					atualiza_balanco_Livros(Pai, 1);
				}
			
            return (L);			
		}
		else
			printf("\n Erro por falta de memoria!");
	}
	else {
		if (X.codigo < L->Info.codigo)
			insere_no_Livros(L, L->Esq, X);
		else
			insere_no_Livros(L, L->Dir, X);
        L = corrige_balanceamento_Livros(L);
	}
	
	//getchar();
	//getchar();
    return L;
}

void InserirLivros(pTlistaLivros *L){
    Tlivros X;
    int test_int;
    
    while ( !mygeti(&test_int) ){
        fputs("Digite o codigo: ", stdout);
    	fflush(stdout);
    } 
    X.codigo = test_int;
    
	printf("Digite o titulo: ");
	scanf("%s",&X.titulo);
	printf("Digite o autor: ");
	scanf("%s",&X.autor);
	printf("Digite categoria: ");
	scanf("%s",&X.categoria);
	
	while ( !mygeti(&test_int) ){
    	fputs("Digite a quantidade total disponivel: ", stdout);
    	fflush(stdout);
    } 
    X.qtd_total = test_int;
	X.qtd_disp = X.qtd_total;
	
	*L = insere_no_Livros(*L, *L, X);
}

////////////////////////////////////////////////////////////////////////////
///////////////////////QUANTIDADE LIVROS///////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void modificar_qtd_livros(pTlistaLivros L){
	int test_int;
	int adicionar, x;
	    
	printf("-------------------------------------------");
	printf("\n# Aquisicao de volumes #\n");
	while ( !mygeti(&test_int) ){
    	fputs("Digite o codigo do livro que voce deseja adicionar volumes: ", stdout);
    	fflush(stdout);
    } 
    x= test_int;  

	while ( !mygeti(&test_int) ){
        fputs("Quantos volumes serao adicionados: ", stdout);
        fflush(stdout);
    } 
    adicionar = test_int; 
    
    acha_livros_para_adicionar(L, x, adicionar);
}

pTlistaLivros acha_livros_para_adicionar(pTlistaLivros L, int x, int adicionar) {
    
    if (L) {    //Verificar se a árvore não está vazia.
        if (L->Info.codigo == x){     //1 - Verificar se o elemento x está na raiz.
            (L)->Info.qtd_total += adicionar;
	        (L)->Info.qtd_disp += adicionar;
            printf("Livros adicionados com sucesso."); 
        	//getchar();
	        //getchar();
        } else {            
            if (x < L->Info.codigo) 
                L->Esq = acha_livros_para_adicionar(L->Esq, x, adicionar);
            else 
                L->Dir = acha_livros_para_adicionar(L->Dir, x, adicionar);
        }   
    }
    else 
        printf("Codigo %d nao encontrado.\n\n\n", x);
    
    //getchar(); 
    //getchar();
    return L;
}


////////////////////////////////////////////////////////////////////////////
///////////////////////INSERE USUARIOS/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

pTlistaUsuarios insere_no_Usuarios(pTlistaUsuarios PaiU, pTlistaUsuarios U, Tusuarios Y){
	if (!U){
		if ((U = (pTlistaUsuarios) malloc(sizeof(TlistaUsuarios))) != NULL){
			U->InfoU = Y;
			U->EsqU = NULL;
			U->DirU = NULL;
			U->BalancoU = 0;
			U->PaiU = PaiU;

			if (PaiU)
				if ( Y.matricula < (PaiU)->InfoU.matricula){
					(PaiU)->EsqU = U;
					atualiza_balanco_Usuarios(PaiU,-1);
				}
				else{
					(PaiU)->DirU = U;
					atualiza_balanco_Usuarios(PaiU, 1);
				}
			
            return (U);			
		}
		else
			printf("\n Erro por falta de memoria!\n\n\n");
	}
	else {
		if (Y.matricula < U->InfoU.matricula)
			insere_no_Usuarios(U, U->EsqU, Y);
		else
			insere_no_Usuarios(U, U->DirU, Y);
        U = corrige_balanceamento_Usuarios(U);
	}
    return U;
}

void InserirUsuarios(pTlistaUsuarios *U){
	Tusuarios Y;
    int test_int;
    
    while ( !mygeti(&test_int) ){
        fputs("Digite o numero da matricula: ", stdout);
    	fflush(stdout);
    } 
    Y.matricula = test_int;
	
	printf("Digite o nome: ");
	scanf("%s",&Y.nome);
	printf("Digite o telefone: ");
	scanf("%s",&Y.telefone);
	Y.qtd_emp = 0;
	Y.ponteiro_emprestados = NULL;
    
	*U = insere_no_Usuarios(*U, *U, Y);
}

////////////////////////////////////////////////////////////////////////////
///////////////////////REMOÇÃO LIVROS//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

pTlistaLivros remover_folha_Livros(pTlistaLivros L) {
    if (L->Pai)
        atualiza_pai_Livros(L, NULL);
    free(L);
    return NULL;
}

pTlistaLivros remover_1filho_esquerda_Livros(pTlistaLivros L) {
    pTlistaLivros Aux;

    Aux = L->Esq;
    if (L->Pai)
        atualiza_pai_Livros(L, Aux);
    else
        Aux->Pai = NULL;        
    free(L);
    return Aux;
}

pTlistaLivros remover_1filho_direita_Livros(pTlistaLivros L) {
    pTlistaLivros Aux;
    Aux = L->Dir;
    if (L->Pai)	
        atualiza_pai_Livros(L, Aux);
    else
        Aux->Pai = NULL;         
    free(L);
    return Aux;
}

//Em vez de remover NO com 2 galhos, copia a informacao do antecessor 
//para p/ o no a ser eliminado.
//Depois elimina o antecessor, pois este so tem 1 galho (filho).
pTlistaLivros remover_2filhos_Livros(pTlistaLivros L) {
    pTlistaLivros Antecessor;
    int info_antecessor_Livros;

    Antecessor = pega_maior_valor_Livros(L->Esq);  //Pega um ponteiro para o sucessor.

    info_antecessor_Livros = Antecessor->Info.codigo;                     //Pega o valor do sucessor.
    L->Esq = localizara_para_remover_Livros(L->Esq, info_antecessor_Livros);     //remover da árvore o nó que substituirá o nó com 2 filhos.

    L->Info.codigo = info_antecessor_Livros;                       //substituir o valor do nó com 2 filhos.
    L->Balanco = retorna_balanco_Livros(L);
    return L;
}

//Função para verificar o tipo de nó que será removido.
pTlistaLivros remover_no_Livros(pTlistaLivros L) {
    
    if (L->Dir == L->Esq)  //verifica se L é folha
        L = remover_folha_Livros(L);
    else 
        if (L->Dir == NULL) 
            L = remover_1filho_esquerda_Livros(L);
        else 
            if (L->Esq == NULL) 
                L = remover_1filho_direita_Livros(L);
            else {
                L = remover_2filhos_Livros(L);
                L = corrige_balanceamento_Livros(L);
            }   

    return L;
}

pTlistaLivros localizara_para_remover_Livros(pTlistaLivros L, int x) {

    if (L) {    //Verificar se a árvore não está vazia.
        if (L->Info.codigo == x)     //1 - Verificar se o elemento x está na raiz.
            if (L->Info.qtd_disp < L->Info.qtd_total){
	            printf("ERRO: Existem livros que nao foram devolvidos.\n\n\n");
	            //getchar();
	            //getchar();
	        }else{
                L = remover_no_Livros(L);
	        }
        else {            
            if (x < L->Info.codigo) 
                L->Esq = localizara_para_remover_Livros(L->Esq, x);
            else 
                L->Dir = localizara_para_remover_Livros(L->Dir, x);
            
            L = corrige_balanceamento_Livros(L);
        }        
    }
    else 
        printf("Livro %d nao encontrado.\n\n\n", x);
    
    //getchar();
    //getchar();
    return L;
}

void Remover_Livros(pTlistaLivros *L){
	int X;
	
	printf("\n\nDigite o codigo do livro a ser removido: ");
	scanf("%d", &X);
    
    *L = localizara_para_remover_Livros (*L, X);
}

////////////////////////////////////////////////////////////////////////////
///////////////////////REMOÇÃO USUARIOS////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

pTlistaUsuarios remover_folha_Usuarios(pTlistaUsuarios U) {
    if (U->PaiU)
        atualiza_pai_Usuarios(U, NULL);
    free(U);
    return NULL;
}

pTlistaUsuarios remover_1filho_esquerda_Usuarios(pTlistaUsuarios U) {
    pTlistaUsuarios Aux;

    Aux = U->EsqU;
    if (U->PaiU)
        atualiza_pai_Usuarios(U, Aux);
    else
        Aux->PaiU = NULL;        
    free(U);
    return Aux;
}

pTlistaUsuarios remover_1filho_direita_Usuarios(pTlistaUsuarios U) {
    pTlistaUsuarios Aux;
    Aux = U->DirU;
    if (U->PaiU)	
            atualiza_pai_Usuarios(U, Aux);
    else
        Aux->PaiU = NULL;         
    free(U);
    return Aux;
}

//Em vez de remover NO com 2 galhos, copia a informacao do antecessor 
//para p/ o no a ser eliminado.
//Depois elimina o antecessor, pois este so tem 1 galho (filho).
pTlistaUsuarios remover_2filhos_Usuarios(pTlistaUsuarios U) {
    pTlistaUsuarios Antecessor;
    int info_antecessor_Usuarios;

    Antecessor = pega_maior_valor_Usuarios(U->EsqU);  //Pega um ponteiro para o sucessor.

    info_antecessor_Usuarios = Antecessor->InfoU.matricula;                     //Pega o valor do sucessor.
    U->EsqU = localizara_para_remover_Usuarios(U->EsqU, info_antecessor_Usuarios);     //remover da árvore o nó que substituirá o nó com 2 filhos.

    U->InfoU.matricula= info_antecessor_Usuarios;                       //substituir o valor do nó com 2 filhos.
    U->BalancoU = retorna_balanco_Usuarios(U);
    return U;
}

//Função para verificar o tipo de nó que será removido.
pTlistaUsuarios remover_no_Usuarios(pTlistaUsuarios U) {
    
    if (U->DirU == U->EsqU)  //verifica se L é folha
        U = remover_folha_Usuarios(U);
    else 
        if (U->DirU == NULL) 
            U = remover_1filho_esquerda_Usuarios(U);
        else 
            if (U->EsqU == NULL) 
                U = remover_1filho_direita_Usuarios(U);
            else {
                U = remover_2filhos_Usuarios(U);
                U = corrige_balanceamento_Usuarios(U);
            }   

    return U;
}

pTlistaUsuarios localizara_para_remover_Usuarios(pTlistaUsuarios U, int x) {

    if (U) {    //Verificar se a árvore não está vazia.
        if (U->InfoU.matricula == x)     //1 - Verificar se o elemento x está na raiz.
            if (U->InfoU.qtd_emp != 0){
	            printf("ERRO: Este usuario, ainda tem livros emprestados.\n\n\n");
              	//getchar();
              	//getchar();
	        }else{
                U = remover_no_Usuarios(U); 
	        }
        else {            
            if (x < U->InfoU.matricula) 
                U->EsqU = localizara_para_remover_Usuarios(U->EsqU, x);
            else 
                U->DirU = localizara_para_remover_Usuarios(U->DirU, x);
            
            U = corrige_balanceamento_Usuarios(U);
        }        
    }
    else 
        printf("Usuario %d nao encontrado.\n\n\n", x);
    
    //getchar();
    //getchar();
    return U;
}

void Remover_Usuarios(pTlistaUsuarios *U){
	int X;
	
	printf("\n\nDigite a matricula do usuario a ser removido: ");
	scanf("%d", &X);
	
	*U = localizara_para_remover_Usuarios (*U, X);
}

////////////////////////////////////////////////////////////////////////////
///////////////////////BALANÇO SUB ARVORE//////////////////////////////////
//////////////////////////////////////////////////////////////////////////


int pega_lado_Emprestados(pTlistaEmprestados NoE){
	if (NoE->PaiE){
		if (NoE == NoE->PaiE->EsqE)
			return -1;
		else
			return 1;
	}
	else
		return 0;
}

int pega_altura_Emprestados(pTlistaEmprestados E) {
   
    int esq, dir;
   
    esq = 0;
    dir = 0;
    if ( E ) {   
        esq = pega_altura_Emprestados(E->EsqE);
        dir = pega_altura_Emprestados(E->DirE);
   
        if ( esq > dir )
            return esq + 1;
        else
            return dir + 1;
    }
    else
        return -1;
}

int retorna_balanco_Emprestados(pTlistaEmprestados E){
    return pega_altura_Emprestados(E->DirE) - pega_altura_Emprestados(E->EsqE);
}


void atualiza_balanco_Emprestados(pTlistaEmprestados PaiE, int ladoE){
	pTlistaEmprestados PaiAux;

	PaiE->BalancoE += ladoE;  //atualizo o pai	

	if (PaiE->BalancoE){	//se o pai ficou desbalanceado, confere os ancestrais
        PaiAux = PaiE;

		while (PaiAux->PaiE && (PaiAux->BalancoE != ladoE * 2)){ 
			ladoE = pega_lado_Emprestados(PaiAux);
			PaiAux = PaiAux->PaiE;
			PaiAux->BalancoE += ladoE;
		};
    }
}

void atualiza_pai_Emprestados (pTlistaEmprestados E, pTlistaEmprestados FilhoE){
	int lado;

    lado = pega_lado_Emprestados(E);
    if (lado == -1)
	    E->PaiE->EsqE = FilhoE;
    else
	    E->PaiE->DirE = FilhoE;

    if (FilhoE)
        FilhoE->PaiE = E->PaiE;
}

pTlistaEmprestados pega_maior_valor_Emprestados(pTlistaEmprestados E){
	while (E->DirE)
		E = E->DirE;
	return E;
}

pTlistaEmprestados rotacao_direita_Emprestados(pTlistaEmprestados NoE){
	pTlistaEmprestados Aux;

	Aux = NoE->EsqE;
	NoE->EsqE = Aux->DirE;
	Aux->PaiE = NoE->PaiE;
	Aux->DirE = NoE;
	if (NoE->EsqE)
		NoE->EsqE->PaiE = NoE;
	if (NoE->PaiE){
		if (NoE == NoE->PaiE->EsqE)
			NoE->PaiE->EsqE = Aux;   //No eh filho a esquerda
		else
			NoE->PaiE->DirE = Aux;   //No eh filho a direita
	}
	
	NoE->PaiE = Aux;
	NoE->BalancoE = retorna_balanco_Emprestados(NoE);
	Aux->BalancoE = retorna_balanco_Emprestados(Aux);
    return Aux;
}

pTlistaEmprestados rotacao_esquerda_Emprestados(pTlistaEmprestados NoE){
	pTlistaEmprestados Aux;

	Aux = NoE->DirE;
	NoE->DirE = Aux->EsqE;
	Aux->PaiE = NoE->PaiE;
	Aux->EsqE = NoE;
	if (NoE->DirE)
		NoE->DirE->PaiE = NoE;

	if (NoE->PaiE){
		if (NoE == NoE->PaiE->EsqE)
			NoE->PaiE->EsqE = Aux;   //No eh filho a esquerda
		else
			NoE->PaiE->DirE = Aux;   //No eh filho a direita
	}

	NoE->PaiE = Aux;
	NoE->BalancoE = retorna_balanco_Emprestados(NoE);
	Aux->BalancoE = retorna_balanco_Emprestados(Aux);
    return Aux;
}


//Função que verifica se um nó está balanceado e faz as rotações necessárias.
pTlistaEmprestados corrige_balanceamento_Emprestados(pTlistaEmprestados E) {

    E->BalancoE = retorna_balanco_Emprestados(E);    //Após inserir o nó para esq ou dir, verificar Fator de bal.

    if (E->BalancoE < -1) {
        if (retorna_balanco_Emprestados(E->EsqE) > 0)
            E->EsqE = rotacao_esquerda_Emprestados(E->EsqE);
        E = rotacao_direita_Emprestados(E);
    }
    else if (E->BalancoE > 1) {
        if (retorna_balanco_Emprestados(E->DirE) < 0)
            E->DirE = rotacao_direita_Emprestados(E->DirE);
        E = rotacao_esquerda_Emprestados(E);
    }
    return E;
}

////////////////////////////////////////////////////////////////////////////
///////////////////////EMPRESTIMO LIVROS///////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void chama_usuario_para_emprestimo(pTlistaUsuarios U, pTlistaLivros L){
    int x, y, test_int;
    printf("-------------------------------------------");
	printf("\n# Empretimo de livro#\n");
	while ( !mygeti(&test_int) ){
    	fputs("Digite a matricula do usuario: ", stdout);
    	fflush(stdout);
    } 
    x = test_int;
    
	while ( !mygeti(&test_int) ){
    	fputs("Digite o codigo do livro: ", stdout);
    	fflush(stdout);
    } 
    y = test_int; 
    
    acha_usuarios_para_emprestimo(U, L, x, y);
}

pTlistaUsuarios acha_usuarios_para_emprestimo(pTlistaUsuarios U, pTlistaLivros L, int x, int y) {
    if (U) {    //Verificar se a árvore não está vazia.
        if (U->InfoU.matricula == x)     //1 - Verificar se o elemento x está na raiz.
            acha_livros_para_emprestimo(L, U, y);  
        else {            
            if (x < U->InfoU.matricula) 
                U->EsqU = acha_usuarios_para_emprestimo(U->EsqU, L, x, y);
            else 
                U->DirU = acha_usuarios_para_emprestimo(U->DirU, L, x, y);
        }        
    }
    else 
        printf("Matricula %d nao encontrada.\n\n\n", x);
    
    //getchar();
    //getchar();
    return U;
}

pTlistaLivros acha_livros_para_emprestimo(pTlistaLivros L, pTlistaUsuarios U, int y) {
 
    if (L) {    //Verificar se a árvore não está vazia.
        if (L->Info.codigo == y){     //1 - Verificar se o elemento x está na raiz.
            if(L->Info.qtd_disp > 0){ 
                InserirEmprestados(L, U);  
            }else{
               printf("O livro nao contem volumes disponiveis no momento.\n\n\n");
               //getchar();
               //getchar();
            }
        } else {            
            if (y < L->Info.codigo) 
                L->Esq = acha_livros_para_emprestimo(L->Esq, U, y);
            else 
                L->Dir = acha_livros_para_emprestimo(L->Dir, U, y);
        }   
    }
    else 
        printf("Codigo %d nao encontrado.\n\n\n", y);
    
    //getchar();
    //getchar();
    return L;
}

////////////////////////////////////////////////////////////////////////////
///////////////////////INSERE LIVROS NA SUB ARVORE/////////////////////////
//////////////////////////////////////////////////////////////////////////

pTlistaEmprestados insere_no_Emprestados(pTlistaEmprestados PaiE, pTlistaEmprestados E, Temprestimo X){
	if (!E){
		if ((E = (pTlistaEmprestados) malloc(sizeof(TlistaEmprestados))) != NULL){
			E->InfoE = X;
			E->EsqE = NULL;
			E->DirE = NULL;
			E->BalancoE = 0;
			E->PaiE = PaiE;

			if (PaiE)
				if ( X.cod_livro < (PaiE)->InfoE.cod_livro){
					(PaiE)->EsqE = E;
					atualiza_balanco_Emprestados(PaiE, -1);
				}
				else{
					(PaiE)->DirE = E;
					atualiza_balanco_Emprestados(PaiE, 1);
				}
			
            return (E);			
		}
		else
			printf("\n Erro por falta de memoria!\n\n\n");
	}
	else {
		if (X.cod_livro < E->InfoE.cod_livro)
			insere_no_Emprestados(E, E->EsqE, X);
		else
			insere_no_Emprestados(E, E->DirE, X);
        E = corrige_balanceamento_Emprestados(E);
	}
    return E;
}

void InserirEmprestados(pTlistaLivros L, pTlistaUsuarios U){
	Temprestimo X;
	X.cod_livro = L->Info.codigo;
	
	printf("Informe a data do emprestimo: ");
	scanf("%s", &X.data_emp);
	X.ponteiro_L = L;

	L->Info.qtd_disp -= 1;
	U->InfoU.qtd_emp += 1;
	
	U->InfoU.ponteiro_emprestados = insere_no_Emprestados(U->InfoU.ponteiro_emprestados, U->InfoU.ponteiro_emprestados, X);
}


////////////////////////////////////////////////////////////////////////////
///////////////////////REMOVE LIVROS NA SUB ARVORE/////////////////////////
//////////////////////////////////////////////////////////////////////////

pTlistaEmprestados remover_folha_Emprestados(pTlistaEmprestados E) {
    if (E->PaiE)
        atualiza_pai_Emprestados(E, NULL);
    
    free(E);
    return NULL;
}

pTlistaEmprestados remover_1filho_esquerda_Emprestados(pTlistaEmprestados E) {
    pTlistaEmprestados Aux;

    Aux = E->EsqE;
    if (E->PaiE)
        atualiza_pai_Emprestados(E, Aux);
    else
        Aux->PaiE = NULL;        
     
    free(E);
    return Aux;
}

pTlistaEmprestados remover_1filho_direita_Emprestados(pTlistaEmprestados E) {
    pTlistaEmprestados Aux;

    Aux = E->DirE;
    if (E->PaiE)	
        atualiza_pai_Emprestados(E, Aux);
    else
        Aux->PaiE = NULL;         

    free(E);
    return Aux;
}

//Em vez de remover NO com 2 galhos, copia a informacao do antecessor 
//para p/ o no a ser eliminado.
//Depois elimina o antecessor, pois este so tem 1 galho (filho).
pTlistaEmprestados remover_2filhos_Emprestados(pTlistaEmprestados E) {
    pTlistaEmprestados Antecessor;
    int info_antecessor;

    Antecessor = pega_maior_valor_Emprestados(E->EsqE);  //Pega um ponteiro para o sucessor.

    info_antecessor = Antecessor->InfoE.cod_livro;                     //Pega o valor do sucessor.
    E->EsqE = localizara_para_remover_Emprestados(E->EsqE, info_antecessor);     //remover da árvore o nó que substituirá o nó com 2 filhos.

    E->InfoE.cod_livro = info_antecessor;                       //substituir o valor do nó com 2 filhos.
    E->BalancoE = retorna_balanco_Emprestados(E);
    return E;
}

//Função para verificar o tipo de nó que será removido.
pTlistaEmprestados remover_no_Emprestados(pTlistaEmprestados E) {
    
    if (E->DirE == E->EsqE)  //verifica se L é folha
        E = remover_folha_Emprestados(E);
    else 
        if (E->DirE == NULL) 
            E = remover_1filho_esquerda_Emprestados(E);
        else 
            if (E->EsqE == NULL) 
                E = remover_1filho_direita_Emprestados(E);
            else {
                E = remover_2filhos_Emprestados(E);
                E = corrige_balanceamento_Emprestados(E);
            }   

    return E;
}

pTlistaEmprestados localizara_para_remover_Emprestados(pTlistaEmprestados E, int x) {

    if (E) {    //Verificar se a árvore não está vazia.
        if (E->InfoE.cod_livro == x)     //1 - Verificar se o elemento x está na raiz.
            E = remover_no_Emprestados(E);  
        else {            
            if (x < E->InfoE.cod_livro) 
                E->EsqE = localizara_para_remover_Emprestados(E->EsqE, x);
            else 
                E->DirE = localizara_para_remover_Emprestados(E->DirE, x);
            
            E = corrige_balanceamento_Emprestados(E);
        }        
    }
    else 
        printf("Emprestimo do livro %d nao encontrado.\n\n\n", x);
    
    //getchar();
    //getchar();
    return E;
}

void chama_usuario_para_devolucao(pTlistaUsuarios U, pTlistaLivros L){
    int x, y, test_int;
    
    printf("-------------------------------------------");
	  printf("\n# Devolucao de livro#\n");
	  while ( !mygeti(&test_int) ){
    	fputs("Digite a matricula do usuario: ", stdout);
    	fflush(stdout);
    } 
    x = test_int; 
    
    while ( !mygeti(&test_int) ){
    	fputs("Digite o codigo do livro: ", stdout);
    	fflush(stdout);
    } 
    y = test_int; 
    
    acha_usuarios_para_devolucao(U, L, x, y);
}

pTlistaUsuarios acha_usuarios_para_devolucao(pTlistaUsuarios U, pTlistaLivros L, int x, int y) {
    if (U) {    //Verificar se a árvore não está vazia.
        if (U->InfoU.matricula == x)     //1 - Verificar se o elemento x está na raiz.
            acha_livros_para_devolucao(L, U, y);  
        else {            
            if (x < U->InfoU.matricula) 
                U->EsqU = acha_usuarios_para_devolucao(U->EsqU, L, x, y);
            else 
                U->DirU = acha_usuarios_para_devolucao(U->DirU, L, x, y);
        }        
    }
    else 
        printf("Matricula %d nao encontrada.\n\n\n", x);
    
    //getchar();
    //getchar();
    return U;
}

pTlistaLivros acha_livros_para_devolucao(pTlistaLivros L, pTlistaUsuarios U, int y) {
 
    if (L) {    //Verificar se a árvore não está vazia.
        if (L->Info.codigo == y){     //1 - Verificar se o elemento x está na raiz.
            if(L->Info.qtd_disp < L->Info.qtd_total){ 
                L->Info.qtd_disp += 1;
                U->InfoU.qtd_emp -=1;
                U->InfoU.ponteiro_emprestados = localizara_para_remover_Emprestados (U->InfoU.ponteiro_emprestados, y);  
            }else{
               printf("O livro ja possui todos os volumes armazenados.\n\n\n");
               //getchar();
               //getchar();
            }
        } else {            
            if (y < L->Info.codigo) 
                L->Esq = acha_livros_para_devolucao(L->Esq, U, y);
            else 
                L->Dir = acha_livros_para_devolucao(L->Dir, U, y);
        }   
    }
    else 
        printf("Codigo %d nao encontrado.\n\n\n", y);
    
    //getchar();
    //getchar();
    return L;
}


////////////////////////////////////////////////////////////////////////////
///////////////////////IMPRIMIR ARVORE/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


int imprimirArvoreLivro(TlistaLivros *L, int e_esq, int offset, int profundidade, char s[20][255]){
    
    char b[20];
    int largura = 5;

    if (!L) return 0;

    sprintf(b, "(%03d)", L->Info.codigo);
    
    int Esq  = imprimirArvoreLivro(L->Esq,  1, offset, profundidade + 1, s);
    int Dir = imprimirArvoreLivro(L->Dir, 0, offset + Esq + largura, profundidade + 1, s);
#ifdef COMPACT
    for (int i = 0; i < largura; i++)
        s[profundidade][offset + Esq + i] = b[i];

    if (profundidade && e_esq) {

        for (int i = 0; i < largura + Dir; i++)
            s[profundidade - 1][offset + Esq + largura/2 + i] = '-';

        s[profundidade - 1][offset + Esq + largura/2] = '.';

    } else if (profundidade && !e_esq) {

        for (int i = 0; i < Esq + largura; i++)
            s[profundidade - 1][offset - largura/2 + i] = '-';

        s[profundidade - 1][offset + Esq + largura/2] = '.';
    }
#else
    for (int i = 0; i < largura; i++)
        s[2 * profundidade][offset + Esq + i] = b[i];

    if (profundidade && e_esq) {

        for (int i = 0; i < largura + Dir; i++)
            s[2 * profundidade - 1][offset + Esq + largura/2 + i] = '-';

        s[2 * profundidade - 1][offset + Esq + largura/2] = '+';
        s[2 * profundidade - 1][offset + Esq + largura + Dir + largura/2] = '+';

    } else if (profundidade && !e_esq) {

        for (int i = 0; i < Esq + largura; i++)
            s[2 * profundidade - 1][offset - largura/2 + i] = '-';

        s[2 * profundidade - 1][offset + Esq + largura/2] = '+';
        s[2 * profundidade - 1][offset - largura/2 - 1] = '+';
    }
#endif

    return Esq + largura + Dir;
}

void imprimeArvore(TlistaLivros *L){
    char s[20][255];
    for (int i = 0; i < 20; i++){
        gotoxy(80,80);
        sprintf(s[i], "%80s", " ");
    }
    imprimirArvoreLivro(L, 0, 0, 0, s);

    for (int i = 0; i < 20; i++){
        gotoxy(80,80);
        printf("%s\n", s[i]);
    }
    getchar();
    getchar();
}

////////////////////////////////////////////////////////////////////////////
///////////////////////LIBERA MEMORIA /////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//Modulo que faz a liberação de memoria de livros.
void libera_memoriaL(pTlistaLivros L){
    pTlistaLivros Aux;
	if (L != NULL) {
		libera_memoriaL(L->Esq);
		libera_memoriaL(L->Dir);
		free(L);
  }
  //printf("\nMemoria de livro liberada.");
}

    //Modulo que faz a liberação de memoria de usuarios.
void libera_memoriaU(pTlistaUsuarios U){
	pTlistaUsuarios Aux;
	if (U != NULL) {
	    libera_memoriaE(U->InfoU.ponteiro_emprestados); //libera memoria alocada para arvore emprestados.
		libera_memoriaU(U->EsqU);
		libera_memoriaU(U->DirU);
		free(U);
  }
  //printf("\nMemoria de usuario liberada.");
}

    //Modulo que faz a liberação de memoria de usuarios.
void libera_memoriaE(pTlistaEmprestados E){
	pTlistaEmprestados Aux;
	if (E != NULL) {
		libera_memoriaU(E->EsqE);
		libera_memoriaU(E->DirE);
		free(E);
  }
  //printf("\nMemoria de emprestimo liberada.");
}

void creditos(){
    printf("\n***************************************************************************************************\nEstrutura de Dados e Algoritmos\n");
    printf("Turma: Analise e Desenvolvimento de Sistemas (ADS 2018) - IFBA Eunapolis\n");
    printf("Trabalho: Sistema de biblioteca\nDiscentes: Jéssica Alves Batista; João Vitor Oliveira Mendes; Tande Henrique Cerqueira\n");
	printf("Prof.: Danilo Alves Martins de Faria\n***************************************************************************************************\n");
    getchar();
}

int main(){
	char opcao;
	pTlistaLivros L;
	pTlistaUsuarios U;
    int x, senha_gerente, cod_gerente, test_int, cont, op;
    
	L = NULL;
    U = NULL;
	op = -1;
	cont = 0;

// login obrigatorio para liberar o menu principal	
	do{	      
		if (cont != 0){
			printf("\n(Erro) Informacoes invalidas. Por favor tente novamente ");
		}
		
		printf("\n## Preencha o login ##\n\n");
		printf("Codigo: ");
		while ( !mygeti(&test_int) ){
	   		fputs("\(Erro) Digite o codigo: ", stdout);
    		fflush(stdout);
    	} 
    	cod_gerente = test_int;
    	
		printf("Senha: ");
		while ( !mygeti(&test_int) ){
    		fputs("\n(Erro) Digite a senha: ", stdout);
    		fflush(stdout);
    	} 
    	senha_gerente = test_int;
    	
    	if (senha_gerente == 123 && cod_gerente == 456){// testa se a senha e codigo estao corretos e emite mensagem de confirmação
    		printf("\n\n\n## Menu Liberado ##\n");
    	}else{
    		cont += 1; //contador para liberar mensagem de erro
    	}

	}while(senha_gerente != 123 || cod_gerente != 456);	// se mantem no laço enquando a senha e o codigo estiverem incorretos
		    
	while (op != 0){
		printf("-------------------------------------------");
		printf("\n\n**** MENU PRINCIPAL ****\n\n");
		printf("-> Opcoes sobre livros:\n\n");
      	printf("(1) Inserir Livro\n(2) Aquisicao de volumes\n(3) Consultar informacoes de um livro\n(4) Remover livro\n(5) Imprimir Arvore");
		printf("\n\n-> Opcoes sobre usuarios:\n\n");
		printf("(6) Inserir Usuario\n(7) Consultar informacoes de um usuario\n(8) Remover usuario");
		printf("\n\n-> Opcoes sobre emprestimo:\n\n");
		printf("(9) Empretimo\n(10) Devolucao");
		printf("\n\nOBS.:\nPara sair digite '0'\nPara ver os creditos digite '11'");
		printf("\n\n-------------------------------------------\nDigite a opcao desejada: ");
		
		/*do{
		    scanf("%d", &op);
		}while (op >= 10 || op <= 0);*/
		
		while ( !mygeti(&test_int) ){
    		printf("(Erro) Digite a opcao desejada: ", stdout);
    		fflush(stdout);
    	}
		op = test_int;
		
		switch(op){
			//LIVRO
			case 1 : InserirLivros(&L); //Passagem por referencia, pois pode alterar valores.
			break;
				
			case 2 : modificar_qtd_livros(L); //Passagem por referencia, pois pode alterar valores.
			break;
			
			case 3 : chama_acha_para_imprimirLivros(L); //Passagem por valor, pois NAO alterara a lista.
			break;
					
			case 4 : Remover_Livros(&L); //Passagem por referencia, pois pode alalterar valores.
			break;
			
			case 5: imprimeArvore(L);
			break;
					
			//USUARIO
			case 6 : InserirUsuarios(&U); //Passagem por referencia, pois pode alterar valores.
			break;
				
			case 7 : acha_para_imprimirUsuarios(U); //Passagem por valor, pois NAO alterara a lista.
			break;
				
			case 8 : Remover_Usuarios(&U); //Passagem por referencia, pois pode alterar valores.
			break;
			
			//EMPRESTIMO	
			case 9 : chama_usuario_para_emprestimo(U, L); //Passagem por referencia, pois pode alterar valores.
			break;
					
			case 10 : chama_usuario_para_devolucao(U, L); //Passagem por referencia, pois pode alterar valores.
			break;
			
			//CREDITOS
			case 11 : creditos();
			break;
			
			default :
				if (op != 0) {
					printf("\n(Erro) Opcao invalida. Tente novamente."); //mensagem de erro para números fora do menu
				}					
		};
	}
	printf("\n- Programa finalizado -");

    libera_memoriaL(L); //libera memoria alocada para arvore livros.
    libera_memoriaU(U); //libera memoria alocada para arvore usuarios.
}
