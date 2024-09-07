#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_socios 100
#define MAX_dependentes 10
#define MAX_categoria 3
#define MAX_candidatos 50

// Definições das categorias
#define MAX_bronze 3
#define MAX_prata 7
#define MAX_ouro 10

typedef struct {
    char descricao[50];
    int limiteDependentes;
    double valorMensalidade;
} Categoria;

typedef struct {
    long cartaoDependente;
    char nome[100];
    char parentesco[50];
    char email[100];
} Dependente;

typedef struct {
    char dataMensalidade[11]; // DD/MM/AAAA
    double valorMensalidade;
    char dataPagamento[11]; // DD/MM/AAAA
    double valorPago;
    double juros;
    int quitada; // 0 para não quitada, 1 para quitada
    char dataVencimento[11]; // DD/MM/AAAA
} Mensalidade;

typedef struct {
    long cartaoSocio;
    char nome[100];
    char endereco[100];
    char telefone[20];
    char email[100];
    Categoria categoria;
    Dependente dependentes[MAX_dependentes];
    int numDependentes;
    Mensalidade mensalidades[MAX_socios];
    int numMensalidades;
} Socio;

typedef struct {
    long cartaoCandidato;
    char nome[100];
    char endereco[100];
    char telefone[20];
    char email[100];
    Categoria categoria;
} Candidato;

// Função para converter data de string para struct tm
void converterData(const char *dataStr, struct tm *data) {
    sscanf(dataStr, "%d/%d/%d", &data->tm_mday, &data->tm_mon, &data->tm_year);
    data->tm_mon -= 1; // struct tm usa meses de 0 a 11
    data->tm_year -= 1900; // struct tm usa o ano desde 1900
}

// Função para calcular o número de dias de atraso
int calAtraso(const char *dataMensalidade, const char *dataPagamento) {
    struct tm tmMensalidade = {0}, tmPagamento = {0};
    time_t tMensalidade, tPagamento;
    double diff;
    int dias;

    converterData(dataMensalidade, &tmMensalidade);
    converterData(dataPagamento, &tmPagamento);

    tMensalidade = mktime(&tmMensalidade);
    tPagamento = mktime(&tmPagamento);

    diff = difftime(tPagamento, tMensalidade);
    dias = diff / (60 * 60 * 24);

    return (dias > 0) ? dias : 0;
}

// Função para calcular juros
void calJuros(Mensalidade *mensalidade) {
    int diasDeAtraso = calAtraso(mensalidade->dataVencimento, mensalidade->dataPagamento);
    if (diasDeAtraso > 0) {
        mensalidade->juros = 0.02 * diasDeAtraso; // 2% de juros por dia de atraso
    } else {
        mensalidade->juros = 0.0;
    }
}

// Função para definir a categoria do sócio
void defCategoria(Socio *socio) {
    if (strcmp(socio->categoria.descricao, "bronze") == 0) {
        socio->categoria.limiteDependentes = MAX_bronze;
        socio->categoria.valorMensalidade = 300.0;
    } else if (strcmp(socio->categoria.descricao, "prata") == 0) {
        socio->categoria.limiteDependentes = MAX_prata;
        socio->categoria.valorMensalidade = 500.0;
    } else if (strcmp(socio->categoria.descricao, "ouro") == 0) {
        socio->categoria.limiteDependentes = MAX_ouro;
        socio->categoria.valorMensalidade = 1000.0;
    } else {
        socio->categoria.limiteDependentes = MAX_dependentes;
        socio->categoria.valorMensalidade = 0.0;
    }
}

// Função para registrar um sócio
void registrarSocio(Socio *socio) {
    printf("Digite o número do cartão do sócio: ");
    scanf("%ld", &socio->cartaoSocio);
    getchar();
    printf("Digite o nome do sócio: ");
    fgets(socio->nome, 100, stdin);
    socio->nome[strcspn(socio->nome, "\n")] = '\0';
    printf("Digite o endereço do sócio: ");
    fgets(socio->endereco, 100, stdin);
    socio->endereco[strcspn(socio->endereco, "\n")] = '\0';
    printf("Digite o telefone do sócio: ");
    fgets(socio->telefone, 20, stdin);
    socio->telefone[strcspn(socio->telefone, "\n")] = '\0';
    printf("Digite o e-mail do sócio: ");
    fgets(socio->email, 100, stdin);
    socio->email[strcspn(socio->email, "\n")] = '\0';

    printf("Digite categoria (bronze, prata, ouro): ");
    fgets(socio->categoria.descricao, 50, stdin);
    socio->categoria.descricao[strcspn(socio->categoria.descricao, "\n")] = '\0';
    
    defCategoria(socio);
    
    socio->numDependentes = 0;
    socio->numMensalidades = 0;
}

// Função para listar todos os sócios
void listarSocios(const Socio *socios, int numSocios) {
    for (int i = 0; i < numSocios; i++) {
        printf("\nSócio %d:\n", i + 1);
        printf("Cartão do sócio: %ld\n", socios[i].cartaoSocio);
        printf("Nome: %s\n", socios[i].nome);
        printf("Endereço: %s\n", socios[i].endereco);
        printf("Telefone: %s\n", socios[i].telefone);
        printf("E-mail: %s\n", socios[i].email);
        printf("Categoria: %s\n", socios[i].categoria.descricao);
    }
}

// Função para adicionar dependente a um sócio
void addDependente(Socio *socio) {
    if (socio->numDependentes < socio->categoria.limiteDependentes) {
        Dependente *dependente = &socio->dependentes[socio->numDependentes];
        printf("Digite o número do cartão do dependente: ");
        scanf("%ld", &dependente->cartaoDependente);
        getchar();
        printf("Digite o nome do dependente: ");
        fgets(dependente->nome, 100, stdin);
        dependente->nome[strcspn(dependente->nome, "\n")] = '\0';
        printf("Digite o parentesco do dependente: ");
        fgets(dependente->parentesco, 50, stdin);
        dependente->parentesco[strcspn(dependente->parentesco, "\n")] = '\0';
        printf("Digite o e-mail do dependente: ");
        fgets(dependente->email, 100, stdin);
        dependente->email[strcspn(dependente->email, "\n")] = '\0';

        socio->numDependentes++;
    } else {
        printf("Erro, numero maximo de depedentes atingido.\n");
    }
}

// Função para registrar uma mensalidade para um sócio
void regMensalidade(Socio *socio) {
    if (socio->numMensalidades < MAX_socios) {
        Mensalidade *mensalidade = &socio->mensalidades[socio->numMensalidades];
        printf("Digite a data da mensalidade (DD.MM.AAAA): ");
        fgets(mensalidade->dataMensalidade, 11, stdin);
        mensalidade->dataMensalidade[strcspn(mensalidade->dataMensalidade, "\n")] = '\0';
        mensalidade->valorMensalidade = socio->categoria.valorMensalidade;
        printf("Digite a data de pagamento (DD/MM/AAAA): ");
        fgets(mensalidade->dataPagamento, 11, stdin);
        mensalidade->dataPagamento[strcspn(mensalidade->dataPagamento, "\n")] = '\0';
        printf("Digite o valor pago: ");
        scanf("%lf", &mensalidade->valorPago);
        getchar();
        printf("A mensalidade está quitada? (0 - Não, 1 - Sim): ");
        scanf("%d", &mensalidade->quitada);
        getchar();

        // Calcular juros, se necessário
        if (!mensalidade->quitada) {
            calJuros(mensalidade);
        } else {
            mensalidade->juros = 0.0;
        }

        printf("Digite a data de vencimento (DD.MM.AAAA): ");
        fgets(mensalidade->dataVencimento, 11, stdin);
        mensalidade->dataVencimento[strcspn(mensalidade->dataVencimento, "\n")] = '\0';

        socio->numMensalidades++;
    } else {
        printf("Erro: número máximo de mensalidades atingido.\n");
    }
}

// Função para listar mensalidades de um sócio
void listarMensalidades(const Socio *socio) {
    for (int i = 0; i < socio->numMensalidades; i++) {
        const Mensalidade *mensalidade = &socio->mensalidades[i];
        printf("\nMensalidade %d:\n", i + 1);
        printf("Data da mensalidade: %s\n", mensalidade->dataMensalidade);
        printf("Valor da mensalidade: %.2lf\n", mensalidade->valorMensalidade);
        printf("Data de pagamento: %s\n", mensalidade->dataPagamento);
        printf("Valor pago: %.2lf\n", mensalidade->valorPago);
        printf("Juros: %.2lf\n", mensalidade->juros);
        printf("Quitada: %s\n", mensalidade->quitada ? "Sim" : "Não");
        printf("Data de vencimento: %s\n", mensalidade->dataVencimento);
    }
}

// Função para listar dependentes de um sócio
void listarDependentes(const Socio *socio) {
    for (int i = 0; i < socio->numDependentes; i++) {
        const Dependente *dependente = &socio->dependentes[i];
        printf("\nDependente %d:\n", i + 1);
        printf("Número do cartão: %ld\n", dependente->cartaoDependente);
        printf("Nome: %s\n", dependente->nome);
        printf("Parentesco: %s\n", dependente->parentesco);
        printf("E-mail: %s\n", dependente->email);
    }
}

int main() {
    Socio socios[MAX_socios];
    Candidato candidatos[MAX_candidatos];
    int numSocios = 0, numCandidatos = 0;
    int opcao;

    do {
        printf("\n--------Menu-------\n");
        printf("1. Registrar sócio\n");
        printf("2. consultar sócios\n");
        printf("3. Adicionar dependente\n");
        printf("4. Registrar mensalidade\n");
        printf("5. consultar mensalidades\n");
        printf("6. consultar dependentes\n");
        printf("7. Sair so site!\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpar o buffer do stdin

        switch (opcao) {
            case 1:
                if (numSocios < MAX_socios) {
                    registrarSocio(&socios[numSocios]);
                    numSocios++;
                } else {
                    printf("Número máximo de sócios atingido.\n");
                }
                break;
            case 2:
                listarSocios(socios, numSocios);
                break;
            case 3:
                if (numSocios > 0) {
                    int idSocio;
                    printf("Digite o número do cartão do sócio: ");
                    scanf("%d", &idSocio);
                    getchar();
                    for (int i = 0; i < numSocios; i++) {
                        if (socios[i].cartaoSocio == idSocio) {
                            addDependente(&socios[i]);
                            break;
                        }
                    }
                } else {
                    printf("Nenhum sócio registrado.\n");
                }
                break;
            case 4:
                if (numSocios > 0) {
                    int idSocio;
                    printf("Digite o número do cartão do sócio: ");
                    scanf("%d", &idSocio);
                    getchar();
                    for (int i = 0; i < numSocios; i++) {
                        if (socios[i].cartaoSocio == idSocio) {
                            regMensalidade(&socios[i]);
                            break;
                        }
                    }
                } else {
                    printf("Nenhum sócio registrado.\n");
                }
                break;
            case 5:
                if (numSocios > 0) {
                    int idSocio;
                    printf("Digite o número do cartão do sócio: ");
                    scanf("%d", &idSocio);
                    getchar();
                    for (int i = 0; i < numSocios; i++) {
                        if (socios[i].cartaoSocio == idSocio) {
                            listarMensalidades(&socios[i]);
                            break;
                        }
                    }
                } else {
                    printf("Nenhum sócio registrado.\n");
                }
                break;
            case 6:
                if (numSocios > 0) {
                    int idSocio;
                    printf("Digite o número do cartão do sócio: ");
                    scanf("%d", &idSocio);
                    getchar();
                    for (int i = 0; i < numSocios; i++) {
                        if (socios[i].cartaoSocio == idSocio) {
                            listarDependentes(&socios[i]);
                            break;
                        }
                    }
                } else {
                    printf("Nenhum sócio registrado.\n");
                }
                break;
            case 7:
                printf("programa encerrado!\n");
                break;
            default:
                printf("invalida.\n");
                break;
        }
    } while (opcao != 7);

    return 0;
}

