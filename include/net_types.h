//
// Created by JAY VARGAS on 8/1/18.
//

#ifndef POKER_NET_TYPES_H
#define POKER_NET_TYPES_H
#include <cstdint>
#include <memory>
#include <string>
#include <sys/socket.h>

//
//    struct sockaddr {
//        __uint8_t	sa_len;		/* total length */
//    sa_family_t	sa_family;	/* [XSI] address family */
//    char		sa_data[14];	/* [XSI] addr value (actually larger) */
//    };

#define DEFAULT_GAME_SERVER_PORT 5025

template<typename Data>
struct NetConnection {
    NetConnection(): port(0), ipAddr(), sa(){}
    uint32_t port;
    std::string ipAddr;
    sockaddr sa;
    std::unique_ptr<Data> data;
};

#endif //POKER_NET_TYPES_H
