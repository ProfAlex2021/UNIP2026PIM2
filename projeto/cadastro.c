/*
 * cadastro.c - Opção 1 do menu principal: Cadastro
 *
 *   1 - Serviço automotivo (nome e valor)
 *   2 - Cliente (nome, telefone e CPF)
 *   3 - Veículo (placa, tipo, marca, modelo, cor e CPF do cliente)
 *   4 - Mecânico (nome e CPF)
 */
#include <stdio.h>
#include <string.h>
#include "cadastro.h"
#include "dados.h"
#include "tela.h"
#include "relatorio.h"

void menuCadastro(void)
{
    const char *opcoes[] = {
        "1 - Cadastrar serviço automotivo",
        "2 - Cadastrar cliente",
        "3 - Cadastrar veículo",
        "4 - Cadastrar mecânico",
        "0 - Voltar ao menu principal"
    };
    int opcao;

    do {
        opcao = exibirMenu("CADASTRO", opcoes, 5);

        switch (opcao) {
            case 1: cadastrarServico();  break;
            case 2: cadastrarCliente();  break;
            case 3: cadastrarVeiculo();  break;
            case 4: cadastrarMecanico(); break;
            case 0: break;
            default:
                printf("\n%*sOpção inválida!", MENU_X, "");
                pausar();
        }
    } while (opcao != 0);
}

void cadastrarServico(void)
{
    Servico novo;

    cabecalho("CADASTRO DE SERVIÇO AUTOMOTIVO");
    printf("Serviços já cadastrados:\n\n");
    listarServicos();
    printf("\n");

    lerTextoObrigatorio("Nome do serviço: ", novo.nome, sizeof(novo.nome));
    if (buscarServicoPorNome(novo.nome, NULL)) {
        printf("\nJá existe um serviço com o nome \"%s\".\n", novo.nome);
        pausar();
        return;
    }
    novo.valor = lerValor("Valor (R$): ");
    novo.codigo = proximoCodigoServico();

    if (gravarServico(&novo)) {
        printf("\nServiço %d - %s (R$ %.2f) cadastrado com sucesso!\n",
               novo.codigo, novo.nome, novo.valor);
    } else {
        printf("\nErro ao gravar em %s.\n", ARQ_SERVICOS);
    }
    pausar();
}

void cadastrarCliente(void)
{
    Cliente novo;
    char cpfFormatado[15];

    cabecalho("CADASTRO DE CLIENTE");

    lerCPF("CPF: ", novo.cpf);
    if (buscarClientePorCPF(novo.cpf, NULL)) {
        formatarCPF(novo.cpf, cpfFormatado);
        printf("\nJá existe um cliente com o CPF %s.\n", cpfFormatado);
        pausar();
        return;
    }
    lerTextoObrigatorio("Nome: ", novo.nome, sizeof(novo.nome));
    lerTextoObrigatorio("Telefone: ", novo.telefone, sizeof(novo.telefone));

    if (gravarCliente(&novo)) {
        printf("\nCliente %s cadastrado com sucesso!\n", novo.nome);
    } else {
        printf("\nErro ao gravar em %s.\n", ARQ_CLIENTES);
    }
    pausar();
}

void cadastrarVeiculo(void)
{
    Veiculo novo;
    Cliente dono;
    int tipo;

    cabecalho("CADASTRO DE VEÍCULO");

    lerPlaca("Placa: ", novo.placa);
    if (buscarVeiculoPorPlaca(novo.placa, NULL)) {
        printf("\nA placa %s já está cadastrada.\n", novo.placa);
        pausar();
        return;
    }

    /* o veículo precisa pertencer a um cliente já cadastrado */
    lerCPF("CPF do cliente: ", novo.cpfCliente);
    if (!buscarClientePorCPF(novo.cpfCliente, &dono)) {
        printf("\nCliente não encontrado. Cadastre o cliente antes do veículo.\n");
        pausar();
        return;
    }
    printf("   Cliente: %s\n", dono.nome);

    do {
        tipo = lerInteiro("Tipo (1 - Carro | 2 - Moto): ");
    } while (tipo != 1 && tipo != 2);
    strcpy(novo.tipo, tipo == 1 ? "Carro" : "Moto");

    lerTextoObrigatorio("Marca: ", novo.marca, sizeof(novo.marca));
    lerTextoObrigatorio("Modelo: ", novo.modelo, sizeof(novo.modelo));
    lerTextoObrigatorio("Cor: ", novo.cor, sizeof(novo.cor));

    if (gravarVeiculo(&novo)) {
        printf("\n%s %s %s (%s) cadastrado para %s.\n",
               novo.tipo, novo.marca, novo.modelo, novo.placa, dono.nome);
    } else {
        printf("\nErro ao gravar em %s.\n", ARQ_VEICULOS);
    }
    pausar();
}

void cadastrarMecanico(void)
{
    Mecanico novo;
    char cpfFormatado[15];

    cabecalho("CADASTRO DE MECÂNICO");

    lerCPF("CPF: ", novo.cpf);
    if (buscarMecanicoPorCPF(novo.cpf, NULL)) {
        formatarCPF(novo.cpf, cpfFormatado);
        printf("\nJá existe um mecânico com o CPF %s.\n", cpfFormatado);
        pausar();
        return;
    }
    lerTextoObrigatorio("Nome: ", novo.nome, sizeof(novo.nome));

    if (gravarMecanico(&novo)) {
        printf("\nMecânico %s cadastrado com sucesso!\n", novo.nome);
    } else {
        printf("\nErro ao gravar em %s.\n", ARQ_MECANICOS);
    }
    pausar();
}
