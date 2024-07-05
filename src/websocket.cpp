#include "websocket.h"
#include "logging.h"
#include "requests.h"

//
//_____________________________________________________________________________________________________________________________

void WebSocket::handleEvents(const Json::Value& event) {
    
    // Check if event is an object and not null
    if (!event.isObject()) {
        logger->Error("Event is not a JSON object");
        return;
    }

    // Check if "t" property exists and is a string
    if (!event.isMember("t") || !event["t"].isString()) {
        logger->Error("Event type (t) is missing or not a string");
        return;
    }

    std::string type = event["t"].asString();

    if (type == "MESSAGE_CREATE") {
        logger->Info(("User: " + event["d"]["author"]["username"].asString() + " created message: " + event["d"]["content"].asString()).c_str());

    } else if (type == "TYPING_START") {
        logger->Info(("User is typing: " + event["d"]["user_id"].asString() + " in channel: " + event["d"]["channel_id"].asString()).c_str());
        
    } else if (type == "MESSAGE_DELETE") {
        logger->Info(("Message deleted: " + event["d"]["id"].asString() + " in channel: " + event["d"]["name"].asString()).c_str());

    } else if (type == "PRESENCE_UPDATE") {
        logger->Info(("Presence updated: " + event["d"]["user"]["id"].asString() + " to: " + event["d"]["status"].asString() ).c_str());

    } else if (type == "USER_SETTINGS_UPDATE") {
        logger->Info(("User:" + event["d"]["display_name"].asString() + " changed: " + event["d"]["key"].asString() + "to: " + event["d"]["value"].asString()).c_str());

    } else if (type == "MESSAGE_UPDATE") {
        logger->Info(("Message updated: " + event["d"]["content"].asString() + " in channel: " + event["d"]["channel_id"].asString()).c_str());
    }
    
    // Add more events here
}

//
//_____________________________________________________________________________________________________________________________

void WebSocket::sendHeartBeat(ix::WebSocket& w_websocket, int interval) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        Json::Value heartbeat;

        heartbeat["op"] = 1;
        heartbeat["d"] = 251;

        w_websocket.send(heartbeat.toStyledString());
        logger->Info("Heartbeat sent to keep connection alive");
    }
}

//
//_____________________________________________________________________________________________________________________________

void WebSocket::connectToGateway() {
    ix::initNetSystem();

    logger->Info("Connecting to Discord gateway");

    w_websocket.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            logger->Info("Connected to Discord gateway");

            Json::Value identify;
            Requests req;
            identify["op"] = 2;
            identify["d"]["token"] = req.r_token;
            identify["d"]["intents"] = DISCORD_ALL_INTENTS;
            identify["d"]["properties"]["$os"] = "linux";
            identify["d"]["properties"]["$browser"] = "my_library";
            identify["d"]["properties"]["$device"] = "my_library";

            w_websocket.send(identify.toStyledString());
        }

        if (msg->type == ix::WebSocketMessageType::Message) {
            Json::Value json;
            Json::Reader reader;
            auto event = reader.parse(msg->str, json);

            if (json["op"].asInt() == 0) {
                handleEvents(json);
            }

            static bool initialized = false;
            if (!initialized && event["t"] == false && event["s"] == false) {
                int heartbeatInterval = event["d"]["heartbeat_interval"];
                std::thread heartbeatThread([this, heartbeatInterval]() {
                    this->sendHeartBeat(w_websocket, heartbeatInterval);
                });

                heartbeatThread.detach();

                initialized = true;
            }
        }
    });

    logger->Info("Starting Websocket");
    w_websocket.start();
    logger->Info("Running Websocket");

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    ix::uninitNetSystem();
}

//
//_____________________________________________________________________________________________________________________________