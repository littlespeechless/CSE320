//
// Created by student on 11/29/19.
//
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include "client_registry.h"
#include "csapp.h"
#include "debug.h"
struct client_registry{
    int clientFd[FD_SETSIZE];
    int currentClients;
};
int waitEmptyFlag = false;
sem_t registry_sem;
pthread_mutex_t registryMutex;
pthread_mutexattr_t registryAttr;
CLIENT_REGISTRY *creg_init(){
    pthread_mutexattr_init(&registryAttr);
    pthread_mutexattr_settype(&registryAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&registryMutex, &registryAttr);
    CLIENT_REGISTRY* cr  = malloc(sizeof(CLIENT_REGISTRY));
    int sem =sem_init(&registry_sem,0,0);
    if (sem <0){
        return NULL;
    }
    for (int i = 0; i < FD_SETSIZE; ++i) {
        cr->clientFd[i]=-1;
    }
    cr->currentClients=0;
    info("client registry initialize");
    return cr;
}
void creg_fini(CLIENT_REGISTRY *cr){
    if (cr!=NULL)
        free(cr);
}
int creg_register(CLIENT_REGISTRY *cr, int fd){
    if (cr==NULL||fd>FD_SETSIZE-1){
        return -1;
    }
    pthread_mutex_lock(&registryMutex);
    if (cr->clientFd[fd]!=-1){
        pthread_mutex_unlock(&registryMutex);
        return -1;
    }
    cr->clientFd[fd]=fd;
    cr->currentClients+=1;
    pthread_mutex_unlock(&registryMutex);
    info("%s%d","Registered Client ",fd);
    info("%s%d","Total connected client: ", cr->currentClients);
    return 0;
}
int creg_unregister(CLIENT_REGISTRY *cr, int fd){
    if (cr==NULL||fd>FD_SETSIZE-1){
        return -1;
    }
    pthread_mutex_lock(&registryMutex);
    if (cr->clientFd[fd]==-1){
        pthread_mutex_unlock(&registryMutex);
        return -1;
    }
    cr->clientFd[fd]=-1;
    cr->currentClients-=1;
    close(fd);
    pthread_mutex_unlock(&registryMutex);
    info("%s%d","Unregistered Client ",fd);
    info("%s%d","Total connected client: ", cr->currentClients);
    if (cr->currentClients==0&&waitEmptyFlag==true)
        V(&registry_sem);
    return 0;
}

void creg_wait_for_empty(CLIENT_REGISTRY *cr){
    waitEmptyFlag = true;
    if(cr->currentClients==0)
        V(&registry_sem);
    P(&registry_sem);
}

void creg_shutdown_all(CLIENT_REGISTRY *cr){
    for (int i = 0; i < FD_SETSIZE; ++i) {
        if (cr->clientFd[i]!=-1){
            shutdown(cr->clientFd[i],SHUT_RD);
        }
    }
}
