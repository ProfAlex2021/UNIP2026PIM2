/*
 * dados.c - Leitura e gravação dos arquivos texto (repositório de dados).
 *
 * Padrão usado em todas as funções:
 *   leitura  -> fopen("r"), fgets linha a linha, sscanf separando por ';'
 *   gravação -> fopen("a"), fprintf no final do arquivo
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados.h"
#include "tela.h"

/* Cria a pasta "dados" e a lista inicial de serviços, se ainda não existirem */
void inicializarDados(void)
{
    FILE *arq;

    system("if not exist " PASTA_DADOS " mkdir " PASTA_DADOS);

    arq = fopen(ARQ_SERVICOS, "r");
    if (arq != NULL) {
        fclose(arq);
        return;
    }

    arq = fopen(ARQ_SERVICOS, "w");
    if (arq == NULL) {
        printf("Erro: não foi possível criar %s\n", ARQ_SERVICOS);
        pausar();
        return;
    }
    fprintf(arq, "1;Revisão;350.00\n");
    fprintf(arq, "2;Troca de óleo;180.00\n");
    fprintf(arq, "3;Alinhamento;120.00\n");
    fprintf(arq, "4;Balanceamento;100.00\n");
    fclose(arq);
}

/* ---------- Leitura ---------- */

int carregarServicos(Servico *vetor, int max)
{
    FILE *arq = fopen(ARQ_SERVICOS, "r");
    char linha[300];
    int qtd = 0;

    if (arq == NULL) {
        return 0;
    }
    while (qtd < max && fgets(linha, sizeof(linha), arq) != NULL) {
        Servico *s = &vetor[qtd];
        if (sscanf(linha, "%d;%49[^;];%f", &s->codigo, s->nome, &s->valor) == 3) {
            qtd++;
        }
    }
    fclose(arq);
    return qtd;
}

int carregarClientes(Cliente *vetor, int max)
{
    FILE *arq = fopen(ARQ_CLIENTES, "r");
    char linha[300];
    int qtd = 0;

    if (arq == NULL) {
        return 0;
    }
    while (qtd < max && fgets(linha, sizeof(linha), arq) != NULL) {
        Cliente *c = &vetor[qtd];
        if (sscanf(linha, "%11[^;];%99[^;];%19[^;\n]", c->cpf, c->nome, c->telefone) == 3) {
            qtd++;
        }
    }
    fclose(arq);
    return qtd;
}

int carregarVeiculos(Veiculo *vetor, int max)
{
    FILE *arq = fopen(ARQ_VEICULOS, "r");
    char linha[300];
    int qtd = 0;

    if (arq == NULL) {
        return 0;
    }
    while (qtd < max && fgets(linha, sizeof(linha), arq) != NULL) {
        Veiculo *v = &vetor[qtd];
        if (sscanf(linha, "%7[^;];%5[^;];%29[^;];%29[^;];%19[^;];%11[^;\n]",
                   v->placa, v->tipo, v->marca, v->modelo, v->cor, v->cpfCliente) == 6) {
            qtd++;
        }
    }
    fclose(arq);
    return qtd;
}

int carregarMecanicos(Mecanico *vetor, int max)
{
    FILE *arq = fopen(ARQ_MECANICOS, "r");
    char linha[300];
    int qtd = 0;

    if (arq == NULL) {
        return 0;
    }
    while (qtd < max && fgets(linha, sizeof(linha), arq) != NULL) {
        Mecanico *m = &vetor[qtd];
        if (sscanf(linha, "%11[^;];%99[^;\n]", m->cpf, m->nome) == 2) {
            qtd++;
        }
    }
    fclose(arq);
    return qtd;
}

/*
 * A linha do agendamento tem uma quantidade variável de serviços no final.
 * %n informa quantos caracteres o sscanf já leu, para continuarmos dali.
 */
int carregarAgendamentos(Agendamento *vetor, int max)
{
    FILE *arq = fopen(ARQ_AGENDAMENTOS, "r");
    char linha[300];
    char *resto;
    int qtd = 0, lidos, i;

    if (arq == NULL) {
        return 0;
    }
    while (qtd < max && fgets(linha, sizeof(linha), arq) != NULL) {
        Agendamento *a = &vetor[qtd];
        if (sscanf(linha, "%d;%10[^;];%5[^;];%7[^;];%11[^;];%d%n",
                   &a->codigo, a->data, a->hora, a->placa, a->cpfMecanico,
                   &a->qtdServicos, &lidos) != 6) {
            continue;
        }
        if (a->qtdServicos < 0 || a->qtdServicos > MAX_SERV_AGEND) {
            continue;
        }
        resto = linha + lidos;
        for (i = 0; i < a->qtdServicos; i++) {
            if (sscanf(resto, ";%d%n", &a->servicos[i], &lidos) != 1) {
                break;
            }
            resto += lidos;
        }
        a->qtdServicos = i;
        qtd++;
    }
    fclose(arq);
    return qtd;
}

/* ---------- Gravação ---------- */

int gravarServico(const Servico *s)
{
    FILE *arq = fopen(ARQ_SERVICOS, "a");
    if (arq == NULL) {
        return 0;
    }
    fprintf(arq, "%d;%s;%.2f\n", s->codigo, s->nome, s->valor);
    fclose(arq);
    return 1;
}

int gravarCliente(const Cliente *c)
{
    FILE *arq = fopen(ARQ_CLIENTES, "a");
    if (arq == NULL) {
        return 0;
    }
    fprintf(arq, "%s;%s;%s\n", c->cpf, c->nome, c->telefone);
    fclose(arq);
    return 1;
}

int gravarVeiculo(const Veiculo *v)
{
    FILE *arq = fopen(ARQ_VEICULOS, "a");
    if (arq == NULL) {
        return 0;
    }
    fprintf(arq, "%s;%s;%s;%s;%s;%s\n",
            v->placa, v->tipo, v->marca, v->modelo, v->cor, v->cpfCliente);
    fclose(arq);
    return 1;
}

int gravarMecanico(const Mecanico *m)
{
    FILE *arq = fopen(ARQ_MECANICOS, "a");
    if (arq == NULL) {
        return 0;
    }
    fprintf(arq, "%s;%s\n", m->cpf, m->nome);
    fclose(arq);
    return 1;
}

int gravarAgendamento(const Agendamento *a)
{
    FILE *arq = fopen(ARQ_AGENDAMENTOS, "a");
    int i;

    if (arq == NULL) {
        return 0;
    }
    fprintf(arq, "%d;%s;%s;%s;%s;%d",
            a->codigo, a->data, a->hora, a->placa, a->cpfMecanico, a->qtdServicos);
    for (i = 0; i < a->qtdServicos; i++) {
        fprintf(arq, ";%d", a->servicos[i]);
    }
    fprintf(arq, "\n");
    fclose(arq);
    return 1;
}

/* ---------- Buscas ---------- */

int buscarServicoPorCodigo(int codigo, Servico *saida)
{
    Servico lista[MAX_REGISTROS];
    int qtd = carregarServicos(lista, MAX_REGISTROS);
    int i;

    for (i = 0; i < qtd; i++) {
        if (lista[i].codigo == codigo) {
            if (saida != NULL) {
                *saida = lista[i];
            }
            return 1;
        }
    }
    return 0;
}

/* Nome exato, sem diferenciar maiúsculas (usado para evitar duplicados) */
int buscarServicoPorNome(const char *nome, Servico *saida)
{
    Servico lista[MAX_REGISTROS];
    int qtd = carregarServicos(lista, MAX_REGISTROS);
    int i;

    for (i = 0; i < qtd; i++) {
        if (strlen(lista[i].nome) == strlen(nome) && contemTexto(lista[i].nome, nome)) {
            if (saida != NULL) {
                *saida = lista[i];
            }
            return 1;
        }
    }
    return 0;
}

int buscarClientePorCPF(const char *cpf, Cliente *saida)
{
    Cliente lista[MAX_REGISTROS];
    int qtd = carregarClientes(lista, MAX_REGISTROS);
    int i;

    for (i = 0; i < qtd; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
            if (saida != NULL) {
                *saida = lista[i];
            }
            return 1;
        }
    }
    return 0;
}

int buscarVeiculoPorPlaca(const char *placa, Veiculo *saida)
{
    Veiculo lista[MAX_REGISTROS];
    int qtd = carregarVeiculos(lista, MAX_REGISTROS);
    int i;

    for (i = 0; i < qtd; i++) {
        if (strcmp(lista[i].placa, placa) == 0) {
            if (saida != NULL) {
                *saida = lista[i];
            }
            return 1;
        }
    }
    return 0;
}

int buscarMecanicoPorCPF(const char *cpf, Mecanico *saida)
{
    Mecanico lista[MAX_REGISTROS];
    int qtd = carregarMecanicos(lista, MAX_REGISTROS);
    int i;

    for (i = 0; i < qtd; i++) {
        if (strcmp(lista[i].cpf, cpf) == 0) {
            if (saida != NULL) {
                *saida = lista[i];
            }
            return 1;
        }
    }
    return 0;
}

/* ---------- Códigos e valores ---------- */

int proximoCodigoServico(void)
{
    Servico lista[MAX_REGISTROS];
    int qtd = carregarServicos(lista, MAX_REGISTROS);
    int i, maior = 0;

    for (i = 0; i < qtd; i++) {
        if (lista[i].codigo > maior) {
            maior = lista[i].codigo;
        }
    }
    return maior + 1;
}

int proximoCodigoAgendamento(void)
{
    Agendamento lista[MAX_REGISTROS];
    int qtd = carregarAgendamentos(lista, MAX_REGISTROS);
    int i, maior = 0;

    for (i = 0; i < qtd; i++) {
        if (lista[i].codigo > maior) {
            maior = lista[i].codigo;
        }
    }
    return maior + 1;
}

/* Soma os valores dos serviços do agendamento (valores atuais do cadastro) */
float valorTotalAgendamento(const Agendamento *a)
{
    Servico lista[MAX_REGISTROS];
    int qtd = carregarServicos(lista, MAX_REGISTROS);
    float total = 0;
    int i, j;

    for (i = 0; i < a->qtdServicos; i++) {
        for (j = 0; j < qtd; j++) {
            if (lista[j].codigo == a->servicos[i]) {
                total += lista[j].valor;
            }
        }
    }
    return total;
}
