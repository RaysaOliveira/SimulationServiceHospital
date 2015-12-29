/* 
 * File:   main.cpp
 * Author: raysaoliveira
 *
 * Created on 15 de Dezembro de 2015, 19:15
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//como o incluide foi criado por mim, nao coloca <> e sim entre ""
//pra entender que está no mesmo projeto e diretorio do main.
#include "fila.h" 

#include "simulacao.h"
#include "random.h"

void inserir_utente_na_fila_fase1(struct fase *fase, int minuto){
    /*Se o minuto atual for menor que o valor 
    atual de poisson vai chegar um cliente. se não, nao chega cliente no momento.
    */
    if(minuto < poisson()) { 
        struct utente *utente = (struct utente *)malloc(sizeof(struct utente));
        utente->status_fase[fase->numero_fase].prioridade = eh_prioritario();
        /* Se só existir uma fila para a fase, todos os utentes
         * serão adicionados na fila 0, caso contrário,
         * serão adicionados na fila de mesmo número de sua prioridade
         */
        int indice_fila = 0;
        if(fase->total_filas > 1)
            indice_fila = utente->status_fase[fase->numero_fase].prioridade;
        utente->id = fase->filas[indice_fila]->total_utentes_chegados+1; 
        utente->status_fase[fase->numero_fase].tempo_chegada=minuto;
        utente->status_fase[fase->numero_fase].duracao_atendimento = 0;
        utente->status_fase[fase->numero_fase].tempo_partida = 0;
        inserir(utente, fase->filas[indice_fila]);
        imprimir_utente("inserido   na fase 1", utente, fase->numero_fase);
    }
}

void redirecionar_utente_para_fase_seguinte(struct fase *fase_seguinte, struct utente * utente, int minuto_atual){
    utente->status_fase[fase_seguinte->numero_fase].tempo_chegada=minuto_atual;
    utente->status_fase[fase_seguinte->numero_fase].tempo_inicio_atendimento = 0;
    utente->status_fase[fase_seguinte->numero_fase].prioridade = gerar_prioridade_fase2();
    utente->status_fase[fase_seguinte->numero_fase].duracao_atendimento = 0;
    utente->status_fase[fase_seguinte->numero_fase].tempo_partida = 0;
    int prioridade = utente->status_fase[fase_seguinte->numero_fase].prioridade;
    
    /*
     * Vou inserir o usuario na fila de acordo com sua prioridade
     */
    inserir(utente, fase_seguinte->filas[prioridade]); 
    char mensagem[100];
    sprintf(mensagem, "inserido   na fase %d", fase_seguinte->numero_fase);
    imprimir_utente(mensagem, utente, fase_seguinte->numero_fase);
}

void atender_utente(struct fase *fase, int minuto) {
    for(int i=0; i<fase->total_filas; i++){
        if(servidor_esta_livre(fase->servidores, i)){
            //printf("Servidor Livre: %d\n", indice_servidor);
            struct utente* utente = 
                remover_utente_da_primeira_fila_com_clientes_em_espera(fase->filas, fase->total_filas); 
            if(utente!=NULL){
                fase->servidores[i] = utente;
                utente->status_fase[fase->numero_fase].tempo_inicio_atendimento= minuto;
                utente->status_fase[fase->numero_fase].duracao_atendimento=gerar_tempo_atendimento_fase(fase);

                //imprimir_utente("início atend. fase 1", utente, FASE1);
             } //else printf("Nenhum servidor livre\n");
        }    
    }    
}

int chegou_momento_de_finalizar_atendimento_utente(int minuto_atual, struct status_fase fase){
    return minuto_atual >= fase.tempo_partida;
}

void finalizar_atendimento_utentes_fase1(struct fase *fase_atual, struct fase *fase_seguinte, int minuto_atual){
   for(int i=0; i<fase_atual->total_servidores; i++){
        struct utente * utente = fase_atual->servidores[i];
        if(utente!=NULL){
            /*
             * se o minuto atual for maior ou igual a soma do tempo de 
             * chegada com o tempo de atendimento, significa q o atendimento
             * do cliente deve ser finalizado nesta fase e redirecionado para
             * a proxima fase.
             */
            int tempo_partida = calcular_tempo_partida_na_fila_fase(utente->status_fase[FASE1]);
            if(chegou_momento_de_finalizar_atendimento_utente(minuto_atual, utente->status_fase[FASE1])){
                utente->status_fase[FASE1].tempo_partida=tempo_partida;
                fase_atual->servidores[i]=NULL;
                redirecionar_utente_para_fase_seguinte(fase_seguinte, utente, minuto_atual);
                //imprimir_utente("finalizado na fase 1", utente, FASE1);
            }

        }
    } 
}

int main(int argc, char** argv) {
    /*Parâmetros da simulação*/
    int total_servidores_fases[TOTAL_FASES] = {2, 2, 4, 2};
    int total_filas_fases[TOTAL_FASES] = {1, 4, 4, 4};
    float intervalo_medio_entre_chegadas_utentes = 8.5;
    int total_minutos_simulacao = 80;

    
    /*A seed será a hora atual, assim,
     cada rodada gerará resultados diferentes.
     Coloque uma seed fixa e verá sempre os mesmos resultados.*/
    long seed = time(NULL);
    struct fase fases[TOTAL_FASES];
    int minuto_atual, total_utentes = 0;
    
    inicializar_seed(seed);
    //Inicializa o gerador com a média desejada
    inicializar_poisson(intervalo_medio_entre_chegadas_utentes);
    inicializar_fases(fases, total_filas_fases, total_servidores_fases);        
    
    /* 
    //verifica se a media das somas bate com a media do intervalo medio entre chegadas
    int i;
    float soma = 0;
    for(i = 0; i < total_minutos; i++)
        soma += poisson();
    printf("média: %f\n\n", soma/total_minutos); 
    */
    
    for(minuto_atual = 1; minuto_atual <= total_minutos_simulacao; minuto_atual++){
        inserir_utente_na_fila_fase1(&fases[FASE1], minuto_atual);
        atender_utente(&fases[FASE1], minuto_atual);
        finalizar_atendimento_utentes_fase1(&fases[FASE1], &fases[FASE2], minuto_atual); 
    }
    
    int total_pessoas=0;
    for(int i=0;i<fases[FASE1].total_filas; i++){
        total_pessoas += fases[FASE1].filas[i]->total_utentes_chegados;
    }    
    
    printf("Total geral de pessoas que chegaram: %d\n", total_pessoas);
    printf("Pessoas na fila:\n");
    
    //Libera todas as filas criadas
    for(int i=0; i<TOTAL_FASES; i++){
        limpar_vetor_filas(fases[i].filas, fases[i].total_filas);
    }
    return 0;
}


