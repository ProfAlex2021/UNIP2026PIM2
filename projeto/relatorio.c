/*
 * relatorio.c - Opção 4 do menu principal: Relatório
 *
 *   Exibe todos os serviços, clientes, veículos, mecânicos e agendamentos.
 */
#include <stdio.h>
#include "relatorio.h"
#include "dados.h"
#include "tela.h"
#include "agendamento.h"

void menuRelatorio(void)
{
    const char *opcoes[] = {
        "1 - Todos os serviços",
        "2 - Todos os clientes",
        "3 - Todos os veículos",
        "4 - Todos os mecânicos",
        "5 - Todos os agendamentos",
        "0 - Voltar ao menu principal"
    };
    int opcao;

    do {
        opcao = exibirMenu("RELATÓRIO", opcoes, 6);

        switch (opcao) {
            case 1: cabecalho("RELATÓRIO DE SERVIÇOS");     listarServicos();     pausar(); break;
            case 2: cabecalho("RELATÓRIO DE CLIENTES");     listarClientes();     pausar(); break;
            case 3: cabecalho("RELATÓRIO DE VEÍCULOS");     listarVeiculos();     pausar(); break;
            case 4: cabecalho("RELATÓRIO DE MECÂNICOS");    listarMecanicos();    pausar(); break;
            case 5: cabecalho("RELATÓRIO DE AGENDAMENTOS"); listarAgendamentos(); pausar(); break;
            case 0: break;
            default:
                printf("\n%*sOpção inválida!", MENU_X, "");
                pausar();
        }
    } while (opcao != 0);
}

void listarServicos(void)
{
    Servico lista[MAX_REGISTROS];
    int qtd = carregarServicos(lista, MAX_REGISTROS);
    int i;

    if (qtd == 0) {
        printf("Nenhum serviço cadastrado.\n");
        return;
    }
    printf("%-6s %-40s %12s\n", "CÓD.", "SERVIÇO", "VALOR (R$)");
    printf("------------------------------------------------------------\n");
    for (i = 0; i < qtd; i++) {
        printf("%-6d %-40s %12.2f\n", lista[i].codigo, lista[i].nome, lista[i].valor);
    }
    printf("\nTotal: %d serviço(s)\n", qtd);
}

void listarClientes(void)
{
    Cliente lista[MAX_REGISTROS];
    int qtd = carregarClientes(lista, MAX_REGISTROS);
    char cpf[15];
    int i;

    if (qtd == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
    printf("%-40s %-15s %s\n", "NOME", "CPF", "TELEFONE");
    printf("------------------------------------------------------------------------\n");
    for (i = 0; i < qtd; i++) {
        formatarCPF(lista[i].cpf, cpf);
        printf("%-40.40s %-15s %s\n", lista[i].nome, cpf, lista[i].telefone);
    }
    printf("\nTotal: %d cliente(s)\n", qtd);
}

void listarVeiculos(void)
{
    Veiculo lista[MAX_REGISTROS];
    int qtd = carregarVeiculos(lista, MAX_REGISTROS);
    Cliente dono;
    int i;

    if (qtd == 0) {
        printf("Nenhum veículo cadastrado.\n");
        return;
    }
    printf("%-8s %-6s %-12s %-14s %-10s %s\n", "PLACA", "TIPO", "MARCA", "MODELO", "COR", "CLIENTE");
    printf("-----------------------------------------------------------------------------\n");
    for (i = 0; i < qtd; i++) {
        printf("%-8s %-6s %-12.12s %-14.14s %-10.10s ", lista[i].placa, lista[i].tipo,
               lista[i].marca, lista[i].modelo, lista[i].cor);
        if (buscarClientePorCPF(lista[i].cpfCliente, &dono)) {
            printf("%.22s\n", dono.nome);
        } else {
            printf("CPF %s\n", lista[i].cpfCliente);
        }
    }
    printf("\nTotal: %d veículo(s)\n", qtd);
}

void listarMecanicos(void)
{
    Mecanico lista[MAX_REGISTROS];
    int qtd = carregarMecanicos(lista, MAX_REGISTROS);
    char cpf[15];
    int i;

    if (qtd == 0) {
        printf("Nenhum mecânico cadastrado.\n");
        return;
    }
    printf("%-50s %s\n", "NOME", "CPF");
    printf("-----------------------------------------------------------------\n");
    for (i = 0; i < qtd; i++) {
        formatarCPF(lista[i].cpf, cpf);
        printf("%-50.50s %s\n", lista[i].nome, cpf);
    }
    printf("\nTotal: %d mecânico(s)\n", qtd);
}

void listarAgendamentos(void)
{
    Agendamento lista[MAX_REGISTROS];
    int qtd = carregarAgendamentos(lista, MAX_REGISTROS);
    float total = 0;
    int i;

    if (qtd == 0) {
        printf("Nenhum agendamento cadastrado.\n");
        return;
    }
    ordenarAgendamentos(lista, qtd);
    for (i = 0; i < qtd; i++) {
        exibirAgendamento(&lista[i]);
        total += valorTotalAgendamento(&lista[i]);
    }
    printf("Total: %d agendamento(s) - valor previsto: R$ %.2f\n", qtd, total);
}
