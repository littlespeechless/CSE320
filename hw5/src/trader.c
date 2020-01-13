//
// Created by student on 12/3/19.
//
#include "trader_struct.h"
#include "debug.h"
#include "csapp.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
pthread_mutex_t tradeMutex;
pthread_mutexattr_t tradeAttr;
TRADER* allTrader[MAX_TRADERS]={NULL};
int trader_init(void){
    pthread_mutexattr_init(&tradeAttr);
    pthread_mutexattr_settype(&tradeAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&tradeMutex, &tradeAttr);
    info("Trader module initialized");
    return 0;
}

void trader_fini(void){
    info("Finalize trader module");
    for (int i = 0; i < MAX_TRADERS; ++i) {
        if (allTrader[i]!=NULL){
            trader_unref(allTrader[i],"Trader is shutting down");
            //check again in case 0 ref count cause double free
            if (allTrader[i]!=NULL){
                free(allTrader[i]->traderName);
                free(allTrader[i]);
            }
        }
    }
}

TRADER *trader_login(int fd, char *name){
    //CHECK USER EXIST OR NOT
    info("%s%s","Login name: ",name);
    pthread_mutex_lock(&tradeMutex);
    for (int i = 0; i < MAX_TRADERS; ++i) {
        if (allTrader[i]!=NULL){
            if (strcmp(allTrader[i]->traderName,name)==0){
                if (allTrader[i]->traderFd>0){
                    pthread_mutex_unlock(&tradeMutex);
                    return NULL;
                }
                allTrader[i]->traderFd=fd;
                trader_ref(allTrader[i],"Trader login");
                pthread_mutex_unlock(&tradeMutex);
                return allTrader[i];
            }
        }
    }
    // CHECK AVAILABLE SLOT
    int slot = -1;
    for (int i = 0; i < MAX_TRADERS; ++i) {
        if (allTrader[i]==NULL){
            slot=i;
            break;
        }
    }
    if (slot==-1){
        pthread_mutex_unlock(&tradeMutex);
        return NULL;
    }
    // add new client
    TRADER* newTrader = malloc(sizeof(struct trader));
    newTrader->balance=0;
    newTrader->inventory =0;
    newTrader->traderFd = fd;
    newTrader->traderName = malloc(strlen(name)+1);
    strcpy(newTrader->traderName,name);
    newTrader->tradeReferenceCount = 0;
    allTrader[slot] = newTrader;
    trader_ref(newTrader,"Register new trader");
    trader_ref(newTrader,"Trader login");
    pthread_mutex_unlock(&tradeMutex);
    return newTrader;
}
void trader_logout(TRADER *trader){
    pthread_mutex_lock(&tradeMutex);
    for (int i = 0; i < MAX_TRADERS; ++i) {
        if (allTrader[i]==trader){
            trader->traderFd =-1;
            trader_unref(trader,"Trader logout");
            info("%s%s%s","Trader [",trader->traderName,"] logged out");
        }
    }
    pthread_mutex_unlock(&tradeMutex);
}
TRADER *trader_ref(TRADER *trader, char *why){
    pthread_mutex_lock(&tradeMutex);
    for (int i = 0; i < MAX_TRADERS; ++i) {
        if (allTrader[i]==trader){
            allTrader[i]->tradeReferenceCount+=1;
            info("%s%s%s%d%s%d%s%s","Trader[",
                 allTrader[i]->traderName,
                 "] reference count increase from ",
                 allTrader[i]->tradeReferenceCount-1,
                 " to ",allTrader[i]->tradeReferenceCount,
                 ", reason: ",why);
            pthread_mutex_unlock(&tradeMutex);
            return allTrader[i];
        }
    }
    pthread_mutex_unlock(&tradeMutex);
    return NULL;
}
void trader_unref(TRADER *trader, char *why){
    pthread_mutex_lock(&tradeMutex);
    for (int i = 0; i < MAX_TRADERS; ++i) {
        if (allTrader[i]==trader){
            allTrader[i]->tradeReferenceCount-=1;
            info("%s%s%s%d%s%d%s%s","Trader[",
                 allTrader[i]->traderName,
                 "] reference count deceased from ",
                 allTrader[i]->tradeReferenceCount+1,
                 " to ",allTrader[i]->tradeReferenceCount,
                 ", reason: ",why);
            if (allTrader[i]->tradeReferenceCount == 0){
                free(allTrader[i]->traderName);
                free(allTrader[i]);
                allTrader[i]=NULL;
            }
            pthread_mutex_unlock(&tradeMutex);
            break;
        }
    }
}
int trader_send_packet(TRADER *trader, BRS_PACKET_HEADER *pkt, void *data){
    //check header
    if (pkt == NULL) {
        return -1;
    }
    //check payload
    if ((pkt->size != 0 && data == NULL) || (pkt->size == 0 && data != NULL)) {
        return -1;
    }
    int hasPayload = 0;
    if (pkt->size > 0) {
        hasPayload = pkt->size;
    }
    //get time
    struct timespec time;
    int getTime = clock_gettime(CLOCK_REALTIME, &time);
    if (getTime == -1) {
        return -1;
    }
    //convert byte to network byte
    pkt->size=htons(pkt->size);
    pkt->timestamp_nsec=htonl(time.tv_nsec);
    pkt->timestamp_sec=htonl(time.tv_sec);
    //write all
    int writeHeader = rio_writen(trader->traderFd,(void*)pkt, sizeof(BRS_PACKET_HEADER));
    if(writeHeader==-1){
        return -1;
    }
//    int writeHeader = write(fd,(void*)hdr, sizeof(BRS_PACKET_HEADER));
//    if (writeHeader!= sizeof(BRS_PACKET_HEADER)){
//        return -1;
//    }
    if (hasPayload){
        int writePayload = rio_writen(trader->traderFd,data,hasPayload);
        if (writePayload==-1){
            return -1;
        }
    }
    return 0;
}
int trader_broadcast_packet(BRS_PACKET_HEADER *pkt, void *data){
    pthread_mutex_lock(&tradeMutex);
    for (int i = 0; i < MAX_TRADERS; ++i) {
        if (allTrader[i]!=NULL){
            if (trader_send_packet(allTrader[i],pkt,data)==-1){
                pthread_mutex_unlock(&tradeMutex);
                return -1;
            }
        }
    }
    pthread_mutex_unlock(&tradeMutex);
    return 0;
}
int trader_send_ack(TRADER *trader, BRS_STATUS_INFO *info){
    //make a pkt
    BRS_PACKET_HEADER* ack = malloc(sizeof(BRS_PACKET_HEADER));
    memset(ack,0, sizeof(BRS_PACKET_HEADER));
    if (info!=NULL){
        //info("%s%lo","info size: ", sizeof(BRS_STATUS_INFO));
        ack->size = sizeof(BRS_STATUS_INFO);
    } else{
        ack->size=0;
    }
    ack->type = BRS_ACK_PKT;
    // send pkt
    if (trader_send_packet(trader,ack,info)==-1){
        free(ack);
        return -1;
    }
    free(ack);
    return 0;
}
int trader_send_nack(TRADER *trader){
    BRS_PACKET_HEADER* nack = malloc(sizeof(BRS_PACKET_HEADER));
    memset(nack,0, sizeof(BRS_PACKET_HEADER));
    nack->size = 0;
    nack->type = BRS_NACK_PKT;

    //sent pkt
    if (trader_send_packet(trader,nack,NULL)==-1){
        free(nack);
        return -1;
    }
    free(nack);
    return 0;
}
void trader_increase_balance(TRADER *trader, funds_t amount){
    pthread_mutex_lock(&tradeMutex);
    trader->balance+=amount;
    info("%s%s%s%d%s%d","Trader [",trader->traderName,
            "] increased balance from ",trader->balance-amount,
            " to ",trader->balance);
    pthread_mutex_unlock(&tradeMutex);
}
int trader_decrease_balance(TRADER *trader, funds_t amount){
    pthread_mutex_lock(&tradeMutex);
    if (trader->balance>=amount){
        trader->balance-=amount;
        info("%s%s%s%d%s%d","Trader [",trader->traderName,
             "] decreased balance from ",trader->balance+amount,
             " to ",trader->balance);
        pthread_mutex_unlock(&tradeMutex);
        return 0;
    } else{
        pthread_mutex_unlock(&tradeMutex);
        return -1;
    }
}
void trader_increase_inventory(TRADER *trader, quantity_t quantity){
    pthread_mutex_lock(&tradeMutex);
    trader->inventory+=quantity;
    info("%s%s%s%d%s%d","Trader [",trader->traderName,
         "] increased inventory from ",trader->inventory-quantity,
         " to ",trader->inventory);
    pthread_mutex_unlock(&tradeMutex);
}

int trader_decrease_inventory(TRADER *trader, quantity_t quantity){
    pthread_mutex_lock(&tradeMutex);
    if (trader->inventory>=quantity){
        trader->inventory-=quantity;
        info("%s%s%s%d%s%d","Trader [",trader->traderName,
             "] decreased inventory from ",trader->inventory+quantity,
             " to ",trader->inventory);
        pthread_mutex_unlock(&tradeMutex);
        return 0;
    } else{
        pthread_mutex_unlock(&tradeMutex);
        return -1;
    }
}