#define TAM_HEADER 13
#define TAM_REG 76

// registros 'headers' com 13 bytes
struct header {
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
};

// registros de 76 bytes
struct registro {
    char removido;
    int grupo;
    int popularidade;
    int peso;
    int tamanhoTecnologiaOrigem;
    char* nomeTecnologiaOrigem;
    int tamanhoTecnologiaDestino;
    char* nomeTecnologiaDestino;
};

typedef struct header header;
typedef struct registro registro;

void escrever_registro(FILE* arquivo, registro reg);
int ler_registro(FILE* arquivo, registro* reg);
void escrever_header(FILE* arquivo, header cabecalho);