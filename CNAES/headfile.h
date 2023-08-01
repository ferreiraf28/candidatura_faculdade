#ifndef HEADFILE
#define HEADFILE

//estrutura das opções
    struct opcao {
            char ide[5]; //instituição de ensino
            char idc[5]; //identificador de curso
        };

//estrutura de identificação dos candidatos
    struct Candidato{
        int id_candi; //identificador de candidato
        char* nota_ing; //nota de ingresso
        char* media_sec; //media do secundário
        float nota_candi; //nota de candidatura
        struct opcao opcoes[5]; //opções do candidato (1-5)
        int colocacao; //avalia se o candidato está colocado ou não (0 ou 1)
        int opcao_colocacao; //opção em que o candidato foi colocado
        };

//lista de Candidatos
    struct node {
        struct Candidato* candidato;
        struct node* next; //apontador para o candidato seguinte
    };

//estrutura de identificação dos cursos
    struct Cursos{
        int id_inst; //identificador da instituição
        char* id_curso; //identificador de curso
        char* nome_inst; //nome da instituição
        char* nome_curso; //nome do curso
        char grau[3]; //grau de ensino
        int vagas_iniciais;
        int vagas_sobram;
        float nota_minima;
        struct node* candidatos_colocados; //lista dos candidatos colocados em cada curso
        int num_colocados; //numero de colocados em cada curso
        struct Cursos* prox_cursos; //apontador para o curso seguinte
    };

#endif // HEADFILE
