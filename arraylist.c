// Practica 3 se SO
// Data: 12 de decembro do 2019

// Grupo 1.3
// Brais Garc�a Brenlla ; b.brenlla ; 46097652V
// �ngel �lvarez Rey ; angel.alvarez.rey ; 21050084R
#include "arraylist.h"


/******************************SENALES ******************************************/

struct SEN{
char *nombre;
int senal;
};
static struct SEN sigstrnum[] = {
{"HUP", SIGHUP},
{"INT", SIGINT},
{"QUIT", SIGQUIT},
{"ILL", SIGILL},
{"TRAP", SIGTRAP},
{"ABRT", SIGABRT},
{"IOT", SIGIOT},
{"BUS", SIGBUS},
{"FPE", SIGFPE},
{"KILL", SIGKILL},
{"USR1", SIGUSR1},
{"SEGV", SIGSEGV},
{"USR2", SIGUSR2},
{"PIPE", SIGPIPE},
{"ALRM", SIGALRM},
{"TERM", SIGTERM},
{"CHLD", SIGCHLD},
{"CONT", SIGCONT},
{"STOP", SIGSTOP},
{"TSTP", SIGTSTP},
{"TTIN", SIGTTIN},
{"TTOU", SIGTTOU},
{"URG", SIGURG},
{"XCPU", SIGXCPU},
{"XFSZ", SIGXFSZ},
{"VTALRM", SIGVTALRM},
{"PROF", SIGPROF},
{"WINCH", SIGWINCH},
{"IO", SIGIO},
{"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
{"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
{"PWR", SIGPWR},
#endif
#ifdef SIGEMT
{"EMT", SIGEMT},
#endif
#ifdef SIGINFO
{"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
{"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
{"CLD", SIGCLD},
#endif
#ifdef SIGLOST
{"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
{"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
{"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
{"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
{"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
{"WAITING", SIGWAITING},
#endif
{NULL,-1}
}; /*fin array sigstrnum */

//##############################################################################
//##############################################################################
//##############################################################################
void estadoActual(tNodoP * n){
int estado;
    if ((n->prio=getpriority(PRIO_PROCESS,n->pid))==-1) perror("Error");
    if (n->pid == waitpid(n->pid, &estado, WNOHANG | WUNTRACED | WCONTINUED)) {
        if (WIFEXITED(estado)) strcpy(n->estado, "REMATADO NORMALMENTE");
        else if (WIFSIGNALED(estado)) {
            strcpy(n->estado, "REMATADO POR SINAL");
            strcpy(n->signal,nombreSenal(WTERMSIG(estado)));
        }
        else if (WIFSTOPPED(estado)) strcpy(n->estado, "PARADO");
        else if (WIFCONTINUED(estado)) strcpy(n->estado, "ACTIVO");
      }
}

int senal(char * sen) /*devuel el numero de senial a partir del nombre*/
{
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++) if (!strcmp(sen, sigstrnum[i].nombre))
      return sigstrnum[i].senal;
  return -1;
}

char *nombreSenal(int sen) /*devuelve el nombre senal a partir de la senal*/
{ /* para sitios donde no hay sig2str*/
  int i;
  for (i=0; sigstrnum[i].nombre!=NULL; i++) if (sen==sigstrnum[i].senal)
      return sigstrnum[i].nombre;
  return ("SIGUNKNOWN");
}
/*
--------------------------------------------------------------------------------
*/
tNodoP crearNodoP(int pid,char comand[15]){
  tNodoP n;
  n.pid = pid;
  strcpy(n.comand, comand);
  time_t tiempo = time(0);
  struct tm tlocal = *(localtime(&tiempo));
  strftime(n.time,128,"%H:%M:%S",&tlocal);
  strcpy(n.estado, "ACTIVO");
  strcpy(n.signal, "");
  return n;
}
/*
--------------------------------------------------------------------------------
*/
 void addNodoP(tListP * l, int pid, char comand[15]){

   if(l->final==MAXP){
       printf("O proceso non se puido meter na lista porque a lista está chea\n");
        return;
   }
  tNodoP n = crearNodoP(pid,comand);
  l->final++;
  l->datos[l->final]=n;
}
/*
--------------------------------------------------------------------------------
*/
void borrarNodoP(tListP *l, int mode){
  int i=0;
  while (i<=l->final){
    tNodoP n = l->datos[i];
    if (strcmp(n.estado,"ACTIVO" )==0 || strcmp(n.estado,"PARADO")==0) {estadoActual(&n); l->datos[i]=n;}
    if((strcmp(n.estado,"REMATADO NORMALMENTE")==0 && (mode == 0)) ||
              ((mode == 1) && strcmp(n.estado,"REMATADO POR SINAL")==0)) {
      printf("Borrouse da lista o pid %d\n",l->datos[i].pid);
      for(int j = i; j<l->final; j++) l->datos[j]=l->datos[j+1];
      l->final--;
    }
    else i++;
  }
}

/*
--------------------------------------------------------------------------------
*/

/*
WIFEXITED(status); //Devolve true se o proceso rematou normalmente
  WEXITSTATUS(status); //Devolve o estado de salida do proceso. So se debe usar se WIFEXITED devolveu true
WIFSIGNALED(status); //Devolve true se o proceso foi rematado por unha signal
        WTERMSIG(status); //Devolve o numero da sinal que rematou o proceso. So se debe usar se WIFSIGNALED devolveu true
  WCOREDUMP(status); //Devolve true se o proceso produciu un core dump. So se debe usar se WIFSIGNALED devolveu true
        WIFSTOPPED(status); //devolve true se o proceso foi pausado por unha sinal; Esto só é posíbel se a chamada foi feita usando WUNTRACED ou se o proceso está sendo rastreada
WSTOPSIG(status); //devolve o nº de sinal que causou a parada do proceso. So se debe usar se WIFSTOPPED devolveu true
WIFCONTINUED(status); //devolve true se o proceso fillo foi reanudado por entrega ou SIGCONT
*/


int verListaP(tListP *l, int pid){
  int found=0;
  for(int i = 0 ; i<= l->final ; i++){
    tNodoP n = l->datos[i];
    if (pid == 0 || n.pid == pid) {
      found =1;
        if (strcmp(n.estado,"ACTIVO")==0 || strcmp(n.estado,"PARADO")==0) {estadoActual(&n); l->datos[i]=n;}
        printf("%d | p=%d | inicio: %s | estado: %s " ,
          n.pid, n.prio , n.time, n.estado);
          if(strcmp(n.signal,"")!=0) printf("(con señal: %s )",n.signal );
          printf("| %s\n", n.comand);
    }
  }
  return found;
}
/*
--------------------------------------------------------------------------------
*/
void vaciarP(tListP  *l){
  for(int i= 0; i== l->final; i++) free(l->datos);
}
