#include "pokerServer.h"
#include "TexasHoldem.h"

int main(int argv, char** args) {
    PokerServer<HoldemEvent, NetConnection<TexasHoldem>>  server;
    return 0;
}
