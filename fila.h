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

/**
 * Aloca memória para uma nova fila e inicializa a mesma.
 * @return a nova fila alocada
 */
fila * inicializar_fila();

/**
 * Inicializa um vetor de filas;
 * @param filas
 * @param tamanho_vetor_filas
 */
void inicializar_vetor_filas(fila * filas[], int tamanho_vetor_filas);
void limpar_fila(fila *f);
void limpar_vetor_filas(fila * filas[], int tamanho_vetor_filas);
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


/**
 * Remove o primeiro utente que estiver na fila de espera respeitando a prioridade
 * @param filas vetor de fila que pretende fazer as buscas
 * @param tamanho_vetor_filas tamanho do vetor
 * @return o utente removido ou NULL caso todas as filas estejam vazias. 
 */
struct utente * remover_utente_da_primeira_fila_com_clientes_em_espera(fila * filas[], int tamanho_vetor_filas);
#endif	/* FILA_H */

