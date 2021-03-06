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
#include "parametros.h"
#include "random.h"
#include "estatisticas.h"

void inserir_utente_na_fila_fase1(simulacao *sim){
    /* Se o resultado do poisson for menor que o rnd(), indica que vai chegar um cliente. 
     * Senão, nao chega cliente no momento.
    */
    if(rnd() < poisson()) { 
        struct utente *utente = criar_e_inicializar_utente(sim->total_utentes_chegaram, sim->max_consulta_medicas_por_utente);
        
        // -1 indica que nao foi definida nenhuma prioridade para esta fase
        utente->prioridade = -1;
        /* Se só existir uma fila para a fase, todos os utentes
         * serão adicionados na fila 0, caso contrário,
         * serão adicionados na fila de mesmo número de sua prioridade
         */
        int indice_fila = 0;
        if(sim->fases[0].total_filas > 1)
            indice_fila = utente->prioridade;
        utente->status_fase[0].tempo_chegada=sim->minuto_atual;
        utente->status_fase[0].tempo_inicio_atendimento = 0;
        utente->status_fase[0].duracao_atendimento = 0;
        utente->status_fase[0].tempo_partida = 0;
        inserir(utente, sim->fases[0].filas[indice_fila]);
        sim->total_utentes_chegaram++;
        if(sim->imprimir_dados_utentes_individuais)
            imprimir_utente("inserido     ", utente, 0);
    }
}

void redirecionar_utente_para_fase_seguinte(simulacao *sim, struct fase *fase_seguinte, struct utente * utente){
    utente->status_fase[fase_seguinte->numero_fase].tempo_chegada=sim->minuto_atual;
    utente->status_fase[fase_seguinte->numero_fase].tempo_inicio_atendimento = 0;
    //Se prioridade for igual a -1 indica que a prioridade nao foi definida e precisa ser definida nesta fase
    if(utente->prioridade==-1)
        utente->prioridade = gerar_prioridade(sim);
    utente->status_fase[fase_seguinte->numero_fase].duracao_atendimento = 0;
    utente->status_fase[fase_seguinte->numero_fase].tempo_partida = 0;
    int prioridade = utente->prioridade;
    
    /*
     * insere o usuario na fila de acordo com sua prioridade
     */
    inserir(utente, fase_seguinte->filas[prioridade]); 
    if(sim->imprimir_dados_utentes_individuais)
        imprimir_utente("redirecionado", utente, fase_seguinte->numero_fase);
}

/**
 * Apenas tira o utente da fila e faz com que o servidor inicie o atendimento dele.
 * @param fase
 */
void chamar_utente_para_atendimento_pelo_servidor(simulacao *sim, struct fase *fase) {    
    for(int i=0; i < fase->total_servidores; i++){
        if(servidor_esta_livre(fase->servidores, i)){
            struct utente* utente = 
                pesquisar_todas_as_filas_e_remover_utente_maior_prioridade(fase); 
            //se algum cliente foi chamdo da fila para iniciar atendimento pelo servidor
            if(utente!=NULL){
                //atribui o utente ao servidor para indicar que o servidor agora está ocupado
                fase->servidores[i] = utente;
                utente->status_fase[fase->numero_fase].tempo_inicio_atendimento = sim->minuto_atual;
                utente->status_fase[fase->numero_fase].duracao_atendimento = gerar_duracao_atendimento(fase);
                if(sim->imprimir_dados_utentes_individuais)
                    imprimir_utente("início atend.", utente, fase->numero_fase);
             } 
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
    return minuto_atual >= calcular_tempo_partida_do_utente_na_fila(status_fase);
}

/**
 * Após o utente passar por todas as fases do antedimento, ele sai do sistema
 * e é inserido na fila de finalizados para permitir computar
 * estatísticas ao final da simulação
 * @param sim
 * @param fase_atual
 * @param utente
 */
void inserir_utente_fila_finalizados(simulacao *sim, struct fase *fase_atual, struct utente *utente){
    inserir(utente, sim->fila_utentes_finalizados); 
    if(sim->imprimir_dados_utentes_individuais)
        imprimir_utente("fila finaliz.", utente, fase_atual->numero_fase);
}

/**
 * Verifica se o utente passado deve ser redirecionado para a fase de exames.
 * Caso seja, redireciona o mesmo.
 * @param sim
 * @param fase_atual Fase atual da simulação
 * @param utente Utente a ser redirecionado para a fase de exames
 */
void verificar_e_redirecionar_utente_fase_exame(simulacao *sim, struct fase *fase_atual, struct utente *utente){
    int idx_medico_atual = utente->total_atendimentos_concluidos;
    int total_exames_utente = gerar_total_exames(sim);
    utente->exames_medicos[idx_medico_atual] = total_exames_utente;
    /*Se o utente não tem exames, ele sai do sistema
     e é guardado na fila de finalizados para 
     computar estatísticas no final da simulaçao*/
    if(total_exames_utente==0){
        /*como o utente não fará exames, ele não
         retornan ao médico. Assim, seta o retorno 
         para o médico atual como zero para indicar que ele já 
         passou pelo médico mas náo retorna.*/
        utente->retorno_medicos[idx_medico_atual] = 0;
        utente->total_atendimentos_concluidos++;
        /*Redireciona o utente para uma fila da mesma fase para
          que ele aguarde atendimento de outro médico*/
        if(vai_para_outro_medico(sim, utente)) 
            redirecionar_utente_para_fase_seguinte(sim, fase_atual, utente); 
        else inserir_utente_fila_finalizados(sim, fase_atual, utente);
    } else {
        struct fase *fase_seguinte = &sim->fases[fase_atual->numero_fase+1];
        redirecionar_utente_para_fase_seguinte(sim, fase_seguinte, utente); 
    }
}

void finalizar_atendimento_utente_fase_medico(simulacao *sim, struct fase *fase_atual, struct utente *utente){
    int idx_medico_atual = utente->total_atendimentos_concluidos;
    /*Se ainda náo foi definida a especialidade para qual o utente vai
     * para a consulta atual, define agora*/
    if(utente->especialidades_medicas_consultadas[idx_medico_atual] == -1)
        utente->especialidades_medicas_consultadas[idx_medico_atual] = escolher_especialidade(sim);

    //se está retornando ao médico atual
    if(utente->retorno_medicos[idx_medico_atual]==1){
        utente->total_atendimentos_concluidos++;
        if(vai_para_outro_medico(sim, utente))
            redirecionar_utente_para_fase_seguinte(sim, fase_atual, utente);
        else inserir_utente_fila_finalizados(sim, fase_atual, utente); 
    }
    else verificar_e_redirecionar_utente_fase_exame(sim, fase_atual, utente);
}

void finalizar_atendimento_utente_fase_exame(simulacao *sim, struct fase *fase_atual, struct utente *utente){
    int idx_medico_atual = utente->total_atendimentos_concluidos;
    /*indica que o utente agora vai retornar ao médico após ter finalizado os exames
    solicitados por ele*/
    utente->retorno_medicos[idx_medico_atual] = 1;
    struct fase *fase_anterior = &sim->fases[fase_atual->numero_fase-1];
    redirecionar_utente_para_fase_seguinte(sim, fase_anterior, utente);         
}

/**
 * Procura utentes que já chegaram no momento de finalizar o atendimento e move
 * eles para a próxima fase
 * @param fase_atual Fase atual que deseja-se verificar se há utentes que deve-se finalizar atendimento
 * @param minuto_atual Minuto atual do clock da simulação.
 */
void finalizar_atendimento_utentes(simulacao * sim, struct fase *fase_atual){
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
            struct status_fase status_fase_utente = utente->status_fase[fase_atual->numero_fase];
            if(chegou_momento_de_finalizar_atendimento_utente(sim->minuto_atual, status_fase_utente)){
                int tempo_partida = calcular_tempo_partida_do_utente_na_fila(status_fase_utente);
                utente->status_fase[fase_atual->numero_fase].tempo_partida=tempo_partida;
                //indica que o servidor agora está livre pois o utente terminou de ser atendido
                fase_atual->servidores[i]=NULL;
                if(sim->imprimir_dados_utentes_individuais)
                    imprimir_utente("finalizado   ", utente, fase_atual->numero_fase);
                /*Se a fase atual for menor ou igual a 1, a fase seguinte
                 nestes casos será sempre a ase_atual->numero_fase + 1*/
                if(fase_atual->numero_fase <= 1) {
                    struct fase *fase_seguinte = &sim->fases[fase_atual->numero_fase+1];
                    redirecionar_utente_para_fase_seguinte(sim, fase_seguinte, utente);
                }
                else if(fase_atual->numero_fase==2)
                    finalizar_atendimento_utente_fase_medico(sim, fase_atual, utente);
                else if(fase_atual->numero_fase==3)
                    finalizar_atendimento_utente_fase_exame(sim, fase_atual, utente);
            }
        }
    } 
}

void chamar_utentes_em_espera_e_finalizar_atendimento_dos_utentes(simulacao *sim){
    for(int num_fase=0; num_fase < TOTAL_FASES; num_fase++){
        chamar_utente_para_atendimento_pelo_servidor(sim, &sim->fases[num_fase]);
        finalizar_atendimento_utentes(sim, &sim->fases[num_fase]); 
    }
}

/**
 * Função temporária apenas para verificar números de poisson
 */
int testar_poisson_e_finalizar_programa_imediatamente(){
    //verifica se a média das somas das chamadas de poisson() bate com a média do intervalo médio entre chegadas
    int total_minutos = 360, total_chegados=0;
    float inter_chegada = 8;
    long seed = time(NULL);
    inicializar_seed(seed);
    inicializar_poisson(inter_chegada);
    float soma = 0, num_poisson, num_uniforme;
    for(int minuto = 1; minuto <= total_minutos; minuto++) {
        num_poisson = poisson();
        num_uniforme = rnd();
        if(num_uniforme < num_poisson){
            total_chegados++;
            printf("minuto: %3d uniforme: %.2f poisson: %.2f soma: %f - chegou\n", minuto, num_uniforme, num_poisson, soma); 
        } else printf("minuto: %3d uniforme: %.2f poisson: %.2f soma: %f\n", minuto, num_uniforme, num_poisson, soma); 
        soma += num_poisson;
        
    }
    float media = soma/total_minutos * 100;
    printf("média: %f total utentes: %d tempo simulação: %d\n\n", media, total_chegados, total_minutos);  
    
    exit(0);
}

int main(int argc, char** argv) {  
    //testar_poisson_e_finalizar_programa_imediatamente();
    
    struct simulacao sim;
    /*Parâmetros para todas as simulações*/
    sim.max_consulta_medicas_por_utente=2;
    sim.intervalo_medio_entre_chegadas_utentes = 8.0;
    sim.total_minutos_simulacao = 80;
    sim.total_simulacoes = 1;
    sim.probabilidade_de_utente_consultar_com_segundo_medico = 0.5;
    sim.imprimir_dados_utentes_individuais = 0;
    int total_servidores_fases[TOTAL_FASES] = {2, 2, 4, 2};
    int total_filas_fases[TOTAL_FASES] = {1, 4, 4, 4};
    int tempo_max_atendimento_fases[TOTAL_FASES] = {8, 15, 40, 50};
    float probabilidades_prioridades[4] = {0.1, 0.3, 0.6, 1.0};
    float probabilidades_especialidade_medica[4] = {0.1, 0.3, 0.6, 1.0};

    /*Carrega os parâmetros do arquivo.
     Se o arquivo não existir ou determinados parâmetros
     não forem setados, os valores para os parâmetros em falta
     serão os setados por default nas variáveis acima*/
    carregar_parametros_arquivo(&sim, 
            total_filas_fases, total_servidores_fases, tempo_max_atendimento_fases,
            probabilidades_prioridades, probabilidades_especialidade_medica);

    //armazenas as estatísticas para cada simulação
    struct estatisticas vetor_estatisticas[sim.total_simulacoes];
    
    /*Estatísticas finais para todas as simulações.
     Armazena dados como a média das médias de cada simulação.*/
    struct estatisticas estatisticas_finais;
    
    for(int i = 0; i < sim.total_simulacoes; i++){
        /*A seed será a hora atual, assim,
         cada rodada gerará resultados diferentes.
         Coloque uma seed fixa e verá sempre os mesmos resultados.
         Multiplica a seed por i para garantir que cada simulação terá uma seed
         diferente.*/
        sim.seed = time(NULL)*(i+1);

        //Pega os parâmetros das variáveis acima e seta de fato na struct simulacao
        inicializar_simulacao(&sim, 
                total_filas_fases, total_servidores_fases, tempo_max_atendimento_fases,
                probabilidades_prioridades, probabilidades_especialidade_medica);        
        
        //só imprime os parâmetros para a 1a simulação, pois serão iguais para todas
        if(i == 0) {
            imprimir_parametros_simulacao(&sim);
        }
        printf("Iniciada simulação %d de %d - Seed: %ld\n", i+1, sim.total_simulacoes, sim.seed);

        for(sim.minuto_atual = 1; sim.minuto_atual <= sim.total_minutos_simulacao; sim.minuto_atual++){
            inserir_utente_na_fila_fase1(&sim);
            chamar_utentes_em_espera_e_finalizar_atendimento_dos_utentes(&sim);
        }   
        printf("Recepção de novos utentes encerrada no minuto %d. Somente os utentes atuais serão atendidos\n", sim.minuto_atual);

        /*Enquanto houver utentes na fila ou sendo atendidos pelos servidores,
         continua a simulação até que todos os utentes tenham terminado de ser atendidos*/
        while(total_utentes_atualmente_em_fila_em_todas_as_fases(sim.fases) > 0 ||
                total_utentes_em_atendimento_em_todas_fases(sim.fases) > 0){
            ++sim.minuto_atual;
            chamar_utentes_em_espera_e_finalizar_atendimento_dos_utentes(&sim);
        }

        printf("Finalização do atendimento de todos os utentes no minuto %d\n", sim.minuto_atual);
                
        //printf("\nUtentes finalizados\n");
        //listar(sim.fila_utentes_finalizados);
        
        vetor_estatisticas[i] = calcular_estatisticas(&sim);
        imprimir_estatisticas_uma_simulacao(&vetor_estatisticas[i]);

        liberar_filas_servidores_e_utentes_simulacao(&sim);
        printf("---------------------------------------------------------------------------\n\n");
    }
    
    //só exibe os dados gerais para todas as simulações se rodar mais de uma simulação
    if(sim.total_simulacoes > 1){
        estatisticas_finais = 
                calcular_todas_estatisticas_todas_simulacoes(vetor_estatisticas, sim.total_simulacoes);
        printf("\nESTATÍSTICAS FINAIS PARA TODAS AS SIMULAÇÕES\n");
        imprimir_estatisticas_uma_simulacao(&estatisticas_finais);
        printf("\n\tTempo da simulação por rodada: %d minutos \n", sim.total_minutos_simulacao);
        printf("\tTempo total de simulação: %d minutos \n", sim.total_minutos_simulacao * sim.total_simulacoes);
    }
    return 0;
}


