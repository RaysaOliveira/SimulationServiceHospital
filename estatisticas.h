/* 
 * File:   estatisticas.h
 * Author: raysaoliveira
 *
 * Created on 2 de Janeiro de 2016, 20:52
 */

#ifndef ESTATISTICAS_H
#define	ESTATISTICAS_H

#include "tipos.h"

float media_tempo_espera_fila(simulacao *sim);
struct estatisticas calcular_estatisticas(simulacao *sim);
struct estatisticas calcular_todas_estatisticas_todas_simulacoes(struct estatisticas vetor_estatisticas[], int tamanho_vetor);
void imprimir_estatisticas_uma_simulacao(struct estatisticas *estatisticas);

#endif	/* ESTATISTICAS_H */

