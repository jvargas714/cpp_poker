#include "pokerServer.h"
#include "TexasHoldem.h"

int main(int argv, char** args) {
    PokerServer<HoldemEvent, NetConnection<TexasHoldem>> server;
    std::thread eventRecvThread(&GameServer<HoldemEvent, NetConnection<TexasHoldem>>::EventRecieving,
            &server);

    std::thread eventProcThread(&GameServer<HoldemEvent, NetConnection<TexasHoldem>>::EventProcessing,
            &server);
    eventRecvThread.join();
    eventProcThread.join();
    return 0;
}
