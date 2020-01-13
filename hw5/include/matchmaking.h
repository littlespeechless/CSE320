//
// Created by student on 12/6/19.
//

#ifndef HW5_MATCHMAKING_H
#define HW5_MATCHMAKING_H

#include "exchange.h"
#include "semaphore.h"
#include "stdbool.h"
void* match_making(void* ptr);
sem_t matchMutex;
int matchStatus;
sem_t exchangeSem;
#endif //HW5_MATCHMAKING_H
