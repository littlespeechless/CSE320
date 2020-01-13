//
// Created by student on 11/29/19.
//
#include "protocol.h"
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include "csapp.h"

int proto_send_packet(int fd, BRS_PACKET_HEADER *hdr, void *payload) {
    // check fd
    if (fd < 0) {
        return -1;
    }
    //check header
    if (hdr == NULL) {
        return -1;
    }
    //check payload
    if ((hdr->size != 0 && payload == NULL) || (hdr->size == 0 && payload != NULL)) {
        return -1;
    }
    int hasPayload = 0;
    if (hdr->size > 0) {
        hasPayload = hdr->size;
    }
    //get time
    struct timespec time;
    int getTime = clock_gettime(CLOCK_REALTIME, &time);
    if (getTime == -1) {
        return -1;
    }
    //convert byte to network byte
    hdr->size=htons(hdr->size);
    hdr->timestamp_nsec=htonl(time.tv_nsec);
    hdr->timestamp_sec=htonl(time.tv_sec);
    //write all
    int writeHeader = rio_writen(fd,(void*)hdr, sizeof(BRS_PACKET_HEADER));
    if(writeHeader==-1){
        return -1;
    }
//    int writeHeader = write(fd,(void*)hdr, sizeof(BRS_PACKET_HEADER));
//    if (writeHeader!= sizeof(BRS_PACKET_HEADER)){
//        return -1;
//    }
    if (hasPayload){
        int writePayload = rio_writen(fd,payload,hasPayload);
        if (writePayload==-1){
            return -1;
        }
    }
    return 0;
}

int proto_recv_packet(int fd, BRS_PACKET_HEADER *hdr, void **payloadp) {
    //read header
    int readHeader = Rio_readn(fd,hdr, sizeof(BRS_PACKET_HEADER));
    if (readHeader!= sizeof(BRS_PACKET_HEADER)){
        return -1;
    }
    hdr->size = ntohs(hdr->size);
    hdr->timestamp_sec = ntohl(hdr->timestamp_sec);
    hdr->timestamp_nsec = ntohl(hdr->timestamp_nsec);
    if(hdr->size>0){
        size_t size = hdr->size;
        void *payload = malloc(size);
        memset(payload,'\0',hdr->size);
        int readPayload = Rio_readn(fd,payload,hdr->size);
//        while(readPayload!=hdr->size){
//            readPayload+=read(fd,payload,hdr->size-readPayload);
//        }
        if (readPayload!=hdr->size){
            return -1;
        }
        *payloadp = payload;
    }
    return 0;
}