#pragma once

#include <string>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <json/json.h>

/*

TODO: Add more events and handle them in handleEvents function
TODO: Add more intents to INTENTS variable
TODO: Move requests into websocket and completely remove requests.h

*/

const std::string GATEWAY_URL = "wss://gateway.discord.gg/?v=9&encoding=json";
const int INTENTS = (1 << 0) | (1 << 9) | (1 << 1) | (1 << 12);

typedef struct {
    std::string type;
    std::string data;
} WebSocketMessage;


void connectToGateway();
void handleEvents(const Json::Value& event);
void sendHeartBeat(ix::WebSocket& websocket, int interval);