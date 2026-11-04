#include <stdio.h>
#include <stdlib.h> // Necessário para system("cls") e system("pause")
#include <windows.h> // Necessário para a função gotoxy

// Função para mover o cursor para a posição (x, y)
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Função para desenhar a caixa do menu
void desenharMenu() {
    int x_inicial = 20; // Coluna inicial
    int y_inicial = 5;  // Linha inicial
    int largura = 40;   // Largura da caixa
    int i;

    // 1. Desenha a borda superior
    gotoxy(x_inicial, y_inicial);
    printf("%c", 201);
    for (i = 0; i < largura - 2; i++) printf("%c", 205);
    printf("%c", 187);

    // 2. Desenha as laterais e as opções de texto
    gotoxy(x_inicial, y_inicial + 1);
    printf("%c                                      %c", 186, 186);
    
    gotoxy(x_inicial, y_inicial + 2);
    printf("%c       OFICINA AUTOMOTIVA 2000        %c", 186, 186);
    
    gotoxy(x_inicial, y_inicial + 3);
    printf("%c                                      %c", 186, 186);
    
    gotoxy(x_inicial, y_inicial + 4);
    printf("%c   1 - Cadastrar Cliente              %c", 186, 186);
    
    gotoxy(x_inicial, y_inicial + 5);
    printf("%c   2 - Cadastrar Veiculo              %c", 186, 186);
    
    gotoxy(x_inicial, y_inicial + 6);
    printf("%c   3 - Ordem de Servico               %c", 186, 186);
    
    gotoxy(x_inicial, y_inicial + 7);
    printf("%c   0 - Sair                           %c", 186, 186);
    
    gotoxy(x_inicial, y_inicial + 8);
    printf("%c                                      %c", 186, 186);

    // 3. Desenha a borda inferior
    gotoxy(x_inicial, y_inicial + 9);
    printf("%c", 200);
    for (i = 0; i < largura - 2; i++) printf("%c", 205);
    printf("%c", 188);
}

int main() {
    int opcao;

    do {
        system("cls"); // Limpa a tela a cada iteração
        desenharMenu();
        
        // Pede a entrada do usuário logo abaixo do menu
        gotoxy(20, 16);
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        
        // Posiciona o cursor mais abaixo para as mensagens do sistema
        gotoxy(20, 18);

        switch(opcao) {
            case 1:
                printf("=> Tela de Cadastro de Cliente aberta.\n");
                // Aqui você chamaria a função cadastrarCliente();
                break;
            case 2:
                printf("=> Tela de Cadastro de Veiculo aberta.\n");
                // Aqui você chamaria a função cadastrarVeiculo();
                break;
            case 3:
                printf("=> Tela de Ordem de Servico aberta.\n");
                // Aqui você chamaria a função emitirOrdemServico();
                break;
            case 0:
                printf("=> Encerrando o sistema. Ate logo!\n");
                break;
            default:
                printf("=> Opcao Invalida! Tente novamente.\n");
        }

        // Se o usuário não escolheu sair, pausa a tela para ele ler a mensagem
        if (opcao != 0) {
            gotoxy(20, 20);
            system("pause"); 
        }

    } while (opcao != 0);

    return 0;
}