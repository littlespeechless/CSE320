//
// Created by student on 12/7/19.
//

#ifndef HW5_EXCHANGE_STRUCT_H
#define HW5_EXCHANGE_STRUCT_H
#include "exchange.h"
typedef struct Bids{
    quantity_t quantity;           // Quantity to buy/sell
    funds_t price;                 // Price
    TRADER* trader;
    orderid_t orderid;
    struct Bids* nextBids;
}TOTAL_BIDS;
typedef struct Asks{
    quantity_t quantity;           // Quantity to buy/sell
    funds_t price;                 // Price
    TRADER * trader;
    orderid_t orderid;
    struct Asks* nextAsk; // if no next then NULL
}TOTAL_ASKS;
struct exchange{
    TOTAL_BIDS* bids;
    funds_t last;                  // Last trade
    TOTAL_ASKS* asks;
    orderid_t orderid;
};
#endif //HW5_EXCHANGE_STRUCT_H
