#include "channel.h"
#include "logging.h"
#include "requests.h"

void Channel::edit_channel() {};

void Channel::delete_channel() {};

void Channel::view_channel() {};

void Channel::send_message(long channel_id, std::string message) {
    Requests req;
    logger->Info("[!] Sending Server Message");

    Json::Value json;
    json["content"] = message;
    json["tts"] = false;

    Json::StreamWriterBuilder builder;
    std::string json_string = Json::writeString(builder, json);

    cpr::Response response = cpr::Post(cpr::Url{req.r_base_api + "channels/" + std::to_string(channel_id) + "/messages"}, cpr::Body{json_string}, req.r_headers=req.r_headers);
    logger->Info(("Using POST response with URL: " + req.r_base_api + "channels/" + std::to_string(channel_id) + "/messages").c_str());

    if (response.status_code != 200) {
        req.handle_status_code(response.status_code);
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str());
};