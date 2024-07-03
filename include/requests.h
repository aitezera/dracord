#pragma once

#include <cpr/cpr.h>
#include <iostream>
#include <json/json.h>
#include <string>
#include <map>
#include <vector>
#include <filesystem>

#include "friend.h"
#include "guild.h"
#include "channel.h"

using std::string;
using std::vector;
using std::map;

class Requests {
    public:
        Requests() {}
        ~Requests() {}
        
        string r_base_api = "https://discord.com/api/v9/";
        string r_base_gateway = "wss://gateway.discord.gg/?v=9&encoding=json";
        string r_base_cdn = "https://cdn.discordapp.com/";
        string r_base_invite = "https://discord.gg/";

        map<string, Friend> r_friends;
        map<string, Guild> r_guilds;
        map<string, Channel> r_channels;
        //map<string, vector<Messages>> r_messages;

        Requests(string uToken) {
            r_token = uToken;  // Set token
        }

        cpr::Header getHeaders();
        int login_user();

    private:
        string r_token;
        string r_filename = "token.bin";

        cpr::Header r_headers = {
            {"Content-Type", "application/json"},
            {"Authorization", r_token}
        };

        void updateHeaders() {
            r_headers["Authorization"] = r_token; // Set Headers upon receiving token.
        }

        // Read and write token to file
        void save_token();
        void load_token();
    
        // API Requests
        void load_friends();
        void load_guilds();
        void load_channels();
        void load_messages();
        void handle_status_code(int r_status_code, string r_error);
        void setup_cache();
        void read_cache(std::string subdir, std::string file_name);
        void write_cache(std::string subdir, std::string file_name, const Json::Value& data);
};