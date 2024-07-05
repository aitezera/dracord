#pragma once

#include <string>
#include "ixwebsocket/IXWebSocket.h"

typedef struct {
    std::string type;
    std::string data;
} WebSocketMessage;


void connectToGateway();
void handleEvents(std::string event);
void sendHeartBeat(std::string websocket);