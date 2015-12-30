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
        // -1 indica que nao foi definida nenhuma prioridade para esta fase
        utente->prioridade = -1;
        /* Se só existir uma fila para a fase, todos os utentes
         * serão adicionados na fila 0, caso contrário,
         * serão adicionados na fila de mesmo número de sua prioridade
         */
        int indice_fila = 0;
        if(fase->total_filas > 1)
            indice_fila = utente->prioridade;
        utente->id = fase->filas[indice_fila]->total_utentes_chegados; 
        utente->status_fase[fase->numero_fase].tempo_chegada=minuto;
        utente->status_fase[fase->numero_fase].tempo_inicio_atendimento = 0;
        utente->status_fase[fase->numero_fase].duracao_atendimento = 0;
        utente->status_fase[fase->numero_fase].tempo_partida = 0;
        inserir(utente, fase->filas[indice_fila]);
        imprimir_utente("inserido     ", utente, fase->numero_fase);
    }
}

void redirecionar_utente_para_fase_seguinte(struct fase *fase_seguinte, struct utente * utente, int minuto_atual){
    utente->status_fase[fase_seguinte->numero_fase].tempo_chegada=minuto_atual;
    utente->status_fase[fase_seguinte->numero_fase].tempo_inicio_atendimento = 0;
    //Se prioridade for igual a -1 indica que a prioridade nao foi definida e precisa ser definida nesta fase
    if(utente->prioridade==-1)
        utente->prioridade = gerar_prioridade();
    utente->status_fase[fase_seguinte->numero_fase].duracao_atendimento = 0;
    utente->status_fase[fase_seguinte->numero_fase].tempo_partida = 0;
    int prioridade = utente->prioridade;
    
    /*
     * Vou inserir o usuario na fila de acordo com sua prioridade
     */
    inserir(utente, fase_seguinte->filas[prioridade]); 
    imprimir_utente("inserido     ", utente, fase_seguinte->numero_fase);
}

void atender_utente(struct fase *fase, int minuto) {    
    for(int i=0; i < fase->total_servidores; i++){
        if(servidor_esta_livre(fase->servidores, i)){
            //printf("servidor %d está livre no minuto %d\n", i, minuto);
            //printf("Servidor Livre: %d\n", indice_servidor);
            struct utente* utente = 
                remover_utente_da_primeira_fila_com_utentes_em_espera(fase); 
            //se algum cliente foi chamdo da fila para iniciar atendimento pelo servidor
            if(utente!=NULL){
                //atribui o utente ao servidor para indicar que o servidor agora está ocupado
                fase->servidores[i] = utente;
                //printf("utente %d sendo atendido por servidor %d no minuto %d\n", utente->id, i, minuto);
                utente->status_fase[fase->numero_fase].tempo_inicio_atendimento = minuto;
                utente->status_fase[fase->numero_fase].duracao_atendimento=gerar_tempo_atendimento_fase(fase);

                imprimir_utente("início atend.", utente, FASE1);
             } //else printf("Nehum utente restante na fila no minuto %d\n", minuto);
        }    
    }    
}

/**
 * Verifica se chegou o momento de finalizar o antedimento do utente
 * @param minuto_atual Minuto atual do clock da simulação
 * @param status_fase Status da fase em que o utente está
 * @return 1 (true) se chegou o momento de finalizar o atendimento do cliente,
 * caso contrário retorna 0 (false)
 */
int chegou_momento_de_finalizar_atendimento_utente(int minuto_atual, struct status_fase status_fase){
    return minuto_atual >= calcular_tempo_partida_na_fila_fase(status_fase);
}

void finalizar_atendimento_utentes_fase(struct fase *fase_atual, struct fase *fase_seguinte, int minuto_atual){
   for(int i=0; i<fase_atual->total_servidores; i++){
        struct utente * utente = fase_atual->servidores[i];
        //verifica se o servidor está atendendo algum utente
        if(utente!=NULL){
            /*
             * se o minuto atual for maior ou igual a soma do tempo de 
             * chegada com o tempo de atendimento, significa q o atendimento
             * do cliente deve ser finalizado nesta fase e redirecionado para
             * a proxima fase.
             */
            int tempo_partida = calcular_tempo_partida_na_fila_fase(utente->status_fase[fase_atual->numero_fase]);
            if(chegou_momento_de_finalizar_atendimento_utente(minuto_atual, utente->status_fase[fase_atual->numero_fase])){
                utente->status_fase[fase_atual->numero_fase].tempo_partida=tempo_partida;
                fase_atual->servidores[i]=NULL;
                imprimir_utente("finalizado   ", utente, fase_atual->numero_fase);
                redirecionar_utente_para_fase_seguinte(fase_seguinte, utente, minuto_atual);
            }

        }
    } 
}

int main(int argc, char** argv) {
    /*Parâmetros da simulação*/
    int total_servidores_fases[TOTAL_FASES] = {2, 2, 4, 2};
    int total_filas_fases[TOTAL_FASES] = {1, 4, 4, 4};
    int tempo_max_atendimento_fases[TOTAL_FASES]={10, 20, 40, 50};
    double intervalo_medio_entre_chegadas_utentes = 8.5;
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
    inicializar_fases(fases, total_filas_fases, total_servidores_fases, tempo_max_atendimento_fases);        
    
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
        //printf("main: utente %d sendo atendido por servidor %d no minuto %d\n\n", fases[FASE1].servidores[0]->id , 0, minuto_atual);
        finalizar_atendimento_utentes_fase(&fases[FASE1], &fases[FASE2], minuto_atual); 
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


