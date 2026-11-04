/*
 * agendamento.h - Opção 3 do menu principal: Agendamento
 */
#ifndef AGENDAMENTO_H
#define AGENDAMENTO_H

#include "dados.h"

void menuAgendamento(void);

void exibirAgendamentosDoDia(const char *data);
void registrarAgendamento(const char *data);

void exibirAgendamento(const Agendamento *a);
void ordenarAgendamentos(Agendamento *vetor, int qtd);

#endif
