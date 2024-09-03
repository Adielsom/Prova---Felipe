#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define max_Socios 50
#define max_Dependentes 5
#define max_Mensalidade 12
#define max_Categorias 10

typedef struct {
    char nome[100];
    char endereco[100];
    char bairro[50];
    long cep;
    char telefone[20];
    char email[100];
} Candidato;

typedef struct {
    long cartao;
    char nome[100];
    char parentesco[50];
    char[emal];
} Dependente;

typedef struct {
    char descricao[100];
} Categoria;

typedef struct {
    struct tm dataMensalidade;
    double valorMensalidade;
    struct tm dataPagamentoMen;
    double jurosMensalidade;
    double valorPagoMen;
    bool Menquitada;
} Mensalidade;

typede struct {
    long cartao;
    Candidato candidato;
    Dependente dependente[max_Dependentes];
    int numdependentes;
    Categoria categoria;
    Mensalidade mensalidade[max_Mensalidade];
    int nummensalidade;
} Socio;

