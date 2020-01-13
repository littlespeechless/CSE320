//
// Created by student on 12/6/19.
//

#ifndef HW5_TRADER_STRUCT_H
#define HW5_TRADER_STRUCT_H

#include "protocol.h"
#include "trader.h"

struct trader{
    int traderFd;
    char* traderName;
    funds_t balance;               // Trader's account balance
    quantity_t inventory;          // Trader's inventory
    int tradeReferenceCount;
};
#endif //HW5_TRADER_STRUCT_H
