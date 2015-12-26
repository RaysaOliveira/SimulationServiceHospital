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

void inserir_cliente_na_fila_fase1(int minuto, fila *f){
    /*Se o minuto atual for menor que o valor 
    atual de poisson vai chegar um cliente. se não, nao chega cliente no momento.
    */
    if(minuto < poisson()) { 
        struct utente utente;
        utente.id = f->total_utentes_chegados+1;
        utente.fase.id_fase=1; 
        utente.fase.tempo_chegada=minuto;
        utente.prioridade = eh_prioritario();
        utente.fase.tempo_atendimento = 0;
        utente.fase.tempo_partida = 0;
        inserir(utente, f);
        imprimir_utente("Cliente inserido", utente);
    }
}

void atender_cliente_fase1(fila *f) {
    if(!vazia(f)){
        int indice_servidor = procurar_indice_servidor_livre(servidores_fase1, TOTAL_SERVIDORES_FASE1);
        if(indice_servidor >= 0){
            //printf("Servidor Livre: %d\n", indice_servidor);
            struct utente utente = remover_inicio(f);  
            servidores_fase1[indice_servidor] = &utente;
            utente.fase.tempo_atendimento=gerar_tempo_atendimento_fase1();
            imprimir_utente("Cliente atendido", utente);
        }
        //else printf("Nenhum servidor livre\n"); 
    }    
}

void finalizar_atendimento_clientes_fase1(int minuto){
   for(int i=0; i<TOTAL_SERVIDORES_FASE1; i++){
        struct utente * utente = servidores_fase1[i];
        if(utente!=NULL){
            /*
             * se o minuto atual for maior ou igual a soma do tempo de 
             * chegada com o tempo de atendimento, significa q o atendimento
             * do cliente deve ser finalizado nesta fase e redirecionado para
             * a proxima fase.
             */
            int tempo_partida = utente->fase.tempo_chegada + utente->fase.tempo_atendimento;
            if(minuto >= tempo_partida){
                utente->fase.tempo_partida=tempo_partida;
                servidores_fase1[i]=NULL;
                imprimir_utente("Cliente finalizado na fase 1", *utente);
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
    int minuto, total_minutos = 80, total_clientes = 0;

    struct utente utente;
    fila f;
    
    inicializar_fila(&f);
    
    inicializar_servidores_todas_fases();
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
    
    for(minuto = 1; minuto <= total_minutos; minuto++){
        inserir_cliente_na_fila_fase1(minuto, &f);
        atender_cliente_fase1(&f);
        finalizar_atendimento_clientes_fase1(minuto);
        
            
      }
    
    printf("Total de pessoas que chegaram: %d\n", f.quant_atual);
    printf("Pessoas na fila:\n");
    
    
    //inicializa novamente para apagar todos os elementos da fila
    limpar_fila(&f);
    return 0;
}


