#include <pthread.h>

#include "pss.h"
#include "pedir.h"


// Defina aca sus variables globales
pthread_mutex_t m;
pthread_cond_t c;

int ticketDistribution[2];
int ticketDisplay[2];
int requestingUsage[2];
int currentUsage;
int busy;

void iniciar() {
  pthread_mutex_init(&m, NULL);
  pthread_cond_init(&c, NULL);
  ticketDisplay[0] = 0;
  ticketDisplay[1] = 0;
  ticketDistribution[0] = 0;
  ticketDistribution[1] = 0;
  requestingUsage[0] = 0;
  requestingUsage[1] = 0;
  busy = 0;
}

void terminar() {
  pthread_mutex_destroy(&m);
  pthread_cond_destroy(&c);
  ticketDisplay[0] = 0;
  ticketDisplay[1] = 0;
  ticketDistribution[0] = 0;
  ticketDistribution[1] = 0;
  requestingUsage[0] = 0;
  requestingUsage[1] = 0;
  busy = 0;
}

void pedir(int cat) {
  pthread_mutex_lock(&m);
  int ticket = ticketDistribution[cat]++;
  requestingUsage[cat]++;

  while (busy || (requestingUsage[!cat] > 0 && currentUsage == cat)|| ticket != ticketDisplay[cat]) {
    pthread_cond_wait(&c, &m);
  }
  
  busy = 1;
  currentUsage = cat;
  pthread_mutex_unlock(&m);
}

void devolver() {
  pthread_mutex_lock(&m);
  
  ticketDisplay[currentUsage]++;
  requestingUsage[currentUsage]--;
  busy = 0;

  pthread_cond_broadcast(&c);
  pthread_mutex_unlock(&m);
}