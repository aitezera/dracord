#pragma once

#include <string>
#include <filesystem>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXUserAgent.h>
#include <json/json.h>

#include "requests.h"

using std::string;
namespace fs = std::filesystem;

/*

TODO: Add more events and handle them in handleEvents function
TODO: Add more intents to INTENTS variable
TODO: Move requests into websocket and completely remove requests.h

*/

#define DISCORD_GUILD_INTENTS (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4) | (1 << 5) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 8) | (1 << 9) | (1 << 10) | (1 << 11) | (1 << 15) | (1 << 16) | (1 << 20) | (1 << 21) | (1 << 24) 
#define DISCORD_DM_INTENTS (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15) | (1 << 25)
#define DISCORD_ALL_INTENTS DISCORD_GUILD_INTENTS | DISCORD_DM_INTENTS


class WebSocket {
    public:
        WebSocket() {
            w_websocket.setUrl(r_gateway);
        };

        ~WebSocket() {
            w_websocket.stop();
        };

        const string r_gateway = "wss://gateway.discord.gg/?v=9&encoding=json";
        const string r_cdn = "https://cdn.discordapp.com/";
        const string r_invite = "https://discord.gg/";

        void connectToGateway(const Requests& req);
    
    private:
        void handleEvents(const Json::Value& event);
        void sendHeartBeat(ix::WebSocket& websocket, int interval);

        ix::WebSocket w_websocket;

        // Mutex for the websocket
        std::mutex m_mutex; // TODO: Implement this
};

// TODO: Either implement or remove this
typedef struct {
    int id;
    int channelId;
    int userId;
    string type;
    string data;
    string content;
} WebSocketMessage;