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

void inserir_cliente_na_fila_fase1(fila *f, int minuto){
    /*Se o minuto atual for menor que o valor 
    atual de poisson vai chegar um cliente. se não, nao chega cliente no momento.
    */
    if(minuto < poisson()) { 
        struct utente *utente = (struct utente *)malloc(sizeof(struct utente));
        utente->id = f->total_utentes_chegados+1; 
        utente->fase[FASE1].tempo_chegada=minuto;
        utente->fase[FASE1].prioridade = eh_prioritario();
        utente->fase[FASE1].duracao_atendimento = 0;
        utente->fase[FASE1].tempo_partida = 0;
        inserir(utente, f);
        //imprimir_utente("inserido   na fase 1", utente);
    }
}

void inserir_utente_na_fila_fase2(struct utente * utente, int minuto_atual){
    utente->fase[FASE2].tempo_chegada=minuto_atual;
    utente->fase[FASE2].tempo_inicio_atendimento = 0;
    utente->fase[FASE2].prioridade = gerar_prioridade_fase2();
    utente->fase[FASE2].duracao_atendimento = 0;
    utente->fase[FASE2].tempo_partida = 0;
    int prioridade = utente->fase[FASE2].prioridade;
    /*
     * Vou inserir o usuario na fila de acordo com sua prioridade
     */
    
    //inserir(utente, filas_fase2[prioridade]); 
    imprimir_utente("inserido   na fase 2", utente, FASE2);
}

void atender_utente_fase1(fila *f, int minuto) {
    if(!vazia(f)){
        int indice_servidor = procurar_indice_servidor_livre(servidores_fase1, TOTAL_SERVIDORES_FASE1);
        if(indice_servidor >= 0){
            //printf("Servidor Livre: %d\n", indice_servidor);
            struct utente* utente = remover_inicio(f);  
            servidores_fase1[indice_servidor] = utente;
            utente->fase[FASE1].tempo_inicio_atendimento= minuto;
            utente->fase[FASE1].duracao_atendimento=gerar_tempo_atendimento_fase1();
            //imprimir_utente("início atend. fase 1", utente, FASE1);
        }
        //else printf("Nenhum servidor livre\n"); 
    }    
}

int chegou_momento_de_finalizar_atendimento_utente(int minuto_atual, struct fase fase){
    return minuto_atual >= fase.tempo_partida;
}

void finalizar_atendimento_utentes_fase1(int minuto_atual){
   for(int i=0; i<TOTAL_SERVIDORES_FASE1; i++){
        struct utente * utente = servidores_fase1[i];
        if(utente!=NULL){
            /*
             * se o minuto atual for maior ou igual a soma do tempo de 
             * chegada com o tempo de atendimento, significa q o atendimento
             * do cliente deve ser finalizado nesta fase e redirecionado para
             * a proxima fase.
             */
            int tempo_partida = calcular_tempo_partida_na_fila_fase(utente->fase[FASE1]);
            if(chegou_momento_de_finalizar_atendimento_utente(minuto_atual, utente->fase[FASE1])){
                utente->fase[FASE1].tempo_partida=tempo_partida;
                servidores_fase1[i]=NULL;
                inserir_utente_na_fila_fase2(utente, minuto_atual);
                //imprimir_utente("finalizado na fase 1", utente, FASE1);
            }

        }
    } 
}

int main(int argc, char** argv) {
    /*A seed será a hora atual, assim,
     cada rodada gerará resultados diferentes.
     Coloque uma seed fixa e verá sempre os mesmos resultados.*/
    long seed = time(NULL);
    float intervalo_medio_entre_chegadas_utentes = 8.5;
    int minuto_atual, total_minutos = 80, total_clientes = 0;

    struct utente utente;
    fila f;
    
    inicializar_fila(&f);
    
    inicializar_servidores_e_filas_todas_fases();
    inicializar_parametros_simulacao(seed);
    //Inicializa o gerador com a média desejada
    poisson_init(intervalo_medio_entre_chegadas_utentes);
    
    /* 
    //verifica se a media das somas bate com a media do intervalo medio entre chegadas
    int i;
    float soma = 0;
    for(i = 0; i < total_minutos; i++)
        soma += poisson();
    printf("média: %f\n\n", soma/total_minutos); 
    */
    
    for(minuto_atual = 1; minuto_atual <= total_minutos; minuto_atual++){
        inserir_cliente_na_fila_fase1(&f,minuto_atual);
        atender_utente_fase1(&f, minuto_atual);
        finalizar_atendimento_utentes_fase1(minuto_atual); 
      }
    
    printf("Total de pessoas que chegaram: %d\n", f.total_utentes_chegados);
    printf("Pessoas na fila:\n");
    
    
    //inicializa novamente para apagar todos os elementos da fila
    limpar_fila(&f);
    return 0;
}


