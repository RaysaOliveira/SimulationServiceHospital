/* 
 * File:   fila.h
 * Author: raysaoliveira
 *
 * Created on 15 de Dezembro de 2015, 19:18
 */

#ifndef FILA_H
#define	FILA_H

struct fase{
    int tempo_chegada;
    int tempo_inicio_atendimento;
    int duracao_atendimento;
    int tempo_partida;
    int prioridade;
};

struct utente{ //tydef renomear o tipo. Ex: struct pessoa para pessoa;
    int id;
    struct fase fase[4];

};

typedef struct node{//no node tem pessoa e prox
    struct utente *utente;
    struct node * prox; //ponteiro pro proximo node;
} node;

typedef struct fila{
    node * inicio;
    node * fim;
    int quant_atual;
    int total_utentes_chegados;
} fila;

//funcoes da fila
void inicializar_fila(fila *f);
void limpar_fila(fila *f);
int vazia(fila * f);
int inserir(struct utente *utente, fila * f); // a fila é ponteiro pq é unica

/**
 * remove um utente do inicio da fila
 * @param f fila da qual o utente será removido
 * @return utente removido da fila ou NULL se a fila estive vazia
 */
struct utente * remover_inicio(fila * f);
void listar(fila * f);
void imprimir_utente(char mensagem[], struct utente * utente, int indice_fase);

/**
 * Calcula o tempo de espera na fila para uma determinada fase 
 * de um utente.
 * @param fase fase para qual deseja calcular o tempo de espera
 * @return o tempo de espera do utente na fila da fase indicada.
 */
int calcular_tempo_espera_na_fila_fase(struct fase fase);

/**
 * Calcula o tempo de partida na fila para uma determinada fase 
 * de um utente.
 * @param fase fase para qual deseja calcular o tempo de partida
 * @return o tempo de partida do utente na fila da fase indicada.
 */
int calcular_tempo_partida_na_fila_fase(struct fase fase);

#endif	/* FILA_H */

