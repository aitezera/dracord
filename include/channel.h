#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>

#include "guild.h"

class Channel : Guild {
    public:
        Channel() {}
        ~Channel() {}

        Channel(long id, std::string name, std::string topic, bool nsfw) {
            this->c_id = id;
            this->c_name = name;
            this->c_topic = topic;
            this->c_nsfw = nsfw;
        }

        long c_id;
        std::string c_name;
        std::string c_topic;
        bool c_nsfw;

        // TODO: Add permissions later on
        
        void view_channel();
        void edit_channel();
        void delete_channel();
        void send_message(long channel_id, std::string message);
};