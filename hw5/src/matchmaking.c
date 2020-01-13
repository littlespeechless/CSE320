//
// Created by student on 12/6/19.
//
#include "matchmaking.h"
#include <semaphore.h>
#include <stdbool.h>
#include "debug.h"
#include "csapp.h"
#include "server.h"
#include "exchange_struct.h"
void* match_making(void* ptr){
    Pthread_detach(pthread_self());
    info("%s%p","Matchmaking thread initialized for exchange: ",exchange);
    sem_init(&matchMutex,0,0);
    matchStatus = true;
    while(matchStatus){
        info("%s%p%s","Matchmaking Thread for exchange: ",exchange," sleeping");
        P(&matchMutex);
        info("%s%p%s","Matchmaking Thread for exchange: ",exchange," active");
        if (matchStatus==false){
            break;
        }
        if(exchange->bids->trader==NULL||exchange->asks->trader==NULL){
            info("%s%p%s","Matchmaking Thread for exchange: ",exchange," see no possible trade");
        } else{
            //check buy
            TOTAL_BIDS* bidCursor = exchange->bids;
            //TOTAL_BIDS* prevBid = NULL;
            TOTAL_ASKS* askCursor = exchange->asks;
            TOTAL_ASKS* prevAsk = NULL;
            while (bidCursor->trader!=NULL){
                while (askCursor->trader!=NULL){
                    if (bidCursor->price>=askCursor->price){
                        // valid trad
                        if(bidCursor->quantity>=askCursor->quantity){
                            //sent bought pkt
                            trader_increase_inventory(bidCursor->trader,askCursor->quantity);
                            if (bidCursor->price>askCursor->price){
                                //return excess money
                                trader_increase_balance(bidCursor->trader,
                                        (bidCursor->price-askCursor->price)*(askCursor->quantity));
                            }
                            BRS_PACKET_HEADER* boughtHdr = malloc(sizeof(BRS_PACKET_HEADER));
                            memset(boughtHdr,0, sizeof(BRS_PACKET_HEADER));
                            boughtHdr->type = BRS_BOUGHT_PKT;
                            boughtHdr->size = sizeof(BRS_NOTIFY_INFO);
                            BRS_NOTIFY_INFO* boughtInfo = malloc(sizeof(BRS_NOTIFY_INFO));
                            boughtInfo->buyer =htonl(bidCursor->orderid);
                            boughtInfo->seller = htonl(askCursor->orderid);
                            boughtInfo->quantity = htonl(askCursor->quantity);
                            boughtInfo->price =htonl(askCursor->price);
                            if (trader_broadcast_packet(boughtHdr, boughtInfo) == -1){
                                free(boughtHdr);
                                free(boughtInfo);
                                break;
                            }
                            free(boughtHdr);
                            free(boughtInfo);
                            //sent sold pkt
                            trader_increase_balance(askCursor->trader,askCursor->price*askCursor->quantity);
                            BRS_PACKET_HEADER* soldHdr = malloc(sizeof(BRS_PACKET_HEADER));
                            memset(soldHdr,0, sizeof(BRS_PACKET_HEADER));
                            soldHdr->type = BRS_SOLD_PKT;
                            soldHdr->size = sizeof(BRS_NOTIFY_INFO);
                            BRS_NOTIFY_INFO* soldInfo = malloc(sizeof(BRS_NOTIFY_INFO));
                            soldInfo->buyer =htonl(bidCursor->orderid);
                            soldInfo->seller = htonl(askCursor->orderid);
                            soldInfo->quantity = htonl(askCursor->quantity);
                            soldInfo->price =htonl(askCursor->price);
                            if (trader_broadcast_packet(soldHdr, soldInfo) == -1){
                                free(soldHdr);
                                free(soldInfo);
                                break;
                            }
                            free(soldHdr);
                            free(soldInfo);
                            //sent trade pkt
                            bidCursor->quantity-=askCursor->quantity;
                            BRS_PACKET_HEADER* tradeHdr = malloc(sizeof(BRS_PACKET_HEADER));
                            memset(tradeHdr,0, sizeof(BRS_PACKET_HEADER));
                            tradeHdr->type = BRS_TRADED_PKT;
                            tradeHdr->size = sizeof(BRS_NOTIFY_INFO);
                            BRS_NOTIFY_INFO* notifyInfo = malloc(sizeof(BRS_NOTIFY_INFO));
                            notifyInfo->buyer =htonl(bidCursor->orderid);
                            notifyInfo->seller = htonl(askCursor->orderid);
                            notifyInfo->quantity = htonl(askCursor->quantity);
                            notifyInfo->price =htonl(askCursor->price);
                            if (trader_broadcast_packet(tradeHdr, notifyInfo) == -1){
                                free(tradeHdr);
                                free(notifyInfo);
                                break;
                            }
                            free(tradeHdr);
                            free(notifyInfo);
                            exchange->last = askCursor->price;
                            trader_unref(askCursor->trader,"sell order finished");
                            //free ask order
                            if (prevAsk==NULL){
                                exchange->asks = exchange->asks->nextAsk;
                                askCursor->trader=NULL;
                                askCursor->nextAsk=NULL;
                                free(askCursor);
                                askCursor=exchange->asks;
                            } else {
                                prevAsk->nextAsk = askCursor->nextAsk;
                                askCursor->trader=NULL;
                                askCursor->nextAsk=NULL;
                                free(askCursor);
                                askCursor = prevAsk->nextAsk;
                            }
                            //check buy
                            if (bidCursor->quantity==0){
                                trader_unref(bidCursor->trader,"buy order finished ");
                                //change position
                                exchange->bids = exchange->bids->nextBids;
                                bidCursor->trader=NULL;
                                bidCursor->nextBids=NULL;
                                free(bidCursor);
                                bidCursor=exchange->bids;
                            }
                        } else{
                            //sent bought pkt
                            trader_increase_inventory(bidCursor->trader,bidCursor->quantity);
                            if (bidCursor->price>askCursor->price){
                                //return excess money
                                trader_increase_balance(bidCursor->trader,
                                        (bidCursor->price-askCursor->price)*bidCursor->quantity);
                            }
                            BRS_PACKET_HEADER* boughtHdr = malloc(sizeof(BRS_PACKET_HEADER));
                            memset(boughtHdr,0, sizeof(BRS_PACKET_HEADER));
                            boughtHdr->type = BRS_BOUGHT_PKT;
                            boughtHdr->size = sizeof(BRS_NOTIFY_INFO);
                            BRS_NOTIFY_INFO* boughtInfo = malloc(sizeof(BRS_NOTIFY_INFO));
                            boughtInfo->buyer =htonl(bidCursor->orderid);
                            boughtInfo->seller = htonl(askCursor->orderid);
                            boughtInfo->quantity = htonl(bidCursor->quantity);
                            boughtInfo->price =htonl(askCursor->price);
                            if (trader_broadcast_packet(boughtHdr, boughtInfo) == -1){
                                free(boughtHdr);
                                free(boughtInfo);
                                break;
                            }
                            free(boughtHdr);
                            free(boughtInfo);
                            //sent sold pkt
                            trader_increase_balance(askCursor->trader,askCursor->price*bidCursor->quantity);
                            BRS_PACKET_HEADER* soldHdr = malloc(sizeof(BRS_PACKET_HEADER));
                            memset(soldHdr,0, sizeof(BRS_PACKET_HEADER));
                            soldHdr->type = BRS_SOLD_PKT;
                            soldHdr->size = sizeof(BRS_NOTIFY_INFO);
                            BRS_NOTIFY_INFO* soldInfo = malloc(sizeof(BRS_NOTIFY_INFO));
                            soldInfo->buyer =htonl(bidCursor->orderid);
                            soldInfo->seller = htonl(askCursor->orderid);
                            soldInfo->quantity = htonl(bidCursor->quantity);
                            soldInfo->price =htonl(askCursor->price);
                            if (trader_broadcast_packet(soldHdr, soldInfo) == -1){
                                free(soldHdr);
                                free(soldInfo);
                                break;
                            }
                            free(soldHdr);
                            free(soldInfo);
                            //send trade pkt
                            askCursor->quantity-=bidCursor->quantity;
                            BRS_PACKET_HEADER* tradeHdr = malloc(sizeof(BRS_PACKET_HEADER));
                            memset(tradeHdr,0, sizeof(BRS_PACKET_HEADER));
                            tradeHdr->type = BRS_TRADED_PKT;
                            tradeHdr->size = sizeof(BRS_NOTIFY_INFO);
                            BRS_NOTIFY_INFO* notifyInfo = malloc(sizeof(BRS_NOTIFY_INFO));
                            notifyInfo->buyer =htonl(bidCursor->orderid);
                            notifyInfo->seller = htonl(askCursor->orderid);
                            notifyInfo->quantity = htonl(bidCursor->quantity);
                            notifyInfo->price =htonl(askCursor->price);
                            if (trader_broadcast_packet(tradeHdr, notifyInfo) == -1){
                                free(tradeHdr);
                                free(notifyInfo);
                                break;
                            }
                            free(tradeHdr);
                            free(notifyInfo);
                            exchange->last = askCursor->price;
                            trader_unref(bidCursor->trader,"buy order finished");
                            //free buy
                            exchange->bids = exchange->bids->nextBids;
                            bidCursor->trader=NULL;
                            bidCursor->nextBids=NULL;
                            free(bidCursor);
                            bidCursor=exchange->bids;
                        }
                    }else{
                        prevAsk = askCursor;
                        askCursor= askCursor->nextAsk;
                    }
                }
                break;
            }
        }
    }
    info("%s%p%s","Matchmaking Thread for exchange: ",exchange," stopping");
    V(&exchangeSem);
    return NULL;
}