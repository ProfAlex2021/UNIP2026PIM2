/*
 * ============================================================
 *  OFICINA AUTOMOTIVA 2000
 *  Sistema de cadastro, consulta, agendamento e relatórios
 * ============================================================
 *
 *  Arquivos do projeto:
 *    main.c         menu principal
 *    cadastro.c     opção 1 - Cadastro
 *    consulta.c     opção 2 - Consulta
 *    agendamento.c  opção 3 - Agendamento
 *    relatorio.c    opção 4 - Relatório
 *    dados.c        leitura e gravação dos arquivos da pasta "dados"
 *    tela.c         menus com moldura (gotoxy) e leitura do teclado
 *
 *  Os dados ficam em arquivos texto na pasta "dados",
 *  ao lado do projeto (servicos.txt, clientes.txt, ...).
 */

#include <stdio.h>
#include <stdlib.h>
#include "tela.h"
#include "dados.h"
#include "cadastro.h"
#include "consulta.h"
#include "agendamento.h"
#include "relatorio.h"

int main(void)
{
    const char *opcoes[] = {
        "1 - Cadastro",
        "2 - Consulta",
        "3 - Agendamento",
        "4 - Relatório",
        "0 - Sair"
    };
    int opcao;

    configurarConsole();
    inicializarDados();   /* cria a pasta "dados" e a lista inicial de serviços */

    do {
        opcao = exibirMenu("MENU PRINCIPAL", opcoes, 5);

        switch (opcao) {
            case 1: menuCadastro();    break;
            case 2: menuConsulta();    break;
            case 3: menuAgendamento(); break;
            case 4: menuRelatorio();   break;
            case 0:
                printf("\n%*s=> Encerrando o sistema. Até logo!\n", MENU_X, "");
                break;
            default:
                printf("\n%*s=> Opção inválida! Tente novamente.\n", MENU_X, "");
                pausar();
        }
    } while (opcao != 0);

    return 0;
}
