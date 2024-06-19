#pragma once

#include <cpr/cpr.h>
#include <iostream>
#include <json/json.h>
#include <string>
#include <vector>
#include <map>

#include "friend.h"
#include "guild.h"
#include "channel.h"
#include "messages.h"

using std::string;
using std::vector;
using std::map;

class Requests {
    public:
        string r_base_api = "https://discord.com/api/v9/";

        
        vector<Friend> r_friends;
        map<string, Guild> r_guilds;
        map<string, Channel> r_channels;
        map<string, vector<Messages>> r_messages;


        Requests(string uToken) {
            r_token = uToken;  // Set token
        }

        int login_user();

    private:
        string r_token;
        string r_filename = "token.bin";

        cpr::Header r_headers = {
            {"Content-Type", "application/json"},
            {"Authorization", r_token}
        };

        void updateHeaders() {
            r_headers["Authorization"] = r_token; // Set Headers upon receiving token. Otherwise this will be empty and fail
        }

        // Read and write token to file
        void save_token();
        void load_token();
    

        // API Requests
        void load_friends();
        void load_guilds();
        void load_channels();
        void load_messages();
        
        void send_server_message(long r_channel_id, string r_message);
        void send_friend_message(long r_channel_id, string r_message);

        void handle_status_code(int r_status_code);
};