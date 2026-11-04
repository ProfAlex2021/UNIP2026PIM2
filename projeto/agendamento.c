/*
 * agendamento.c - Opção 3 do menu principal: Agendamento
 *
 *   Primeiro o sistema recebe o DIA. Depois, para esse dia:
 *     1 - Exibir os agendamentos do dia
 *     2 - Registrar novo agendamento (hora, placa, serviços e CPF do mecânico)
 *     3 - Informar outro dia
 */
#include <stdio.h>
#include <string.h>
#include "agendamento.h"
#include "dados.h"
#include "tela.h"
#include "relatorio.h"

void menuAgendamento(void)
{
    const char *opcoes[] = {
        "1 - Exibir agendamentos do dia",
        "2 - Registrar novo agendamento",
        "3 - Informar outro dia",
        "0 - Voltar ao menu principal"
    };
    char data[11];
    char titulo[40];
    int opcao;

    cabecalho("AGENDAMENTO");
    lerData("Informe o dia (dd/mm/aaaa, ENTER = hoje): ", data);

    do {
        sprintf(titulo, "AGENDAMENTO - %s", data);
        opcao = exibirMenu(titulo, opcoes, 4);

        switch (opcao) {
            case 1:
                sprintf(titulo, "AGENDAMENTOS DO DIA %s", data);
                cabecalho(titulo);
                exibirAgendamentosDoDia(data);
                pausar();
                break;
            case 2:
                registrarAgendamento(data);
                break;
            case 3:
                cabecalho("AGENDAMENTO");
                lerData("Informe o dia (dd/mm/aaaa, ENTER = hoje): ", data);
                break;
            case 0:
                break;
            default:
                printf("\n%*sOpção inválida!", MENU_X, "");
                pausar();
        }
    } while (opcao != 0);
}

/* Ordena por data e hora (ordenação por inserção) */
void ordenarAgendamentos(Agendamento *vetor, int qtd)
{
    Agendamento atual;
    long long chaveAtual, chaveOutro;
    int i, j;

    for (i = 1; i < qtd; i++) {
        atual = vetor[i];
        chaveDataHora(atual.data, atual.hora, &chaveAtual);
        j = i - 1;
        while (j >= 0 && chaveDataHora(vetor[j].data, vetor[j].hora, &chaveOutro)
               && chaveOutro > chaveAtual) {
            vetor[j + 1] = vetor[j];
            j--;
        }
        vetor[j + 1] = atual;
    }
}

/* Mostra um agendamento com os dados do veículo, cliente, mecânico e serviços */
void exibirAgendamento(const Agendamento *a)
{
    Veiculo veiculo;
    Cliente cliente;
    Mecanico mecanico;
    Servico servico;
    int i;

    printf("Agendamento nº %d - %s às %s\n", a->codigo, a->data, a->hora);

    if (buscarVeiculoPorPlaca(a->placa, &veiculo)) {
        printf("   Veículo  : %s - %s %s %s (%s)\n", veiculo.placa, veiculo.tipo,
               veiculo.marca, veiculo.modelo, veiculo.cor);
        if (buscarClientePorCPF(veiculo.cpfCliente, &cliente)) {
            printf("   Cliente  : %s - Tel. %s\n", cliente.nome, cliente.telefone);
        }
    } else {
        printf("   Veículo  : %s (não cadastrado)\n", a->placa);
    }

    if (buscarMecanicoPorCPF(a->cpfMecanico, &mecanico)) {
        printf("   Mecânico : %s\n", mecanico.nome);
    } else {
        printf("   Mecânico : CPF %s (não cadastrado)\n", a->cpfMecanico);
    }

    printf("   Serviços :\n");
    for (i = 0; i < a->qtdServicos; i++) {
        if (buscarServicoPorCodigo(a->servicos[i], &servico)) {
            printf("      - %-35s R$ %9.2f\n", servico.nome, servico.valor);
        } else {
            printf("      - Serviço %d (não cadastrado)\n", a->servicos[i]);
        }
    }
    printf("   Total    : %-35s R$ %9.2f\n", "", valorTotalAgendamento(a));
    printf("------------------------------------------------------------\n");
}

void exibirAgendamentosDoDia(const char *data)
{
    Agendamento lista[MAX_REGISTROS];
    int qtd = carregarAgendamentos(lista, MAX_REGISTROS);
    int i, doDia = 0;
    float totalDia = 0;

    ordenarAgendamentos(lista, qtd);

    for (i = 0; i < qtd; i++) {
        if (strcmp(lista[i].data, data) == 0) {
            exibirAgendamento(&lista[i]);
            totalDia += valorTotalAgendamento(&lista[i]);
            doDia++;
        }
    }

    if (doDia == 0) {
        printf("Nenhum agendamento para %s.\n", data);
    } else {
        printf("%d agendamento(s) em %s - total previsto: R$ %.2f\n", doDia, data, totalDia);
    }
}

/* Verifica se a placa ou o mecânico já estão ocupados na mesma data e hora */
static int horarioOcupado(const Agendamento *novo)
{
    Agendamento lista[MAX_REGISTROS];
    int qtd = carregarAgendamentos(lista, MAX_REGISTROS);
    int i;

    for (i = 0; i < qtd; i++) {
        if (strcmp(lista[i].data, novo->data) == 0 && strcmp(lista[i].hora, novo->hora) == 0) {
            if (strcmp(lista[i].placa, novo->placa) == 0) {
                printf("\nO veículo %s já tem agendamento em %s às %s.\n",
                       novo->placa, novo->data, novo->hora);
                return 1;
            }
            if (strcmp(lista[i].cpfMecanico, novo->cpfMecanico) == 0) {
                printf("\nO mecânico já está ocupado em %s às %s.\n", novo->data, novo->hora);
                return 1;
            }
        }
    }
    return 0;
}

/* Lê os códigos dos serviços até o usuário digitar 0 */
static void escolherServicos(Agendamento *novo)
{
    Servico servico;
    int codigo, i, repetido;

    novo->qtdServicos = 0;
    printf("\nServiços disponíveis:\n\n");
    listarServicos();
    printf("\nInforme os códigos dos serviços (0 para finalizar).\n");

    while (novo->qtdServicos < MAX_SERV_AGEND) {
        codigo = lerInteiro("Código do serviço: ");
        if (codigo == 0) {
            if (novo->qtdServicos > 0) {
                break;
            }
            printf("   Escolha pelo menos um serviço.\n");
            continue;
        }
        if (!buscarServicoPorCodigo(codigo, &servico)) {
            printf("   Serviço não encontrado.\n");
            continue;
        }
        repetido = 0;
        for (i = 0; i < novo->qtdServicos; i++) {
            if (novo->servicos[i] == codigo) {
                repetido = 1;
            }
        }
        if (repetido) {
            printf("   Serviço já incluído.\n");
            continue;
        }
        novo->servicos[novo->qtdServicos++] = codigo;
        printf("   + %s (R$ %.2f)\n", servico.nome, servico.valor);
    }
}

void registrarAgendamento(const char *data)
{
    Agendamento novo;
    Veiculo veiculo;
    Cliente cliente;
    Mecanico mecanico;
    Veiculo vTeste[1];
    Mecanico mTeste[1];
    char titulo[40];

    sprintf(titulo, "NOVO AGENDAMENTO - %s", data);
    cabecalho(titulo);

    /* sem veículos ou mecânicos cadastrados não há como agendar */
    if (carregarVeiculos(vTeste, 1) == 0 || carregarMecanicos(mTeste, 1) == 0) {
        printf("Cadastre ao menos um veículo e um mecânico antes de agendar.\n");
        pausar();
        return;
    }

    strcpy(novo.data, data);
    lerHora("Hora (hh:mm): ", novo.hora);

    lerPlaca("Placa do veículo: ", novo.placa);
    if (!buscarVeiculoPorPlaca(novo.placa, &veiculo)) {
        printf("\nVeículo não encontrado. Cadastre o veículo antes de agendar.\n");
        pausar();
        return;
    }
    printf("   %s %s %s (%s)", veiculo.tipo, veiculo.marca, veiculo.modelo, veiculo.cor);
    if (buscarClientePorCPF(veiculo.cpfCliente, &cliente)) {
        printf(" - Cliente: %s", cliente.nome);
    }
    printf("\n");

    escolherServicos(&novo);

    printf("\nMecânicos:\n\n");
    listarMecanicos();
    printf("\n");
    lerCPF("CPF do mecânico: ", novo.cpfMecanico);
    if (!buscarMecanicoPorCPF(novo.cpfMecanico, &mecanico)) {
        printf("\nMecânico não encontrado.\n");
        pausar();
        return;
    }
    printf("   Mecânico: %s\n", mecanico.nome);

    if (horarioOcupado(&novo)) {
        pausar();
        return;
    }

    novo.codigo = proximoCodigoAgendamento();

    printf("\n================ RESUMO ================\n");
    exibirAgendamento(&novo);

    if (!confirmar("Confirma o agendamento? (S/N): ")) {
        printf("\nAgendamento cancelado.\n");
    } else if (gravarAgendamento(&novo)) {
        printf("\nAgendamento nº %d registrado com sucesso!\n", novo.codigo);
    } else {
        printf("\nErro ao gravar em %s.\n", ARQ_AGENDAMENTOS);
    }
    pausar();
}
