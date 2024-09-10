#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis globais relacionadas ao número máximo de sócios, dependentes, candidatos e categorias
#define MAX_socios 100
#define MAX_dependentes 10
#define MAX_categoria 3
#define MAX_candidatos 50

// Limites para o número máximo de dependentes permitidos por categoria
#define MAX_bronze 3
#define MAX_prata 7
#define MAX_ouro 10

// Estrutura que define uma Categoria com limite de dependentes e valor da mensalidade
typedef struct {
    char descricao[50];
    int limiteDependentes;
    double valorMensalidade;
} Categoria;

// Estrutura para armazenar dados de um dependente
typedef struct {
    long cartaoDependente;
    char nome[100];
    char parentesco[50];
    char email[100];
} Dependente;

// Estrutura para armazenar informações de uma mensalidade
typedef struct {
    char dataMensalidade[11]; // DD/MM/AAAA
    double valorMensalidade;
    char dataPagamento[11]; // DD/MM/AAAA
    double valorPago;
    double juros;
    int quitada; 
    char dataVencimento[11]; // DD/MM/AAAA
} Mensalidade;


// Estrutura para armazenar dados de um sócio, incluindo dependentes e mensalidades
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

// Estrutura para armazenar dados de um candidato
typedef struct {
    long cartaoCandidato;
    char nome[100];
    char endereco[100];
    char telefone[20];
    char email[100];
    Categoria categoria;
} Candidato;

// Função que converte uma string de data no formato DD/MM/AAAA para uma struct tm
void converterData(const char *dataStr, struct tm *data) {
    sscanf(dataStr, "%d/%d/%d", &data->tm_mday, &data->tm_mon, &data->tm_year);
    data->tm_mon -= 1; // struct tm usa meses de 0 a 11
    data->tm_year -= 1900; // struct tm usa o ano desde 1900
}


// Função que calcula a quantidade de dias de atraso entre a data da mensalidade e a data de pagamento
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

// Função que calcula os juros de uma mensalidade com base nos dias de atraso
void calcularJuros(Mensalidade *mensalidade) {
    int diasDeAtraso = calAtraso(mensalidade->dataVencimento, mensalidade->dataPagamento);
    if (diasDeAtraso > 0) {
        mensalidade->juros = 0.02 * diasDeAtraso * mensalidade->valorMensalidade; // 2% de juros por dia de atraso
    } else {
        mensalidade->juros = 0.0;
    }
}

// Função que define a categoria do sócio com base na descrição fornecida
void definirCategoria(Socio *socio) {
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

// Função que registra um novo sócio, coletando dados do usuário
void registrarSocio(Socio *socio) {
    printf("Digite o numero do cartão do socio: ");
    scanf("%ld", &socio->cartaoSocio);
    getchar();
    printf("Digite o nome do socio: ");
    fgets(socio->nome, 100, stdin);
    socio->nome[strcspn(socio->nome, "\n")] = '\0';
    printf("Digite o endereço do socio: ");
    fgets(socio->endereco, 100, stdin);
    socio->endereco[strcspn(socio->endereco, "\n")] = '\0';
    printf("Digite o telefone do socio: ");
    fgets(socio->telefone, 20, stdin);
    socio->telefone[strcspn(socio->telefone, "\n")] = '\0';
    printf("Digite o e-mail do socio: ");
    fgets(socio->email, 100, stdin);
    socio->email[strcspn(socio->email, "\n")] = '\0';
// Define a categoria do sócio
    printf("Digite categoria (bronze, prata, ouro): ");
    fgets(socio->categoria.descricao, 50, stdin);
    socio->categoria.descricao[strcspn(socio->categoria.descricao, "\n")] = '\0';
    
    definirCategoria(socio);
    
    socio->numDependentes = 0;
    socio->numMensalidades = 0;
}

// Função que consulta todos os sócios registrados
void consultarSocios(const Socio *socios, int numSocios) {
    for (int i = 0; i < numSocios; i++) {
        printf("\nSocio %d:\n", i + 1);
        printf("Cartão do socio: %ld\n", socios[i].cartaoSocio);
        printf("Nome: %s\n", socios[i].nome);
        printf("Endereço: %s\n", socios[i].endereco);
        printf("Telefone: %s\n", socios[i].telefone);
        printf("E-mail: %s\n", socios[i].email);
        printf("Categoria: %s\n", socios[i].categoria.descricao);
    }
}

//Essa função registra um dependente para um sócio, garantindo que o limite de dependentes da categoria seja respeitado.
void registrarDependente(Socio *socio) {
    if (socio->numDependentes < socio->categoria.limiteDependentes) {
        Dependente *dependente = &socio->dependentes[socio->numDependentes];
        printf("Digite o numero do cartão do dependente: ");
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
        printf("Erro, numero maximo de dependentes atingido.\n");
    }
}

//Essa função exibe os detalhes de todos os dependentes registrados de um sócio.
void consultarDependentes(const Socio *socio) {
    for (int i = 0; i < socio->numDependentes; i++) {
        const Dependente *dependente = &socio->dependentes[i];
        printf("\nDependente %d:\n", i + 1);
        printf("Numero do cartão: %ld\n", dependente->cartaoDependente);
        printf("Nome: %s\n", dependente->nome);
        printf("Parentesco: %s\n", dependente->parentesco);
        printf("E-mail: %s\n", dependente->email);
    }
}

//Essa função coleta os dados de um candidato e o registra no sistema, com base no limite de candidatos.
void registrarCandidato(Candidato *candidato, int *numCandidatos) {
    if (*numCandidatos < MAX_candidatos) {
        printf("Digite o numero do cartão do candidato: ");
        scanf("%ld", &candidato[*numCandidatos].cartaoCandidato);
        getchar();
        printf("Digite o nome do candidato: ");
        fgets(candidato[*numCandidatos].nome, 100, stdin);
        candidato[*numCandidatos].nome[strcspn(candidato[*numCandidatos].nome, "\n")] = '\0';
        printf("Digite o endereço do candidato: ");
        fgets(candidato[*numCandidatos].endereco, 100, stdin);
        candidato[*numCandidatos].endereco[strcspn(candidato[*numCandidatos].endereco, "\n")] = '\0';
        printf("Digite o telefone do candidato: ");
        fgets(candidato[*numCandidatos].telefone, 20, stdin);
        candidato[*numCandidatos].telefone[strcspn(candidato[*numCandidatos].telefone, "\n")] = '\0';
        printf("Digite o e-mail do candidato: ");
        fgets(candidato[*numCandidatos].email, 100, stdin);
        candidato[*numCandidatos].email[strcspn(candidato[*numCandidatos].email, "\n")] = '\0';

        printf("Digite a categoria (bronze, prata, ouro): ");
        fgets(candidato[*numCandidatos].categoria.descricao, 50, stdin);
        candidato[*numCandidatos].categoria.descricao[strcspn(candidato[*numCandidatos].categoria.descricao, "\n")] = '\0';

        definirCategoria((Socio *)&candidato[*numCandidatos]);
        (*numCandidatos)++;
    } else {
        printf("Erro, numero maximo de candidatos atingido.\n");
    }
}


 //Essa função registra uma nova mensalidade para um sócio, calculando os juros e verificando se a mensalidade foi quitada.

void registrarMensalidade(Socio *socio) {
    if (socio->numMensalidades < MAX_socios) {
        Mensalidade *mensalidade = &socio->mensalidades[socio->numMensalidades];
        
        printf("Digite a data da mensalidade (DD/MM/AAAA): ");
        scanf("%10s", mensalidade->dataMensalidade);
        
        printf("Digite o valor da mensalidade: ");
        scanf("%lf", &mensalidade->valorMensalidade);
        
        printf("Digite a data de vencimento (DD/MM/AAAA): ");
        scanf("%10s", mensalidade->dataVencimento);
        
        printf("Digite a data de pagamento (DD/MM/AAAA): ");
        scanf("%10s", mensalidade->dataPagamento);
        
        printf("Digite o valor pago: ");
        scanf("%lf", &mensalidade->valorPago);

        calcularJuros(mensalidade);
        mensalidade->quitada = (mensalidade->valorPago >= mensalidade->valorMensalidade + mensalidade->juros) ? 1 : 0;

        socio->numMensalidades++;
    } else {
        printf("Erro, numero maximo de mensalidades atingido.\n");
    }
}


//Essa função exibe todas as mensalidades registradas de um sócio, com informações detalhadas.
void consultarMensalidades(const Socio *socio) {
    for (int i = 0; i < socio->numMensalidades; i++) {
        const Mensalidade *mensalidade = &socio->mensalidades[i];
        printf("\nMensalidade %d:\n", i + 1);
        printf("Data da mensalidade: %s\n", mensalidade->dataMensalidade);
        printf("Valor da mensalidade: %.2f\n", mensalidade->valorMensalidade);
        printf("Data de vencimento: %s\n", mensalidade->dataVencimento);
        printf("Data de pagamento: %s\n", mensalidade->dataPagamento);
        printf("Valor pago: %.2f\n", mensalidade->valorPago);
        printf("Juros: %.2f\n", mensalidade->juros);
        printf("Quitada: %s\n", mensalidade->quitada ? "Sim" : "Não");
    }
}

int main() {
    Socio socios[MAX_socios];
    Candidato candidatos[MAX_candidatos];
    int numSocios = 0;
    int numCandidatos = 0;

    int opcao;
    do {
        printf("=========MENU PRINCIPAL=========");
        printf("\n1.      REGISTRAR SOCIOS\n");
        printf("2.      CONSULTAR SOCIOS\n");
        printf("3.   REGISTRAR DEPENDENTES\n");
        printf("4.   CONSULTAR DEPENDENTES\n");
        printf("5.   REGISTRAR MENSALIDADE\n");
        printf("6.   CONSULTAR MENSALIDADE\n");
        printf("7.    REGISTRAR CANDIDATO\n");
        printf("0.        FINALIZAR!\n");
        printf("       ESCOLHA UMA OPÇÃO: ");
        scanf("%d", &opcao);
        getchar();// Remove o '\n' deixado pelo scanf

        switch (opcao) {
            case 1:
                registrarSocio(&socios[numSocios]);// Registra um novo sócio
                numSocios++;// Incrementa o número de sócios
                break;
            case 2:
                consultarSocios(socios, numSocios);// Exibe a lista de sócios registrados
                break;
            case 3: {
                long cartaoSocio;// Caso o usuário queira registrar dependentes
                printf("Digite o cartão do socio: ");
                scanf("%ld", &cartaoSocio);
                getchar();
                // Busca o sócio correspondente ao número do cartão
                for (int i = 0; i < numSocios; i++) {
                    if (socios[i].cartaoSocio == cartaoSocio) {
                        registrarDependente(&socios[i]);// Adiciona o dependente ao sócio
                        break;
                    }
                }
                break;
            }
            case 4: {
                long cartaoSocio;// Caso o usuário queira consultar os dependentes
                printf("Digite o cartão do socio: ");
                scanf("%ld", &cartaoSocio);
                getchar();
               // Busca o sócio correspondente ao número do cartão
                for (int i = 0; i < numSocios; i++) {
                    if (socios[i].cartaoSocio == cartaoSocio) {
                        consultarDependentes(&socios[i]);// Exibe os dependentes do sócio
                        break;
                    }
                }
                break;
            }
            case 5: {
                long cartaoSocio;// Caso o usuário queira registrar uma mensalidade
                printf("Digite o cartão do socio: ");
                scanf("%ld", &cartaoSocio);
                getchar();
              // Busca o sócio correspondente ao número do cartão
                for (int i = 0; i < numSocios; i++) {
                    if (socios[i].cartaoSocio == cartaoSocio) {
                        registrarMensalidade(&socios[i]);// Registra a mensalidade para o sócio
                        break;
                    }
                }
                break;
            }
            case 6: {
                long cartaoSocio; // Caso o usuário queira consultar as mensalidades
                printf("Digite o cartão do socio: ");
                scanf("%ld", &cartaoSocio);
                getchar();
                // Busca o sócio correspondente ao número do cartão
                for (int i = 0; i < numSocios; i++) {
                    if (socios[i].cartaoSocio == cartaoSocio) {
                        consultarMensalidades(&socios[i]);// Exibe as mensalidades do sócio
                        break;
                    }
                }
                break;
            }
            case 7:
                registrarCandidato(candidatos, &numCandidatos);// Registra um novo candidato
                break;
        }
    } while (opcao != 0);// O loop continua até que o usuário escolha a opção para finalizar

    return 0;
}
