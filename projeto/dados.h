/*
 * dados.h - Estruturas do sistema e acesso aos arquivos texto da pasta "dados".
 *
 * Cada arquivo guarda um registro por linha, com os campos separados por ';'
 *
 *   dados/servicos.txt      codigo;nome;valor
 *   dados/clientes.txt      cpf;nome;telefone
 *   dados/veiculos.txt      placa;tipo;marca;modelo;cor;cpfCliente
 *   dados/mecanicos.txt     cpf;nome
 *   dados/agendamentos.txt  codigo;data;hora;placa;cpfMecanico;qtd;serv1;serv2;...
 */
#ifndef DADOS_H
#define DADOS_H

#define PASTA_DADOS       "dados"
#define ARQ_SERVICOS      "dados/servicos.txt"
#define ARQ_CLIENTES      "dados/clientes.txt"
#define ARQ_VEICULOS      "dados/veiculos.txt"
#define ARQ_MECANICOS     "dados/mecanicos.txt"
#define ARQ_AGENDAMENTOS  "dados/agendamentos.txt"

#define MAX_REGISTROS     500   /* limite de registros lidos por arquivo   */
#define MAX_SERV_AGEND    10    /* serviços por agendamento                */

typedef struct {
    int   codigo;
    char  nome[50];
    float valor;
} Servico;

typedef struct {
    char cpf[12];          /* só os 11 dígitos */
    char nome[100];
    char telefone[20];
} Cliente;

typedef struct {
    char placa[8];         /* ABC1234 ou ABC1D23 */
    char tipo[6];          /* "Carro" ou "Moto"  */
    char marca[30];
    char modelo[30];
    char cor[20];
    char cpfCliente[12];
} Veiculo;

typedef struct {
    char cpf[12];
    char nome[100];
} Mecanico;

typedef struct {
    int  codigo;
    char data[11];         /* dd/mm/aaaa */
    char hora[6];          /* hh:mm      */
    char placa[8];
    char cpfMecanico[12];
    int  qtdServicos;
    int  servicos[MAX_SERV_AGEND];   /* códigos dos serviços escolhidos */
} Agendamento;

void inicializarDados(void);

/* Leitura: preenchem o vetor e devolvem a quantidade lida */
int carregarServicos(Servico *vetor, int max);
int carregarClientes(Cliente *vetor, int max);
int carregarVeiculos(Veiculo *vetor, int max);
int carregarMecanicos(Mecanico *vetor, int max);
int carregarAgendamentos(Agendamento *vetor, int max);

/* Gravação: acrescentam um registro no final do arquivo (1 = sucesso) */
int gravarServico(const Servico *s);
int gravarCliente(const Cliente *c);
int gravarVeiculo(const Veiculo *v);
int gravarMecanico(const Mecanico *m);
int gravarAgendamento(const Agendamento *a);

/* Buscas: devolvem 1 se encontrou e copiam o registro para *saida (se não for NULL) */
int buscarServicoPorCodigo(int codigo, Servico *saida);
int buscarServicoPorNome(const char *nome, Servico *saida);
int buscarClientePorCPF(const char *cpf, Cliente *saida);
int buscarVeiculoPorPlaca(const char *placa, Veiculo *saida);
int buscarMecanicoPorCPF(const char *cpf, Mecanico *saida);

int   proximoCodigoServico(void);
int   proximoCodigoAgendamento(void);
float valorTotalAgendamento(const Agendamento *a);

#endif
