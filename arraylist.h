// Practica 3 se SO
// Data: 12 de decembro do 2019

// Grupo 1.3
// Brais Garc�a Brenlla ; b.brenlla ; 46097652V
// �ngel �lvarez Rey ; angel.alvarez.rey ; 21050084R
#define MAXP 512
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>




typedef struct tNodoP{
  int pid;
  char comand[MAXP];
  char time[128];
  char estado[MAXP];
  char signal[MAXP];
  int prio;
} tNodoP;

typedef struct tListP{
  tNodoP datos[MAXP];
  int final;
} tListP;



//##############################################################################
//##############################################################################
//##############################################################################

int senal(char * sen);
char *nombreSenal(int sen);
tNodoP crearNodoP(int pid,char comand[15]);
void addNodoP(tListP  *l, int pid, char comand[15]);
void borrarNodoP(tListP  *l, int pid);
void vaciarP(tListP  *l);
int verListaP(tListP *l, int pid);
