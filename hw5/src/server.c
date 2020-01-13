//
// Created by student on 12/3/19.
//
#include "server.h"
#include <stdlib.h>
#include "csapp.h"
#include "protocol.h"
#include "debug.h"
#include "trader_struct.h"
sem_t mutex_server;
void *brs_client_service(void *arg){
    int connfd = *((int *)arg);
    free(arg);
    Pthread_detach(pthread_self());
    info("client service started");
    sem_init(&mutex_server,0,1);
    //register this client
    P(&mutex_server);
    creg_register(client_registry,connfd);
    V(&mutex_server);
    int loginStatus=0;
    TRADER* currentTrader=NULL;
    BRS_PACKET_HEADER* hdr = malloc(sizeof(BRS_PACKET_HEADER));
    void* payload = NULL;
    while (proto_recv_packet(connfd,hdr,&payload)==0){
        if (loginStatus==0){
            if (hdr->type==BRS_LOGIN_PKT){
                if (payload !=NULL){
                    //null pointer end
                    //*((char*)payload+hdr->size)='\0';
                    char * loginName = malloc(hdr->size+1);
                    memcpy(loginName,payload,hdr->size);
                    free(payload);
                    payload = loginName;
                    *(loginName+hdr->size) ='\0';
                    currentTrader = trader_login(connfd,payload);
                    if (currentTrader!=NULL){
                        info("%s%s","user logged in ",(char *)payload);
                        loginStatus=1;
                        trader_send_ack(currentTrader,NULL);
                        free(payload);
                    }else{
                        BRS_PACKET_HEADER* nack = malloc(sizeof(BRS_PACKET_HEADER));
                        nack->type=BRS_NACK_PKT;
                        nack->size=0;
                        proto_send_packet(connfd,nack,NULL);
                        free(nack);
                        currentTrader =NULL;
                        free(payload);
                    }
                }
            } else {
                info("Login Required.");
                //follow the document
//                BRS_PACKET_HEADER* nack = malloc(sizeof(BRS_PACKET_HEADER));
//                nack->type=BRS_NACK_PKT;
//                nack->size=0;
//                proto_send_packet(connfd,nack,NULL);
//                free(nack);
            }
        } else {
            switch (hdr->type){
                BRS_FUNDS_INFO *fundsInfo;
                BRS_STATUS_INFO* statusInfo;
                BRS_ESCROW_INFO * escrowInfo;
                BRS_ORDER_INFO * orderInfo;
                BRS_CANCEL_INFO * cancelInfo;
                case BRS_STATUS_PKT:
                    info("Status Packet Received ");
                    statusInfo= malloc(sizeof(BRS_STATUS_INFO));
                    statusInfo->inventory = htonl(currentTrader->inventory);
                    statusInfo->quantity = htonl(0);
                    statusInfo->balance = htonl(currentTrader->balance);
                    statusInfo->orderid = htonl(0);
                    exchange_get_status(exchange,statusInfo);
                    trader_send_ack(currentTrader,statusInfo);
                    free(statusInfo);
                    break;
                case BRS_DEPOSIT_PKT:
                    info("Deposit Packet Received ");
                    fundsInfo = (BRS_FUNDS_INFO *) payload;
                    funds_t depositAmount = ntohl(fundsInfo->amount);
                    free(fundsInfo);
                    trader_increase_balance(currentTrader,depositAmount);
                    statusInfo = malloc(sizeof(BRS_STATUS_INFO));
                    statusInfo->inventory = htonl(currentTrader->inventory);
                    statusInfo->quantity = htonl(0);
                    statusInfo->balance = htonl(currentTrader->balance);
                    statusInfo->orderid = htonl(0);
                    exchange_get_status(exchange,statusInfo);
                    trader_send_ack(currentTrader,statusInfo);
                    free(statusInfo);
                    break;
                case BRS_WITHDRAW_PKT:
                    info("Withdraw Packet Received ");
                    fundsInfo = (BRS_FUNDS_INFO *) payload;
                    funds_t withdrawAmount = ntohl(fundsInfo->amount);
                    free(fundsInfo);
                    int withdrawStatus = trader_decrease_balance(currentTrader,withdrawAmount);
                    if (withdrawStatus==-1){
                        trader_send_nack(currentTrader);
                        break;
                    }
                    statusInfo = malloc(sizeof(BRS_STATUS_INFO));
                    statusInfo->inventory = htonl(currentTrader->inventory);
                    statusInfo->quantity = htonl(0);
                    statusInfo->balance = htonl(currentTrader->balance);
                    statusInfo->orderid = htonl(0);
                    exchange_get_status(exchange,statusInfo);
                    trader_send_ack(currentTrader,statusInfo);
                    free(statusInfo);
                    break;
                case BRS_ESCROW_PKT:
                    info("Escrow Packet Received ");
                    escrowInfo = (BRS_ESCROW_INFO*) payload;
                    quantity_t escrowAmt =  ntohl(escrowInfo->quantity);
                    free(escrowInfo);
                    trader_increase_inventory(currentTrader,escrowAmt);
                    statusInfo = malloc(sizeof(BRS_STATUS_INFO));
                    statusInfo->inventory = htonl(currentTrader->inventory);
                    statusInfo->quantity = htonl(0);
                    statusInfo->balance = htonl(currentTrader->balance);
                    statusInfo->orderid = htonl(0);
                    exchange_get_status(exchange,statusInfo);
                    trader_send_ack(currentTrader,statusInfo);
                    free(statusInfo);
                    break;
                case BRS_RELEASE_PKT:
                    info("Release Packet Received ");
                    escrowInfo = (BRS_ESCROW_INFO*) payload;
                    quantity_t releaseAmt =  ntohl(escrowInfo->quantity);
                    free(escrowInfo);
                    int releaseStatus = trader_decrease_inventory(currentTrader,releaseAmt);
                    if (releaseStatus==-1){
                        trader_send_nack(currentTrader);
                        break;
                    }
                    statusInfo = malloc(sizeof(BRS_STATUS_INFO));
                    statusInfo->inventory = htonl(currentTrader->inventory);
                    statusInfo->quantity = htonl(0);
                    statusInfo->balance = htonl(currentTrader->balance);
                    statusInfo->orderid = htonl(0);
                    exchange_get_status(exchange,statusInfo);
                    trader_send_ack(currentTrader,statusInfo);
                    free(statusInfo);
                    break;
                case BRS_BUY_PKT:
                    info("Buy Packet Received ");
                    orderInfo = (BRS_ORDER_INFO*) payload;
                    quantity_t buyQuantity = ntohl(orderInfo->quantity);
                    funds_t buyPrice = ntohl(orderInfo->price);
                    free(orderInfo);
                    orderid_t buyOrderID = exchange_post_buy(exchange,currentTrader,buyQuantity,buyPrice);
                    if (buyOrderID==0){
                        trader_send_nack(currentTrader);
                        break;
                    }
                    statusInfo = malloc(sizeof(BRS_STATUS_INFO));
                    statusInfo->inventory = htonl(currentTrader->inventory);
                    statusInfo->quantity = htonl(0);
                    statusInfo->balance = htonl(currentTrader->balance);
                    statusInfo->orderid = htonl(buyOrderID);
                    exchange_get_status(exchange,statusInfo);
                    trader_send_ack(currentTrader,statusInfo);
                    free(statusInfo);
                    break;
                case BRS_SELL_PKT:
                    info("Sell Packet Received ");
                    orderInfo = (BRS_ORDER_INFO*) payload;
                    quantity_t sellQuantity = ntohl(orderInfo->quantity);
                    funds_t sellPrice = ntohl(orderInfo->price);
                    free(orderInfo);
                    orderid_t sellOrderID = exchange_post_sell(exchange,currentTrader,sellQuantity,sellPrice);
                    if (sellOrderID==0){
                        trader_send_nack(currentTrader);
                        break;
                    }
                    statusInfo = malloc(sizeof(BRS_STATUS_INFO));
                    statusInfo->inventory = htonl(currentTrader->inventory);
                    statusInfo->quantity = htonl(0);
                    statusInfo->balance = htonl(currentTrader->balance);
                    statusInfo->orderid = htonl(sellOrderID);
                    exchange_get_status(exchange,statusInfo);
                    trader_send_ack(currentTrader,statusInfo);
                    free(statusInfo);
                    break;
                case BRS_CANCEL_PKT:
                    info("Cancel Packet Received ");
                    cancelInfo = (BRS_CANCEL_INFO*) payload;
                    orderid_t cancelID = ntohl(cancelInfo->order);
                    free(cancelInfo);
                    quantity_t cancelQuantity;
                    int cancelStatus = exchange_cancel(exchange,currentTrader,cancelID,&cancelQuantity);
                    if (cancelStatus==-1){
                        trader_send_nack(currentTrader);
                        break;
                    }
                    statusInfo = malloc(sizeof(BRS_STATUS_INFO));
                    statusInfo->inventory = htonl(currentTrader->inventory);
                    statusInfo->quantity = htonl(cancelQuantity);
                    statusInfo->balance = htonl(currentTrader->balance);
                    statusInfo->orderid = htonl(cancelID);
                    exchange_get_status(exchange,statusInfo);
                    trader_send_ack(currentTrader,statusInfo);
                    free(statusInfo);
                    break;
                default: trader_send_nack(currentTrader);
            }

        }
    }
    if(loginStatus>0){
        trader_logout(currentTrader);
        creg_unregister(client_registry,connfd);
        free(hdr);
        info("%s,%d","end client connection",connfd);
    } else {
        creg_unregister(client_registry,connfd);
        free(hdr);
    }
    return NULL;
}