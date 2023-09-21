// Guilherme Lorete Schmidt - 13676857
// Emanuel Percinio Goncalves de Oliveira - 13676878

// tamanhos estaticos para cabecalho e registro
#define TAM_HEADER 13
#define TAM_REG 76

// registros 'headers' com 13 bytes, com campos especificados
struct header {
    char status;
    int proxRRN;
    int nroTecnologias;
    int nroParesTecnologias;
};

// registros de 76 bytes, com campos especificados
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

// typedef dos structs criados
typedef struct header header;
typedef struct registro registro;

// funcoes de manipulacao de registros
int escrever_registro(FILE* arquivo, registro reg);
int ler_registro(FILE* arquivo, registro* reg);
int ler_campo(FILE* arquivo, char* valCampo, char* nomeCampo);
int escrever_header(FILE* arquivo, header cabecalho);
int ler_header(FILE* arquivo, header* cabecalho);
int calcula_byte_off(int RRN);