/*
 * tela.h - Funções de tela (menus com moldura) e de leitura do teclado.
 */
#ifndef TELA_H
#define TELA_H

#define MENU_X        15   /* coluna onde os menus são desenhados */
#define MENU_Y        2    /* linha onde os menus são desenhados  */
#define MENU_LARGURA  50   /* largura da moldura dos menus        */

void  configurarConsole(void);
void  gotoxy(int x, int y);

int   exibirMenu(const char *titulo, const char *opcoes[], int qtdOpcoes);
void  cabecalho(const char *titulo);
void  pausar(void);

void  lerTexto(const char *mensagem, char *destino, int tamanho);
void  lerTextoObrigatorio(const char *mensagem, char *destino, int tamanho);
int   lerInteiro(const char *mensagem);
float lerValor(const char *mensagem);
void  lerCPF(const char *mensagem, char *cpf);
void  lerPlaca(const char *mensagem, char *placa);
void  lerData(const char *mensagem, char *data);
void  lerHora(const char *mensagem, char *hora);
int   confirmar(const char *mensagem);

void  formatarCPF(const char *cpf, char *saida);
int   contemTexto(const char *texto, const char *trecho);
int   chaveDataHora(const char *data, const char *hora, long long *chave);

#endif
