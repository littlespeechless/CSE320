//
// Created by student on 12/3/19.
//

#ifndef HW5_CLIENT_H
#define HW5_CLIENT_H
#include <sys/types.h>
struct client_registry{
    int clientFd[FD_SETSIZE];
    int currentClients;
};
#endif //HW5_CLIENT_H
