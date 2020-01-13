//
// Created by student on 12/6/19.
//
#include "exchange_struct.h"
#include <stdlib.h>
#include "trader_struct.h"
#include "debug.h"
#include "csapp.h"
#include "matchmaking.h"
pthread_t pid;
pthread_mutex_t exchangeMutex;
pthread_mutexattr_t exchangeAttr;
EXCHANGE *exchange_init(){
    pthread_mutexattr_init(&exchangeAttr);
    pthread_mutexattr_settype(&exchangeAttr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&exchangeMutex, &exchangeAttr);
    sem_init(&exchangeSem,0,0);
    EXCHANGE* ex = malloc(sizeof(EXCHANGE));
    TOTAL_BIDS* bids = malloc(sizeof(TOTAL_BIDS));
    memset(bids,0, sizeof(TOTAL_BIDS));
    if (bids==NULL){
        return  NULL;
    }
    bids->trader=NULL;
    bids->nextBids=NULL;
    bids->price=0;
    bids->quantity=0;
    bids->orderid =0;

    TOTAL_ASKS* asks = malloc(sizeof(TOTAL_ASKS));
    if (asks==NULL){
        return  NULL;
    }
    memset(asks,0, sizeof(TOTAL_ASKS));
    asks->trader=NULL;
    asks->nextAsk=NULL;
    asks->price=0;
    asks->quantity=0;
    asks->orderid=0;

    ex->last =0;
    ex->bids = bids;
    ex->asks = asks;
    ex->orderid = 0;
    info("Initialize Exchange Service");
    pthread_create(&pid, NULL, match_making, NULL);
    return ex;
}
void exchange_fini(EXCHANGE *xchg){
    info("Finalize Exchange Service");
    matchStatus = false;
    V(&matchMutex);
    P(&exchangeSem);
    TOTAL_BIDS * bids = xchg->bids;
    if (bids->trader!=NULL){
        info("Cancel all pending bids");
        quantity_t quantity = 0;
        exchange_cancel(xchg,bids->trader,bids->orderid,&quantity);
        bids = xchg->bids;
    }
    TOTAL_ASKS* asks = xchg->asks;
    if (asks->trader!=NULL){
        info("Cancel all pending asks");
        quantity_t quantity=0;
        exchange_cancel(xchg,asks->trader,asks->orderid,&quantity);
        asks = xchg->asks;
    }
    bids = xchg->bids;
    while (bids!=NULL){
        TOTAL_BIDS* cursor = bids->nextBids;
        bids->trader=NULL;
        bids->nextBids=NULL;
        free(bids);
        bids = cursor;
    }
    asks=xchg->asks;
    while (asks!=NULL){
        TOTAL_ASKS* cursor = asks->nextAsk;
        asks->trader=NULL;
        asks->nextAsk=NULL;
        free(asks);
        asks = cursor;
    }
    free(xchg);
}

void exchange_get_status(EXCHANGE *xchg, BRS_STATUS_INFO *infop){
    infop->bid = htonl(xchg->bids->price);
    infop->last= htonl(xchg->last);
    infop->ask = htonl(xchg->asks->price);
    info("get exchange_get_status");
}
orderid_t exchange_post_buy(EXCHANGE *xchg, TRADER *trader, quantity_t quantity,
                            funds_t price){
    if (trader->balance>=(quantity*price)){
        pthread_mutex_lock(&exchangeMutex);
        xchg->orderid+=1;
        TOTAL_BIDS* newBid = malloc(sizeof(TOTAL_BIDS));
        newBid->price=price;
        newBid->quantity=quantity;
        newBid->trader = trader;
        newBid->orderid = xchg->orderid;
        //insert in correct position
        if(newBid->price>=xchg->bids->price){
            newBid->nextBids = xchg->bids;
            xchg->bids = newBid;
        }else{
            TOTAL_BIDS* cursor = xchg->bids->nextBids;
            TOTAL_BIDS* prev = xchg->bids;
            while(cursor!=NULL){
                if(newBid->price>=cursor->price){
                    prev->nextBids = newBid;
                    newBid->nextBids =cursor;
                    break;
                }
                prev = cursor;
                cursor = cursor->nextBids;
            }
        }
        trader_decrease_balance(trader,quantity*price);
        trader_ref(trader, "placed new buy order");
        //run exchange
        V(&matchMutex);
        //post pkt
        BRS_PACKET_HEADER* boughtHdr = malloc(sizeof(BRS_PACKET_HEADER));
        memset(boughtHdr,0, sizeof(BRS_PACKET_HEADER));
        boughtHdr->type = BRS_POSTED_PKT;
        boughtHdr->size = sizeof(BRS_NOTIFY_INFO);
        BRS_NOTIFY_INFO* notifyInfo = malloc(sizeof(BRS_NOTIFY_INFO));
        notifyInfo->buyer = htonl(xchg->orderid);
        notifyInfo->quantity = htonl(quantity);
        notifyInfo->price =htonl(price);
        notifyInfo->seller =htonl(0);
        if (trader_broadcast_packet(boughtHdr, notifyInfo) == -1){
            free(boughtHdr);
            free(notifyInfo);
            return -1;
        }
        free(boughtHdr);
        free(notifyInfo);
        pthread_mutex_unlock(&exchangeMutex);
        return xchg->orderid;
    }
    return 0;
}
orderid_t exchange_post_sell(EXCHANGE *xchg, TRADER *trader, quantity_t quantity,
                             funds_t price){
    if (trader->inventory>=quantity){
        pthread_mutex_lock(&exchangeMutex);
        xchg->orderid+=1;
        TOTAL_ASKS* newAsk = malloc(sizeof(TOTAL_ASKS));
        memset(newAsk,0, sizeof(TOTAL_ASKS));
        newAsk->price=price;
        newAsk->quantity=quantity;
        newAsk->trader = trader;
        newAsk->orderid = xchg->orderid;
        //insert in correct position
        if(newAsk->price>=xchg->asks->price){
            newAsk->nextAsk = xchg->asks;
            xchg->asks = newAsk;
        }else{
            TOTAL_ASKS* cursor = xchg->asks->nextAsk;
            TOTAL_ASKS* prev = xchg->asks;
            while(cursor!=NULL){
                if(newAsk->price>=cursor->price){
                    prev->nextAsk = newAsk;
                    newAsk->nextAsk =cursor;
                    break;
                }
                prev = cursor;
                cursor = cursor->nextAsk;
            }
        }
        trader_decrease_inventory(trader, quantity);
        trader_ref(trader,"placed new sell order");
        V(&matchMutex);
        //post pkt
        BRS_PACKET_HEADER* soldHdr = malloc(sizeof(BRS_PACKET_HEADER));
        memset(soldHdr,0, sizeof(BRS_PACKET_HEADER));
        soldHdr->type = BRS_POSTED_PKT;
        soldHdr->size = sizeof(BRS_NOTIFY_INFO);
        BRS_NOTIFY_INFO* notifyInfo = malloc(sizeof(BRS_NOTIFY_INFO));
        notifyInfo->seller = htonl(xchg->orderid);
        notifyInfo->quantity = htonl(quantity);
        notifyInfo->price =htonl(price);
        notifyInfo->buyer =htonl(0);
        if (trader_broadcast_packet(soldHdr, notifyInfo) == -1){
            free(soldHdr);
            free(notifyInfo);
            return -1;
        }
        free(soldHdr);
        free(notifyInfo);
        pthread_mutex_unlock(&exchangeMutex);
        return xchg->orderid;
    }
    return 0;
}
int exchange_cancel(EXCHANGE *xchg, TRADER *trader, orderid_t order,
                    quantity_t *quantity){
    pthread_mutex_lock(&exchangeMutex);
    //check buy
    TOTAL_BIDS* allBids = xchg->bids;
    TOTAL_BIDS* prev = NULL;
    while(allBids->trader!=NULL){
        if (allBids->orderid==order&&allBids->trader==trader){
            *quantity = allBids->quantity;
            trader_increase_balance(trader,*quantity * allBids->price);
            trader_unref(trader,"buy order canceled");
            if (prev!=NULL){
                prev->nextBids = allBids->nextBids;
                allBids->trader=NULL;
                allBids->nextBids=NULL;
                free(allBids);
            } else{
                xchg->bids=xchg->bids->nextBids;
                allBids->trader=NULL;
                allBids->nextBids=NULL;
                free(allBids);
            }
            BRS_PACKET_HEADER* cancelHdr = malloc(sizeof(BRS_PACKET_HEADER));
            memset(cancelHdr,0, sizeof(BRS_PACKET_HEADER));
            cancelHdr->type = BRS_CANCELED_PKT;
            cancelHdr->size = sizeof(BRS_NOTIFY_INFO);
            BRS_NOTIFY_INFO* notifyInfo = malloc(sizeof(BRS_NOTIFY_INFO));
            notifyInfo->buyer = htonl(order);
            notifyInfo->quantity = htonl(*quantity);
            notifyInfo->price =htonl(0);
            notifyInfo->seller =htonl(0);
            if (trader_broadcast_packet(cancelHdr,notifyInfo)==-1){
                free(cancelHdr);
                free(notifyInfo);
                return -1;
            }
            free(cancelHdr);
            free(notifyInfo);
            pthread_mutex_unlock(&exchangeMutex);
            return 0;
        }
        prev = allBids;
        allBids = allBids->nextBids;
    }
    //check sell
    TOTAL_ASKS* allAsks = xchg->asks;
    TOTAL_ASKS* prevAsk = NULL;
    while(allAsks->trader!=NULL){
        if (allAsks->orderid==order&&allAsks->trader==trader){
            *quantity = allAsks->quantity;
            trader_increase_inventory(trader,*quantity);
            trader_unref(trader,"sell order canceled");
            if (prevAsk!=NULL){
                prevAsk->nextAsk = allAsks->nextAsk;
                allAsks->trader=NULL;
                allAsks->nextAsk=NULL;
                free(allAsks);
            } else{
                xchg->asks=xchg->asks->nextAsk;
                allAsks->trader=NULL;
                allAsks->nextAsk=NULL;
                free(allAsks);
            }
            BRS_PACKET_HEADER* cancelHdr = malloc(sizeof(BRS_PACKET_HEADER));
            memset(cancelHdr,0, sizeof(BRS_PACKET_HEADER));
            cancelHdr->type = BRS_CANCELED_PKT;
            cancelHdr->size = sizeof(BRS_NOTIFY_INFO);
            BRS_NOTIFY_INFO* notifyInfo = malloc(sizeof(BRS_NOTIFY_INFO));
            notifyInfo->seller = htonl(order);
            notifyInfo->quantity = htonl(*quantity);
            notifyInfo->price =htonl(0);
            notifyInfo->buyer =htonl(0);
            if (trader_broadcast_packet(cancelHdr,notifyInfo)==-1){
                free(cancelHdr);
                free(notifyInfo);
                return -1;
            }
            free(cancelHdr);
            free(notifyInfo);
            pthread_mutex_unlock(&exchangeMutex);
            return 0;
        }
        prevAsk = allAsks;
        allAsks = allAsks->nextAsk;
    }
    return -1;
}