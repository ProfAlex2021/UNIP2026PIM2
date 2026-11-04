/*
 * tela.c - Menus com moldura (gotoxy) e leitura segura do teclado.
 *
 * A moldura usa caracteres da página de código 850 do console do Windows:
 *   201 ╔   187 ╗   200 ╚   188 ╝   205 ═   186 ║   204 ╠   185 ╣
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>
#include "tela.h"

#define CANTO_SUP_ESQ  201
#define CANTO_SUP_DIR  187
#define CANTO_INF_ESQ  200
#define CANTO_INF_DIR  188
#define LINHA_HORIZ    205
#define LINHA_VERT     186
#define DIVISAO_ESQ    204
#define DIVISAO_DIR    185

/* Coloca o console na página de código 850 (moldura + acentos) */
void configurarConsole(void)
{
    system("chcp 850 > nul");
    system("title Oficina Automotiva 2000");
}

/* Move o cursor para a coluna x, linha y */
void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

/* ---------- Moldura ---------- */

static void linhaHorizontal(int x, int y, int largura, int esquerda, int direita)
{
    int i;

    gotoxy(x, y);
    printf("%c", esquerda);
    for (i = 0; i < largura - 2; i++) {
        printf("%c", LINHA_HORIZ);
    }
    printf("%c", direita);
}

/* Escreve o texto dentro da moldura; centralizado = 1 centraliza */
static void linhaTexto(int x, int y, int largura, const char *texto, int centralizado)
{
    int espacoInterno = largura - 2;
    int tamanho = (int)strlen(texto);
    int esquerda;

    if (tamanho > espacoInterno) {
        tamanho = espacoInterno;
    }
    esquerda = centralizado ? (espacoInterno - tamanho) / 2 : 3;
    if (esquerda + tamanho > espacoInterno) {
        esquerda = espacoInterno - tamanho;
    }

    gotoxy(x, y);
    printf("%c%*s%.*s%*s%c", LINHA_VERT,
           esquerda, "",
           tamanho, texto,
           espacoInterno - esquerda - tamanho, "",
           LINHA_VERT);
}

/*
 * Limpa a tela, desenha um menu com moldura e lê a opção escolhida.
 * opcoes[] é um vetor de ponteiros para texto (strings).
 */
int exibirMenu(const char *titulo, const char *opcoes[], int qtdOpcoes)
{
    int y = MENU_Y;
    int i;

    system("cls");

    linhaHorizontal(MENU_X, y++, MENU_LARGURA, CANTO_SUP_ESQ, CANTO_SUP_DIR);
    linhaTexto(MENU_X, y++, MENU_LARGURA, "", 0);
    linhaTexto(MENU_X, y++, MENU_LARGURA, "OFICINA AUTOMOTIVA 2000", 1);
    linhaTexto(MENU_X, y++, MENU_LARGURA, titulo, 1);
    linhaHorizontal(MENU_X, y++, MENU_LARGURA, DIVISAO_ESQ, DIVISAO_DIR);
    linhaTexto(MENU_X, y++, MENU_LARGURA, "", 0);
    for (i = 0; i < qtdOpcoes; i++) {
        linhaTexto(MENU_X, y++, MENU_LARGURA, opcoes[i], 0);
    }
    linhaTexto(MENU_X, y++, MENU_LARGURA, "", 0);
    linhaHorizontal(MENU_X, y++, MENU_LARGURA, CANTO_INF_ESQ, CANTO_INF_DIR);

    gotoxy(MENU_X, y + 1);
    return lerInteiro("Escolha uma opção: ");
}

/* Limpa a tela e desenha o título da tela de operação */
void cabecalho(const char *titulo)
{
    system("cls");
    linhaHorizontal(0, 0, 78, CANTO_SUP_ESQ, CANTO_SUP_DIR);
    linhaTexto(0, 1, 78, titulo, 1);
    linhaHorizontal(0, 2, 78, CANTO_INF_ESQ, CANTO_INF_DIR);
    gotoxy(0, 4);
}

void pausar(void)
{
    char linha[10];

    lerTexto("\nPressione ENTER para continuar...", linha, sizeof(linha));
}

/* ---------- Leitura do teclado ---------- */

/*
 * Lê uma linha inteira (com espaços) e remove o ENTER do final.
 * O ';' é trocado por ',' porque ele separa os campos nos arquivos.
 */
void lerTexto(const char *mensagem, char *destino, int tamanho)
{
    char *p;
    int c;

    printf("%s", mensagem);
    if (fgets(destino, tamanho, stdin) == NULL) {
        /* fim da entrada (Ctrl+Z): não há mais o que ler, encerra o programa */
        printf("\nEntrada encerrada.\n");
        exit(0);
    }

    p = strchr(destino, '\n');
    if (p != NULL) {
        *p = '\0';
    } else {
        while ((c = getchar()) != '\n' && c != EOF);  /* descarta o excesso */
    }

    for (p = destino; *p != '\0'; p++) {
        if (*p == ';') {
            *p = ',';
        }
    }
}

/* Igual a lerTexto, mas não aceita texto vazio */
void lerTextoObrigatorio(const char *mensagem, char *destino, int tamanho)
{
    do {
        lerTexto(mensagem, destino, tamanho);
        if (destino[0] == '\0') {
            printf("   Campo obrigatório.\n");
        }
    } while (destino[0] == '\0');
}

int lerInteiro(const char *mensagem)
{
    char linha[30];
    int valor;

    while (1) {
        lerTexto(mensagem, linha, sizeof(linha));
        if (sscanf(linha, "%d", &valor) == 1) {
            return valor;
        }
        printf("   Digite um número inteiro.\n");
    }
}

/* Lê um valor em reais; aceita vírgula ou ponto como separador decimal */
float lerValor(const char *mensagem)
{
    char linha[30];
    char *p;
    float valor;

    while (1) {
        lerTexto(mensagem, linha, sizeof(linha));
        for (p = linha; *p != '\0'; p++) {
            if (*p == ',') {
                *p = '.';
            }
        }
        if (sscanf(linha, "%f", &valor) == 1 && valor > 0) {
            return valor;
        }
        printf("   Digite um valor maior que zero (ex.: 150,00).\n");
    }
}

/* Lê um CPF guardando apenas os 11 dígitos (aceita 123.456.789-00) */
void lerCPF(const char *mensagem, char *cpf)
{
    char linha[30];
    char *p;
    int n;

    while (1) {
        lerTexto(mensagem, linha, sizeof(linha));
        n = 0;
        for (p = linha; *p != '\0' && n <= 11; p++) {
            if (isdigit((unsigned char)*p)) {
                cpf[n++] = *p;
            }
        }
        if (n == 11) {
            cpf[11] = '\0';
            return;
        }
        printf("   O CPF deve ter 11 dígitos.\n");
    }
}

/* Lê a placa no padrão antigo (ABC1234) ou Mercosul (ABC1D23) */
void lerPlaca(const char *mensagem, char *placa)
{
    char linha[30];
    char *p;
    int n;

    while (1) {
        lerTexto(mensagem, linha, sizeof(linha));
        n = 0;
        for (p = linha; *p != '\0' && n <= 7; p++) {
            if (isalnum((unsigned char)*p)) {
                placa[n++] = (char)toupper((unsigned char)*p);
            }
        }
        placa[n < 8 ? n : 7] = '\0';
        if (n == 7 &&
            isalpha((unsigned char)placa[0]) && isalpha((unsigned char)placa[1]) &&
            isalpha((unsigned char)placa[2]) && isdigit((unsigned char)placa[3]) &&
            isalnum((unsigned char)placa[4]) && isdigit((unsigned char)placa[5]) &&
            isdigit((unsigned char)placa[6])) {
            return;
        }
        printf("   Placa inválida. Use ABC1234 ou ABC1D23.\n");
    }
}

static int dataValida(int dia, int mes, int ano)
{
    int diasNoMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (ano < 2000 || ano > 2100 || mes < 1 || mes > 12) {
        return 0;
    }
    if ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0) {
        diasNoMes[1] = 29;
    }
    return dia >= 1 && dia <= diasNoMes[mes - 1];
}

/* Lê uma data dd/mm/aaaa. ENTER vazio = data de hoje. */
void lerData(const char *mensagem, char *data)
{
    char linha[30];
    int dia, mes, ano;
    time_t agora;
    struct tm *hoje;

    while (1) {
        lerTexto(mensagem, linha, sizeof(linha));
        if (linha[0] == '\0') {
            agora = time(NULL);
            hoje = localtime(&agora);
            dia = hoje->tm_mday;
            mes = hoje->tm_mon + 1;
            ano = hoje->tm_year + 1900;
        } else if (sscanf(linha, "%d/%d/%d", &dia, &mes, &ano) != 3) {
            dia = 0;
        }
        if (dataValida(dia, mes, ano)) {
            sprintf(data, "%02d/%02d/%04d", dia % 100, mes % 100, ano % 10000);
            return;
        }
        printf("   Data inválida. Use dd/mm/aaaa.\n");
    }
}

/* Lê um horário hh:mm */
void lerHora(const char *mensagem, char *hora)
{
    char linha[30];
    int h, m;

    while (1) {
        lerTexto(mensagem, linha, sizeof(linha));
        if (sscanf(linha, "%d:%d", &h, &m) == 2 && h >= 0 && h <= 23 && m >= 0 && m <= 59) {
            sprintf(hora, "%02d:%02d", h % 100, m % 100);
            return;
        }
        printf("   Horário inválido. Use hh:mm (ex.: 08:30).\n");
    }
}

/* Pergunta S/N e devolve 1 para sim */
int confirmar(const char *mensagem)
{
    char resposta[10];

    lerTexto(mensagem, resposta, sizeof(resposta));
    return resposta[0] == 'S' || resposta[0] == 's';
}

/* ---------- Utilitários de texto ---------- */

/* "12345678900" -> "123.456.789-00" (saida precisa de 15 posições) */
void formatarCPF(const char *cpf, char *saida)
{
    sprintf(saida, "%.3s.%.3s.%.3s-%.2s", cpf, cpf + 3, cpf + 6, cpf + 9);
}

/* Devolve 1 se 'trecho' aparece em 'texto', sem diferenciar maiúsculas */
int contemTexto(const char *texto, const char *trecho)
{
    const char *t, *a, *b;

    if (*trecho == '\0') {
        return 1;
    }
    for (t = texto; *t != '\0'; t++) {
        a = t;
        b = trecho;
        while (*a != '\0' && *b != '\0' &&
               toupper((unsigned char)*a) == toupper((unsigned char)*b)) {
            a++;
            b++;
        }
        if (*b == '\0') {
            return 1;
        }
    }
    return 0;
}

/* Converte dd/mm/aaaa + hh:mm em um número aaaammddhhmm (para ordenar) */
int chaveDataHora(const char *data, const char *hora, long long *chave)
{
    int dia, mes, ano, h = 0, m = 0;

    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        return 0;
    }
    if (hora != NULL) {
        sscanf(hora, "%d:%d", &h, &m);
    }
    *chave = ((((long long)ano * 100 + mes) * 100 + dia) * 100 + h) * 100 + m;
    return 1;
}
