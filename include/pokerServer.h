//
// Created by jay on 7/31/18.
//

#ifndef POKER_GAMESERVER_H
#define POKER_GAMESERVER_H
#include <chrono>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <queue>
#include <sys/socket.h>

#include "json-forwards.h"
#include "json.h"
#include "net_types.h"
#include "poker.h"
#include "poker_types.h"

enum SERVER_TYPE {
    TCP_TYPE,
    UDP_TYPE,
    SCTP_TYPE,
    HTTP_TYPE
};

// todo :: poker classes need a connection manager of sorts to keep track of port connected etc etc



template<typename EventType, typename GameType>
class GameServer {
public:
    GameServer() : eventProcIsAlive(true), recvIsAlive(true), mtx(), serverType(SERVER_TYPE::TCP_TYPE) {};
    virtual ~GameServer() = default;
    inline bool setProtocol(SERVER_TYPE servType=TCP_TYPE) { serverType = servType; }
    virtual int run();
    void stopEventRecievingThread() { recvIsAlive = false; };
    void stopEventProcessing() { eventProcIsAlive = false; };
    virtual bool EventRecieving()=0;
    virtual bool EventProcessing()=0;
    static const uint32_t RECV_THREAD;
    static const uint32_t Q_PROC_THREAD;
    bool isAlive(uint32_t threadType) const;
protected:
    std::queue<EventType> eventQueue;
    bool eventProcIsAlive;
    bool recvIsAlive;
    std::mutex mtx;
    SERVER_TYPE serverType;
    std::map<uint64_t, NetConnection<GameType>> gameConnectionMap;
    virtual bool processEvent(const EventType&)=0;
};





template <typename PokerEventType, typename PokerGameType>
class PokerServer : public GameServer<PokerEventType, PokerGameType> {
public:
    using GameServer<PokerEventType, PokerGameType>::Q_PROC_THREAD;
    using GameServer<PokerEventType, PokerGameType>::RECV_THREAD;
    using GameServer<PokerEventType, PokerGameType>::isAlive;
    using GameServer<PokerEventType, PokerGameType>::eventQueue;
    using GameServer<PokerEventType, PokerGameType>::mtx;

    PokerServer()=default;
    explicit PokerServer(const std::string& cfgPath);
    ~PokerServer();
    bool processEvent(const PokerEventType& evt) override;
    bool EventRecieving() override;
    bool EventProcessing() override;
private:
    // send functions
    bool sendErrorEvent(POKER_ERROR err) const;

    // process functions
    bool processBet(const PokerEventType& evt);
    bool processRaise(const PokerEventType& evt);
    bool processFold(const PokerEventType& evt);
    bool processAllIn(const PokerEventType& evt);
    bool processCheck(const PokerEventType& evt);
    bool processAddGame(const PokerEventType& evt);
    bool processFindWinner(const PokerEventType& evt);
    bool processDealHoleCards(const PokerEventType& evt);
    bool processDealFlop(const PokerEventType& evt);
    bool processDealTurn(const PokerEventType& evt);
    bool processDealRiver(const PokerEventType& evt);
    bool processLeaveGame(const PokerEventType& evt);
    bool processEnterGame(const PokerEventType& evt);
    bool processSaveGame(const PokerEventType& evt);
    bool processLoadGame(const PokerEventType& evt);

    // notifier for front end
    bool notifyFrontEnd(PokerEventType& evt);

    // Misc
    bool initViaCfgFile(const std::string& cfgPath);
    std::unique_ptr<PokerGameType> lookupGame(uint64_t gameId);
};

// -------------------------------------------implementation---------------------------------------------
template<typename EventType, typename GameType>
const uint32_t GameServer<EventType, GameType>::Q_PROC_THREAD = 1;

template<typename EventType, typename GameType>
const uint32_t GameServer<EventType, GameType>::RECV_THREAD = 2;


template<typename PokerEventType, typename PokerGameType>
PokerServer<PokerEventType, PokerGameType>::PokerServer(const std::string &cfgPath) :
        GameServer<PokerEventType, PokerGameType>() {
    if (!initViaCfgFile(cfgPath)) {
        LOG_ERROR << "there was an error initializing from config file " << cfgPath << END;
        sendErrorEvent(POKER_ERROR::CFG_LOAD_ERROR);
    }
}

//template<typename PokerEventType, typename PokerGameType>
//PokerServer<PokerEventType, PokerGameType>::PokerServer(const std::string &cfgPath) :
//GameServer<PokerEventType, PokerGameType>() {
//    if (!initViaCfgFile(cfgPath)) {
//        LOG_ERROR << "there was an error initializing from config file " << cfgPath << END;
//        sendErrorEvent(POKER_ERROR::CFG_LOAD_ERROR);
//    }
//}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processEvent(const PokerEventType &evt){
    return false;
}

template<typename PokerEventType, typename PokerGameType>
PokerServer<PokerEventType, PokerGameType>::~PokerServer() = default;

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::sendErrorEvent(POKER_ERROR err) const {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::initViaCfgFile(const std::string &cfgPath) {
    return false;
}

template<typename EventType, typename GameType>
int GameServer<EventType, GameType>::run() {
    LOG_TRACE << "starting event processing and recv threads";
    eventProcIsAlive = true;
    recvIsAlive = true;
    return 0;
}

template<typename EventType, typename GameType>
bool GameServer<EventType, GameType>::isAlive(uint32_t threadType) const {
    switch(threadType) {
        case RECV_THREAD:
            return recvIsAlive;
        case Q_PROC_THREAD:
            return eventProcIsAlive;
        default:
            return false;
    }
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processBet(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::EventRecieving() {
    while (isAlive(RECV_THREAD)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        LOG_TRACE << "event recv thread is alive!!" << END;
    }
    LOG_TRACE << "exiting event recv thread!!" << END;
    return true;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::EventProcessing() {
    while (isAlive(Q_PROC_THREAD)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!eventQueue.empty()) {
            mtx.lock();
            PokerEventType pkrEvent = eventQueue.front();
            eventQueue.pop();
            mtx.unlock();
            processEvent(pkrEvent);
        }
        LOG_TRACE << "event queue is empty :( " << END;
    }
    return true;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processRaise(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processFold(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processAllIn(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processCheck(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processAddGame(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processFindWinner(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processDealHoleCards(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processDealFlop(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processDealTurn(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processDealRiver(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processLeaveGame(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processEnterGame(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processSaveGame(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processLoadGame(const PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::notifyFrontEnd(PokerEventType &evt) {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
std::unique_ptr<PokerGameType> PokerServer<PokerEventType, PokerGameType>::lookupGame(uint64_t gameId) {
    return std::unique_ptr<PokerGameType>();
}

#endif //POKER_GAMESERVER_H
