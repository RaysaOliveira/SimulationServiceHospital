/* 
 * File:   fila.h
 * Author: raysaoliveira
 *
 * Created on 15 de Dezembro de 2015, 19:18
 */

#ifndef FILA_H
#define	FILA_H

struct fase{
    int id_fase;
    int tempo_chegada;
    int tempo_partida;
};

typedef struct utente{ //tydef renomear o tipo. Ex: struct pessoa para pessoa;
    int id;
    struct fase fase;
    int prioridade;
} utente;

typedef struct node{//no node tem pessoa e prox
    utente pes;
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
int vazia(fila * f);
int inserir(utente pes, fila * f); // a fila é ponteiro pq é unica
utente remover_inicio(fila * f);
void listar(fila * f);


#endif	/* FILA_H */

