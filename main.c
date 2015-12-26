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

/*
 * 
 */
int main(int argc, char** argv) {
    /*A seed será a hora atual, assim,
     cada rodada gerará resultados diferentes.
     Coloque uma seed fixa e verá sempre os mesmos resultados.*/
    long seed = time(NULL);
    float intervalo_medio_entre_chegadas_utentes = 8.5;
    int minuto, total_minutos = 80, total_clientes = 0;

    utente pes;
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
        /*Se o minuto atual for menor que o valor 
        atual de poisson vai chegar um cliente. se não, nao chega cliente no momento.
        */
        if(minuto < poisson()) { 
            pes.id = f.quant_atual+1;
            pes.fase.id_fase=1; 
            pes.fase.tempo_chegada=minuto;
            pes.prioridade = eh_prioritario();
            inserir(pes, &f);
        }
    }
    
    printf("Total de pessoas que chegaram: %d\n", f.quant_atual);
    printf("Pessoas na fila:\n");
    listar(&f);
    
    
    //inicializa novamente para apagar todos os elementos da fila
    //inicializar(&f);
    return 0;
}


