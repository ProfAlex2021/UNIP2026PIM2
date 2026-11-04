/*
 * consulta.c - Opção 2 do menu principal: Consulta
 *
 *   1 - Serviço por nome
 *   2 - Cliente por nome ou CPF
 *   3 - Veículo por placa ou CPF do cliente
 *   4 - Mecânico por nome ou CPF
 *   5 - Agendamentos por data
 *
 * As buscas por nome aceitam parte do nome e ignoram maiúsculas/minúsculas.
 */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "consulta.h"
#include "dados.h"
#include "tela.h"
#include "agendamento.h"

void menuConsulta(void)
{
    const char *opcoes[] = {
        "1 - Serviço (por nome)",
        "2 - Cliente (por nome ou CPF)",
        "3 - Veículo (por placa ou CPF do cliente)",
        "4 - Mecânico (por nome ou CPF)",
        "5 - Agendamentos (por data)",
        "0 - Voltar ao menu principal"
    };
    int opcao;

    do {
        opcao = exibirMenu("CONSULTA", opcoes, 6);

        switch (opcao) {
            case 1: consultarServico();             break;
            case 2: consultarCliente();             break;
            case 3: consultarVeiculo();             break;
            case 4: consultarMecanico();            break;
            case 5: consultarAgendamentosPorData(); break;
            case 0: break;
            default:
                printf("\n%*sOpção inválida!", MENU_X, "");
                pausar();
        }
    } while (opcao != 0);
}

/* Copia só os dígitos do texto (para comparar CPFs digitados com pontuação) */
static void somenteDigitos(const char *texto, char *saida, int tamanho)
{
    int n = 0;

    for (; *texto != '\0' && n < tamanho - 1; texto++) {
        if (isdigit((unsigned char)*texto)) {
            saida[n++] = *texto;
        }
    }
    saida[n] = '\0';
}

void consultarServico(void)
{
    Servico lista[MAX_REGISTROS];
    int qtd = carregarServicos(lista, MAX_REGISTROS);
    char busca[50];
    int i, achados = 0;

    cabecalho("CONSULTA DE SERVIÇO");
    lerTexto("Nome do serviço (ou parte dele): ", busca, sizeof(busca));
    printf("\n");

    for (i = 0; i < qtd; i++) {
        if (contemTexto(lista[i].nome, busca)) {
            if (achados == 0) {
                printf("%-6s %-40s %12s\n", "CÓD.", "SERVIÇO", "VALOR (R$)");
                printf("------------------------------------------------------------\n");
            }
            printf("%-6d %-40s %12.2f\n", lista[i].codigo, lista[i].nome, lista[i].valor);
            achados++;
        }
    }
    if (achados == 0) {
        printf("Nenhum serviço encontrado.\n");
    }
    pausar();
}

void consultarCliente(void)
{
    Cliente lista[MAX_REGISTROS];
    Veiculo veiculos[MAX_REGISTROS];
    int qtd = carregarClientes(lista, MAX_REGISTROS);
    int qtdVeic = carregarVeiculos(veiculos, MAX_REGISTROS);
    char busca[100], cpfBusca[12], cpf[15];
    int i, j, achados = 0;

    cabecalho("CONSULTA DE CLIENTE");
    lerTexto("Nome ou CPF do cliente: ", busca, sizeof(busca));
    somenteDigitos(busca, cpfBusca, sizeof(cpfBusca));
    printf("\n");

    for (i = 0; i < qtd; i++) {
        if (contemTexto(lista[i].nome, busca) || strcmp(lista[i].cpf, cpfBusca) == 0) {
            formatarCPF(lista[i].cpf, cpf);
            printf("Nome     : %s\n", lista[i].nome);
            printf("CPF      : %s\n", cpf);
            printf("Telefone : %s\n", lista[i].telefone);
            printf("Veículos : ");
            for (j = 0; j < qtdVeic; j++) {
                if (strcmp(veiculos[j].cpfCliente, lista[i].cpf) == 0) {
                    printf("%s (%s %s)  ", veiculos[j].placa, veiculos[j].marca, veiculos[j].modelo);
                }
            }
            printf("\n----------------------------------------\n");
            achados++;
        }
    }
    if (achados == 0) {
        printf("Nenhum cliente encontrado.\n");
    }
    pausar();
}

void consultarVeiculo(void)
{
    Veiculo lista[MAX_REGISTROS];
    int qtd = carregarVeiculos(lista, MAX_REGISTROS);
    Cliente dono;
    char busca[30], placaBusca[30], cpfBusca[12], cpf[15];
    int i, n, achados = 0;
    char *p;

    cabecalho("CONSULTA DE VEÍCULO");
    lerTexto("Placa do veículo ou CPF do cliente: ", busca, sizeof(busca));

    /* prepara a busca nos dois formatos: placa (maiúsculas) e CPF (dígitos) */
    n = 0;
    for (p = busca; *p != '\0'; p++) {
        if (isalnum((unsigned char)*p)) {
            placaBusca[n++] = (char)toupper((unsigned char)*p);
        }
    }
    placaBusca[n] = '\0';
    somenteDigitos(busca, cpfBusca, sizeof(cpfBusca));
    printf("\n");

    for (i = 0; i < qtd; i++) {
        if (strcmp(lista[i].placa, placaBusca) == 0 || strcmp(lista[i].cpfCliente, cpfBusca) == 0) {
            formatarCPF(lista[i].cpfCliente, cpf);
            printf("Placa    : %s\n", lista[i].placa);
            printf("Tipo     : %s\n", lista[i].tipo);
            printf("Veículo  : %s %s - %s\n", lista[i].marca, lista[i].modelo, lista[i].cor);
            if (buscarClientePorCPF(lista[i].cpfCliente, &dono)) {
                printf("Cliente  : %s - CPF %s - Tel. %s\n", dono.nome, cpf, dono.telefone);
            } else {
                printf("Cliente  : CPF %s (não cadastrado)\n", cpf);
            }
            printf("----------------------------------------\n");
            achados++;
        }
    }
    if (achados == 0) {
        printf("Nenhum veículo encontrado.\n");
    }
    pausar();
}

void consultarMecanico(void)
{
    Mecanico lista[MAX_REGISTROS];
    int qtd = carregarMecanicos(lista, MAX_REGISTROS);
    char busca[100], cpfBusca[12], cpf[15];
    int i, achados = 0;

    cabecalho("CONSULTA DE MECÂNICO");
    lerTexto("Nome ou CPF do mecânico: ", busca, sizeof(busca));
    somenteDigitos(busca, cpfBusca, sizeof(cpfBusca));
    printf("\n");

    for (i = 0; i < qtd; i++) {
        if (contemTexto(lista[i].nome, busca) || strcmp(lista[i].cpf, cpfBusca) == 0) {
            if (achados == 0) {
                printf("%-50s %s\n", "NOME", "CPF");
                printf("-----------------------------------------------------------------\n");
            }
            formatarCPF(lista[i].cpf, cpf);
            printf("%-50s %s\n", lista[i].nome, cpf);
            achados++;
        }
    }
    if (achados == 0) {
        printf("Nenhum mecânico encontrado.\n");
    }
    pausar();
}

void consultarAgendamentosPorData(void)
{
    char data[11];

    cabecalho("CONSULTA DE AGENDAMENTOS POR DATA");
    lerData("Data (dd/mm/aaaa, ENTER = hoje): ", data);
    printf("\n");
    exibirAgendamentosDoDia(data);
    pausar();
}
