// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

// define os nos da lista
struct no{
    char * info;
    struct no *prox;
};
typedef struct no no;

// definicao da lista
struct lista {
    no *ini;
    int tam;
};
typedef struct lista lista;

// funcoes de lista a serem empregadas

void inicializa_lista(lista *l);
void destruir(lista *l);
void inserir(lista *l, char* x);