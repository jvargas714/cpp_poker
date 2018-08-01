//
// Created by jay on 7/31/18.
//

#ifndef POKER_GAMESERVER_H
#define POKER_GAMESERVER_H
#include <memory>
#include <map>
#include <thread>
#include <queue>
#include <sys/socket.h>

#include "json-forwards.h"
#include "json.h"
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
    GameServer() : serverType(SERVER_TYPE::TCP_TYPE) {};
    virtual ~GameServer()= default;
    inline bool setProtocol(SERVER_TYPE servType=TCP_TYPE) { serverType = servType; }
    virtual int run()=0;
protected:
    virtual bool EventLoopThread() {};
    virtual bool processEvent(const EventType&)=0;
private:
    SERVER_TYPE serverType;
    std::queue<EventType> eventQueue;
    std::map<uint64_t, std::unique_ptr<GameType>> m_gameMap;
};


template <typename PokerEventType, typename PokerGameType>
class PokerServer : public GameServer<PokerEventType, PokerGameType> {
public:
    explicit PokerServer(const std::string& cfgPath);
    ~PokerServer();
    bool processEvent(const PokerEventType& evt) override;
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
template<typename PokerEventTypem, typename PokerGameType>
PokerServer::PokerServer(const std::string &cfgPath) : GameServer() {
    if (!initViaCfgFile(cfgPath)) {
        LOG_ERROR << "there was an error initializing from config file " << cfgPath << END;
        sendErrorEvent(POKER_ERROR::CFG_LOAD_ERROR);
    }
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::processEvent(const PokerEventType &evt){
    return false;
}

template<typename PokerEventType, typename PokerGameType>
PokerServer<PokerEventType, PokerGameType>::~PokerServer() {

}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::sendErrorEvent(POKER_ERROR err) const {
    return false;
}

template<typename PokerEventType, typename PokerGameType>
bool PokerServer<PokerEventType, PokerGameType>::initViaCfgFile(const std::string &cfgPath) {
    return false;
}

#endif //POKER_GAMESERVER_H
