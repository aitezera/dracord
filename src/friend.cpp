#include "friend.h"
#include "logging.h"
#include "requests.h"

void Friend::edit_friend() {};

void Friend::delete_friend() {};

void Friend::add_friend() {};

void Friend::block_friend() {};

void Friend::unblock_friend() {};

void Friend::view_friend() {};

void Friend::send_message(long channel_id, std::string message) {
    Requests req;
    cpr::Header headers = req.getHeaders();
    logger->Info("[!] Sending Friend Message");

    Json::Value json;
    json["content"] = message;
    json["tts"] = false;

    Json::StreamWriterBuilder builder;
    std::string json_string = Json::writeString(builder, json);

    cpr::Response response = cpr::Post(cpr::Url{req.r_base_api + "channels/" + std::to_string(channel_id) + "/messages"}, cpr::Body{json_string}, headers=headers);
    logger->Info(("Using POST response with URL: " + req.r_base_api + "channels/" + std::to_string(channel_id) + "/messages").c_str());

    if (response.status_code != 200) {
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str());
};