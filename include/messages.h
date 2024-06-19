#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <json/json.h>

#include "channel.h"

class Messages : Channel {
    public:
        Messages() {}
        ~Messages() {}
        Json::Value json;
        Json::StreamWriterBuilder builder;

        long m_id;
        long m_channel_id;

        std::string m_content;
        std::string m_timestamp;

        std::vector<std::string> m_attachments;
        std::vector<std::string> m_mentions;
        
        bool m_tts;
        bool m_edited;

        //void updateJson(std::string json);

        // Overloaded send_message functions
        void send_message(std::string message);
        void send_message(std::string message, std::string attachment);
        void send_message(std::string message, std::vector<std::string> attachments);
        void send_message(std::string message, std::vector<std::string> attachments, std::vector<std::string> mentions);
        void send_message(std::string message, std::vector<std::string> attachments, std::vector<std::string> mentions, std::string tts);

        void reply_message();
        void edit_message();
        void delete_message();
        void pin_message();
        void unpin_message();
        void add_reaction();
        void remove_reaction();
};