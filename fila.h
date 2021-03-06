/* 
 * File:   fila.h
 * Author: raysaoliveira
 *
 * Implementa funções para manipulação de filas encadeadas
 */
#include "tipos.h"

#ifndef FILA_H
#define	FILA_H


//funcoes da fila

/**
 * Aloca memória para uma nova fila e inicializa a mesma.
 * @return a nova fila alocada
 */
struct fila * inicializar_fila();

void limpar_fila(struct fila *f);
void limpar_vetor_filas(struct fila * filas[], int tamanho_vetor_filas);
int vazia(struct fila * f);
int inserir(struct utente *utente, struct fila * f); // a fila é ponteiro pq é unica

/**
 * remove um utente do inicio da fila
 * @param f fila da qual o utente será removido
 * @return utente removido da fila ou NULL se a fila estive vazia
 */
struct utente * remover_inicio(struct fila * f);
void listar(struct fila * f);
void imprimir_utente(char mensagem[], struct utente * utente, int indice_fase);

/**
 * Calcula o tempo de espera na fila para uma determinada fase 
 * de um utente.
 * @param utente para qual deseja-se calcular o tempo de espera para uma determinada fase
 * @param num_fase número da fase para qual deseja calcular o tempo de espera
 * @return o tempo de espera do utente na fila da fase indicada.
 */
int calcular_tempo_espera_na_fila_fase(struct utente *utente, int num_fase);

/**
 * Calcula o tempo de partida na fila para uma determinada fase 
 * de um utente.
 * @param fase fase para qual deseja calcular o tempo de partida
 * @return o tempo de partida do utente na fila da fase indicada.
 */
int calcular_tempo_partida_do_utente_na_fila(struct status_fase fase);


/**
 * Remove o primeiro utente que estiver na fila de espera, respeitando a prioridade
 * @param fase fase de onde será removido um cliente de alguma das filas existentes
 * @return o utente removido ou NULL caso todas as filas estejam vazias. 
 */
struct utente * pesquisar_todas_as_filas_e_remover_utente_maior_prioridade(struct fase * fase);

struct utente * criar_e_inicializar_utente(int id, int max_consulta_medicas_por_utente);
#endif	/* FILA_H */

