#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "headfile.h"

//FYI: O programa demora cerca de 1 minuto e meio a correr para os 60000 candidatos (exemplo 98 e 99)!

//Função para identificar cada curso
struct Cursos* og_cursos (char* file_cursos, char* vagas_curso_comando){

    //Abertura do ficheiro de entrada dos cursos
    if(file_cursos==NULL){
        file_cursos="cursos.csv";
    }

    FILE* input;
    input = fopen(file_cursos, "r");
    if (input == NULL) {
        printf("Erro a abrir o ficheiro.\n");
        exit(1);
    }

    //organiza os dados de cada curso numa lista
    char linha[200];
    struct Cursos* inicio_cursos = NULL;
    struct Cursos* fim_cursos = NULL;

    if(fgets(linha, 200 , input) == NULL) {
        printf("Ficheiro em branco.\n");
    }

    while (fgets(linha, 200 ,input)){

        struct Cursos* atual_cursos = calloc(1, sizeof(struct Cursos));

        char* identificador = strtok(linha, ",");
        atual_cursos->id_inst = atoi(identificador);

        identificador = strtok(NULL, ",");
        atual_cursos->id_curso=calloc(strlen(identificador)+1,sizeof(char));
        strcpy(atual_cursos->id_curso, identificador);

        identificador = strtok(NULL, ",");
        atual_cursos->nome_inst=calloc(strlen(identificador)+1,sizeof(char));
        strcpy(atual_cursos->nome_inst, identificador);

        identificador = strtok(NULL, ",");
        atual_cursos->nome_curso=calloc(strlen(identificador)+1,sizeof(char));
        strcpy(atual_cursos->nome_curso, identificador);

        identificador = strtok(NULL, ",");
        strcpy(atual_cursos->grau, identificador);

        //no caso de se recorrer ao comando que seleciona o número de vagas de cada curso
        if(vagas_curso_comando==NULL){
        identificador = strtok(NULL, "\n");
        atual_cursos->vagas_iniciais = atoi(identificador);}

        else{
            atual_cursos->vagas_iniciais=atoi(vagas_curso_comando);
        }

        atual_cursos->vagas_sobram = atual_cursos->vagas_iniciais; //antes da colocação o numero de vagas que sobram é o mesmo que o numero de vagas iniciais
        atual_cursos->num_colocados = 0; //inicialmente não há colocados


        //para percorrer a lista de cursos
        if(inicio_cursos==NULL) {
            inicio_cursos = atual_cursos;
            fim_cursos = atual_cursos;
        }
        else {
            fim_cursos->prox_cursos = atual_cursos;
            fim_cursos = atual_cursos;
        }
    }

    fclose(input);
    return inicio_cursos;
}

//Função para identificar cada candidato
struct node* og_candidatos (char* file_candidatos, char* n_candidatos_comando){

    //Abertura do ficheiro de entrada de candidatos
    if(file_candidatos==NULL){
        file_candidatos="candidatos.csv";
    }

    FILE* input;
    input = fopen(file_candidatos, "r");
    if (input == NULL) {
        printf("Erro a abrir o ficheiro.\n");
        exit(1);
    }

    //organiza os dados de cada candidato numa lista
    int linhas=1; //variavel que conta o numero de candidatos lidos do ficheiro de entrada
    char linha[200];
    struct node* inicio_node=NULL;
    struct node* fim_node=NULL;

    if(fgets(linha, 200 , input) == NULL) {
        printf("Ficheiro em branco.\n");
    }

    while (fgets(linha, 200 ,input)){

            //no caso de se recorrer ao comando que seleciona o numero de candidatos
            if(n_candidatos_comando!=NULL && linhas>atoi(n_candidatos_comando)) {
                break;
            }

            struct Candidato* atual_candidato = calloc(1, sizeof(struct Candidato));

            char* identificador = strtok(linha, ",");
            atual_candidato->id_candi= atoi(identificador);

            identificador = strtok(NULL, ",");
            atual_candidato->nota_ing=calloc(strlen(identificador)+1,sizeof(char));
            strcpy(atual_candidato->nota_ing, identificador);

            identificador = strtok(NULL, ",");
            atual_candidato->media_sec=calloc(strlen(identificador)+1,sizeof(char));
            strcpy(atual_candidato->media_sec, identificador);

            identificador = strtok(NULL, ",");
            atual_candidato->nota_candi = atof(identificador);

            //guarda o resto da linha num array que tem apenas a parte das opções
            identificador = strtok(NULL, "\n");
            char *resto_linha=calloc(strlen(identificador)+1,sizeof(char));
            strcpy(resto_linha, identificador);

            char *resto_linha_2 =calloc(strlen(identificador)+1,sizeof(char));
            strcpy(resto_linha_2, resto_linha);

            //conta o número de identificadores de opcoes deste candidato (ide , idc) (2*nº de opções)
            strtok(resto_linha, ",");
            strtok(NULL, ",");

            int count = 2;
            while (strtok(NULL, ",") != NULL || (strtok(NULL, ",") == NULL && strtok(NULL, "\n") != NULL)) {
                count++;
            }

            //divide a string em identificadores usando a vírgula como delimitador e guarda-os num array (ident_opcoes)
            char **ident_opcoes = malloc((count)* sizeof(char*));

            for (int j = 0; j < count; j++) {
                ident_opcoes[j] = malloc(5 * sizeof(char));
            }

            identificador = strtok(resto_linha_2, ",");
            strcpy(ident_opcoes[0], identificador);

            for (int i = 1; i < count; i++) {
                identificador = strtok(NULL, ",");
                if (identificador == NULL) {
                    identificador = strtok(NULL, "\n");
                    strcpy(ident_opcoes[i] , identificador);
                    break;
                }
                strcpy(ident_opcoes[i] , identificador);
            }

            //guarda as opcoes no candidato correspondente
            int l = 0;
            for(int k = 0; k< count && l<(count/2); k+=2)
            {
                strcpy(atual_candidato->opcoes[l].ide, ident_opcoes[k]);
                strcpy(atual_candidato->opcoes[l].idc, ident_opcoes[k+1]);

                l++;
            }

        //liberta a memória alocada no processo de guardar as opções de cada candidato
        for (int u = 0; u < count; u++) {
            free(ident_opcoes[u]);
        }
        free(ident_opcoes);
        free(resto_linha);
        free(resto_linha_2);

        //associar os dados de cada candidato a um elemento da lista simplesmente ligada
        struct node* atual_node = calloc(1, sizeof(struct node));
        atual_node->candidato = atual_candidato;

        //para percorrer a lista a cada loop
        if(inicio_node==NULL) {
            inicio_node = atual_node;
            fim_node = atual_node;
        }
        else {
            fim_node->next = atual_node;
            fim_node = atual_node;
        }
        linhas++;
    }

    fclose(input);
    return inicio_node;

}

//função que troca a ordem de dois elementos de uma lista
void trocar(struct node* i, struct node* j) {

    struct Candidato* temp = i->candidato;
    i->candidato = j->candidato;
    j->candidato = temp;

}

//Ordenação de candidatos por ordem decrescente de nota de candidatura
struct node* ordenar_candidato_nota(struct node* inicio_node_ordenado_nota) {
    struct node *aqui, *prox;
    aqui = inicio_node_ordenado_nota;
    prox = aqui->next;
    int troca = 1;
    struct node* ultimo = NULL;

    while (troca == 1) {
        troca = 0;

        while (prox != ultimo) {
            if (aqui->candidato->nota_candi < prox->candidato->nota_candi) {
                trocar(aqui, prox);
                troca = 1;
            }
            aqui = aqui->next;
            prox = prox->next;
        }
        ultimo = aqui;
        aqui = inicio_node_ordenado_nota;
        prox = aqui->next;
    }

    return inicio_node_ordenado_nota;

}

//função para copiar os dados do candidato da lista de candidadatos ordenados, para o candidato da lista de candidatos colocados
void coloca_candi_nos_colocados (struct node *inicio_node_ordenado_nota, struct Cursos *curso_atual, int l) {

    //preenche dados do candidato, na lista de colocados no curso respetivo
    struct node* temp_colocado = curso_atual->candidatos_colocados;

    curso_atual->candidatos_colocados=calloc(1,sizeof(struct node));
    curso_atual->candidatos_colocados->candidato=calloc(1,sizeof(struct Candidato));

    curso_atual->candidatos_colocados->candidato->id_candi=inicio_node_ordenado_nota->candidato->id_candi;

    curso_atual->candidatos_colocados->candidato->nota_candi = inicio_node_ordenado_nota->candidato->nota_candi;

    curso_atual->candidatos_colocados->candidato->opcao_colocacao=l+1; //opção em que o candidato foi colocado

    inicio_node_ordenado_nota->candidato->colocacao = 1;//1=Colocado (vai ser utilizado para criar uma lista com os não colocados)

    curso_atual->nota_minima = inicio_node_ordenado_nota->candidato->nota_candi; //nota do último a entrar no curso

    curso_atual->num_colocados++; //incrementa sempre que alguém é colocado no curso em causa

    curso_atual->candidatos_colocados->next = temp_colocado; /*para colocar o colocado seguinte no inicio da lista de forma a ficarem
    ordenados de forma ascendente de nota e decrescente de ID*/

}

//colocação de cada candidato no respetivo curso
struct Cursos* colocacao_geral (struct node *inicio_node_ordenado_nota, struct Cursos *inicio_cursos){

    //percorre candidatos
    while(inicio_node_ordenado_nota != NULL){

        inicio_node_ordenado_nota->candidato->colocacao = 0;//0=Nao colocado

        //percorre opções
        for(int l=0;l<5;l++){
            if(inicio_node_ordenado_nota->candidato->colocacao==1){
                break;
                }

            //percorre cursos
            struct Cursos* curso_atual = inicio_cursos; //para voltar ao início da lista dos cursos
            while(curso_atual!=NULL){

                //enquanto existirem vagas
                if((atoi(inicio_node_ordenado_nota->candidato->opcoes[l].ide) == curso_atual->id_inst)
                && (strcmp(inicio_node_ordenado_nota->candidato->opcoes[l].idc,curso_atual->id_curso)==0) && (curso_atual->vagas_sobram>0)) {

                    //preenche dados do candidato, na lista de colocados no curso respetivo
                    coloca_candi_nos_colocados (inicio_node_ordenado_nota, curso_atual, l);

                    curso_atual->vagas_sobram--; //vagas que sobram

                    break;
                }

                //se já tiverem "enchido" as vagas
                else if ((atoi(inicio_node_ordenado_nota->candidato->opcoes[l].ide) == curso_atual->id_inst)
                && (strcmp(inicio_node_ordenado_nota->candidato->opcoes[l].idc,curso_atual->id_curso)==0) &&
                (inicio_node_ordenado_nota->candidato->nota_candi == curso_atual->nota_minima) && (curso_atual->vagas_sobram == 0)){

                    //preenche dados do candidato, na lista de colocados no curso respetivo
                    coloca_candi_nos_colocados (inicio_node_ordenado_nota, curso_atual, l);

                    break;

                }
               curso_atual = curso_atual->prox_cursos; //para percorrer a lista de cursos
            }
        }
        inicio_node_ordenado_nota=inicio_node_ordenado_nota->next; //para percorrer a lista de candidatos ordenados por nota decrescentemente
    }
    return inicio_cursos;
}

//para escrever o ficheiro de output de colocações
void file_colocacoes(struct Cursos* inicio_cursos, char* file) {
    //abertura do ficheiro de colocação
    if(file==NULL){
        file="CNAES_Colocacoes.csv";
    }

    FILE* input;
    input = fopen(file, "w");
    if (input == NULL) {
        printf("Erro a abrir o ficheiro.\n");
        exit(1);
    }

    char cabecalho[]={"Candidato,Nota,Opçao,Instituição,Curso\n"};
    fputs(cabecalho,input);

    struct Cursos* curso_atual = inicio_cursos;
    while (curso_atual != NULL) { //loop que percorre os cursos

        struct node *colocados = curso_atual->candidatos_colocados;
        while (colocados != NULL) { //loop que percorre a lista de colocados nesse curso e que imprime os respetivos dados para o ficheiro de saida escolhido

            fprintf(input, "%d,%f,%d,%s,%s\n", colocados->candidato->id_candi, colocados->candidato->nota_candi,
            colocados->candidato->opcao_colocacao, curso_atual->nome_inst, curso_atual->nome_curso);
            colocados = colocados->next;
        }

        curso_atual = curso_atual->prox_cursos;
    }
    fclose(input);
}

//para escrever o ficheiro de output completo
void file_completo(struct Cursos* inicio_cursos, char* file) {
    //abertura do ficheiro de colocação
    if(file==NULL){
        file="CNAES_Completo.csv";
    }

    FILE* input;
    input = fopen(file, "w");
    if (input == NULL) {
        printf("Erro a abrir o ficheiro.\n");
        exit(1);
    }

    struct Cursos* curso_atual = inicio_cursos;
    while (curso_atual != NULL) { //loop que percorre a lista de cursos e que imprime os respetivos dados para o ficheiro de saida escolhido

        if(curso_atual->num_colocados < curso_atual->vagas_iniciais) {
            curso_atual->nota_minima = 10.000000; //caso as vagas não sejam preenchidas a nota minima é predefinida como 10
        }
        fprintf(input, "%s,%s,%d,%d,%f\n", curso_atual->nome_inst, curso_atual->nome_curso, curso_atual->vagas_iniciais,
        curso_atual->num_colocados, curso_atual->nota_minima);

        struct node *colocados = curso_atual->candidatos_colocados;
        while (colocados != NULL) { //loop que percorre a lista de colocados nesse curso e que imprime os respetivos dados para o ficheiro de saida escolhido
            fprintf(input, "%d,%f,%d,%s,%s\n", colocados->candidato->id_candi, colocados->candidato->nota_candi,
            colocados->candidato->opcao_colocacao, curso_atual->nome_inst, curso_atual->nome_curso);

            colocados = colocados->next;
        }

        curso_atual = curso_atual->prox_cursos;
    }
    fclose(input);
}

//para escrever o ficheiro de output dos cursos
void file_cursos(struct Cursos* inicio_cursos, char*file) {
    //abertura do ficheiro de colocação
    if(file==NULL){
        file="CNAES_Cursos.csv";
    }

    FILE* input;
    input = fopen(file, "w");
    if (input == NULL) {
        printf("Erro a abrir o ficheiro.\n");
        exit(1);
    }

    char cabecalho[]={"Instituição,Curso,Vagas,Colocacoes,Nota_Mínima\n"};
    fputs(cabecalho,input);

    struct Cursos* curso_atual = inicio_cursos;
    while (curso_atual != NULL) {
        fprintf(input, "%s,%s,%d,%d,%f\n", curso_atual->nome_inst, curso_atual->nome_curso, curso_atual->vagas_iniciais,
        curso_atual->num_colocados, curso_atual->nota_minima);

        curso_atual = curso_atual->prox_cursos;
    }
    fclose(input);
}

//para ordenar os candidatos por ordem de ID
struct node* ordenar_candidato_ID(struct node* inicio_node_ordenado_ID) {
    struct node *aqui, *prox;
    aqui = inicio_node_ordenado_ID;
    prox = aqui->next;
    int troca = 1;
    struct node* ultimo = NULL;

    while (troca == 1) {
        troca = 0;

        while (prox != ultimo) {
            if (aqui->candidato->id_candi > prox->candidato->id_candi) {
                trocar(aqui, prox);
                troca = 1;
            }
            aqui = aqui->next;
            prox = prox->next;
        }
        ultimo = aqui;
        aqui = inicio_node_ordenado_ID;
        prox = aqui->next;
    }

    return inicio_node_ordenado_ID;

}

//para escrever o ficheiro de output dos não colocados
void file_nao_colocados (struct node* inicio_node_ordenado_ID, char*file) {

    //abertura do ficheiro
   if(file==NULL){
        file="CNAES_NC.csv";
    }

    FILE* input;
    input = fopen(file, "w");
    if (input == NULL) {
        printf("Erro a abrir o ficheiro.\n");
        exit(1);
    }

    char cabecalho[]={"Candidato,Nota\n"};
    fputs(cabecalho,input);

    while(inicio_node_ordenado_ID != NULL) { //loop que percorre a lista de candidatos ordenados por ordem ascendente de ID

        if (inicio_node_ordenado_ID->candidato->colocacao == 0) { //imprime dados apenas dos nao colocados para o ficheiro de saida escolhido

            fprintf(input, "%d,%f\n", inicio_node_ordenado_ID->candidato->id_candi, inicio_node_ordenado_ID->candidato->nota_candi);
        }
        inicio_node_ordenado_ID = inicio_node_ordenado_ID->next;
    }

    fclose(input);
}

int main(int argc, char* argv[]) {

    //variáveis utilizadas para a invocação do programa através das linhas de comando
    int n;
    char* vagas_curso_comando=NULL;
    char* n_candidatos_comando=NULL;
    char* file_cursos_entrada=NULL;
    char* file_candidatos_entrada=NULL;
    char* file_colocacoes_saida=NULL;
    char* file_completo_saida=NULL;
    char* file_cursos_saida=NULL;
    char* file_nao_colocados_saida=NULL;

    while ((n = getopt(argc, argv, "v:n:i:c:o:u:m:x:h")) != -1 ) {
        switch(n) {

            case 'h': //comando de ajuda
            printf("-v: numero de vagas iniciais de cada curso\n"
            "-n: numero de candidatos a ser lidos do ficheiro de entrada de candidatos\n"
            "-i: nome do ficheiro de entrada com a lista de cursos\n"
            "-c: nome do ficheiro de entrada com a lista de candidatos\n"
            "-o: nome do ficheiro de saida com a lista de colocacoes\n"
            "-u: nome do ficheiro de saida com a lista de universidades e colocados\n"
            "-m: nome do ficheiro de saida com a lista dos cursos\n"
            "-x: nome do ficheiro de saida com a lista dos não colocados\n");
            break;

            case 'v':
            vagas_curso_comando=optarg;
            if(atoi(vagas_curso_comando)<0){ //o número de vagas não pode ser negativo
                printf("Erro: Vagas Invalidas\n");
                exit(1);
            }
            break;

            case 'n':
            n_candidatos_comando=optarg;
            if(atoi(n_candidatos_comando)<0){ //o número de candidatos não pode ser negativo
                printf("Erro: Candidatos Invalidos\n");
                exit(1);
            }
            break;

            case 'i':
            file_cursos_entrada=optarg;
            break;

            case 'c':
            file_candidatos_entrada=optarg;
            break;

            case 'o':
            file_colocacoes_saida=optarg;
            break;

            case 'u':
            file_completo_saida=optarg;
            break;

            case 'm':
            file_cursos_saida=optarg;
            break;

            case 'x':
            file_nao_colocados_saida=optarg;
            break;

        }
    }

    struct Cursos* inicio_cursos = og_cursos(file_cursos_entrada, vagas_curso_comando); //guarda os dados de input dos cursos

    struct node* inicio_node = og_candidatos(file_candidatos_entrada, n_candidatos_comando); //guarda os dados de input dos candidatos

    struct node* inicio_node_ordenado_nota = ordenar_candidato_nota(inicio_node); //ordena os candidatos por ordem decrescente de nota

    struct Cursos* inicio_cursos_colocados = colocacao_geral(inicio_node_ordenado_nota,inicio_cursos); //coloca cada candidato no respetivo curso

    struct node* inicio_node_ordenado_ID = ordenar_candidato_ID(inicio_node_ordenado_nota); //para ordenar os candidatos por ordem ascendente de ID

    file_colocacoes(inicio_cursos_colocados,file_colocacoes_saida); //ficheiro de output "CNAES_Colocacoes.csv"

    file_completo(inicio_cursos_colocados,file_completo_saida); //ficheiro de output "CNAES_Completo.csv"

    file_cursos(inicio_cursos_colocados,file_cursos_saida); //ficheiro de output "CNAES_Cursos.csv"

    file_nao_colocados(inicio_node_ordenado_ID, file_nao_colocados_saida); //ficheiro de output "CNAES_NC.csv"


    //Para dar free de toda a lista de cursos
    struct Cursos* temp_cursos;
    struct node* temp_candidatos_colocados;

    while (inicio_cursos !=NULL){
        temp_cursos = inicio_cursos;
        inicio_cursos = inicio_cursos->prox_cursos;
        free(temp_cursos->id_curso);
        free(temp_cursos->nome_curso);
        free(temp_cursos->nome_inst);

        //para dar free de cada lista de candidatos colocados
        temp_candidatos_colocados = temp_cursos->candidatos_colocados;
        while (temp_candidatos_colocados != NULL) {
            temp_cursos->candidatos_colocados = temp_cursos->candidatos_colocados->next;
            free(temp_candidatos_colocados->candidato);
            free(temp_candidatos_colocados);
            temp_candidatos_colocados = temp_cursos->candidatos_colocados;
        }
        free(temp_cursos);
    }

    //Para dar free de toda a lista de candidatos
    struct node* temp_node;
    while (inicio_node !=NULL){
        temp_node = inicio_node;
        inicio_node = inicio_node->next;
        free(temp_node->candidato->nota_ing);
        free(temp_node->candidato->media_sec);
        free(temp_node->candidato);
        free(temp_node);
    }

    return 0;
}
