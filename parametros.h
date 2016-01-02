/* 
 * File:   parametros.h
 * Author: raysaoliveira
 *
 * Created on 2 de Janeiro de 2016, 16:24
 */

#ifndef PARAMETROS_H
#define	PARAMETROS_H

#include <stdlib.h>
#include <stdio.h>
#include "simulacao.h"
#include "tipos.h"

/**
 * Tenta abrir o arquivo de parâmetros da simulação na pasta atual.
 * Se o arquivo não existir, tenta abrir ele em 3 pastas acima do nível
 * de hierarquia de diretórios (para procurar o arquivo na pasta
 * de código fonte do projeto)
 * @param nome_arquivo
 * @return 
 */

FILE * abrir_arquivo(char *nome_arquivo);
int strpos(char *haystack, char *needle);
float obter_valor_parametro_float(char *linha_arquivo, char *nome_parametro);
void carregar_parametros_arquivo(simulacao *sim,
        int total_filas_fase[TOTAL_FASES], 
        int total_servidores_fase[TOTAL_FASES],
        int tempo_max_atendimento_fases[TOTAL_FASES],
        float probabilidades_prioridades[TOTAL_PRIORIDADES], 
        float probabilidades_especialidade_medica[TOTAL_ESPECIALIDADES_MEDICAS]);

#endif	/* PARAMETROS_H */

