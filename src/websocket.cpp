#include "websocket.h"
#include "logging.h"
#include "requests.h"

void handleEvents(const Json::Value& event) {

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
    logger->Info(("Event type: " + type).c_str());

    if (type == "MESSAGE_CREATE") {
        logger->Info(("Message created: " + event["d"]["content"].asString()).c_str());
    } else if (type == "TYPING_START") {
        logger->Info(("User is typing: " + event["d"]["user_id"].asString() + "in channel: " + event["d"]["channel_id"].asString()).c_str());
    } else if (type == "MESSAGE_DELETE") {
        logger->Info(("Message deleted: " + event["d"]["id"].asString() + "in channel: " + event["d"]["channel_id"].asString()).c_str());
    }
    
    // Add more events here
}

void sendHeartBeat(ix::WebSocket& websocket, int interval) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        Json::Value heartbeat;

        heartbeat["op"] = 1;
        heartbeat["d"] = 251;

        websocket.send(heartbeat.toStyledString());
    }
}

void connectToGateway() {
    //ix::initNetSystem();

    ix::WebSocket webSocket;
    webSocket.setUrl(GATEWAY_URL);

    logger->Info("Connecting to Discord gateway");

    webSocket.setOnMessageCallback([&webSocket](const ix::WebSocketMessagePtr& msg) {
        if (msg->type == ix::WebSocketMessageType::Open) {
            logger->Info("Connected to Discord gateway");

            Json::Value identify;
            identify["op"] = 2;
            identify["d"]["token"] = Requests::getToken();
            identify["d"]["intents"] = INTENTS;
            identify["d"]["properties"]["$os"] = "linux";
            identify["d"]["properties"]["$browser"] = "google-chrome";
            identify["d"]["properties"]["$device"] = "chrome";

            webSocket.send(identify.toStyledString());
            logger->Info("Sent identify message");
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
                std::thread heartbeatThread(sendHeartBeat, std::ref(webSocket), heartbeatInterval);
                heartbeatThread.detach();

                initialized = true;
            }
        }
    });

    logger->Info("Starting websocket");
    webSocket.start();
    logger->Info("Running websocket");

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }

    //ix::uninitNetSystem();
}