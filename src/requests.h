#pragma once

#include <cpr/cpr.h>
#include <iostream>
using std::string;

class Requests {
    public:
        string base_api = "https://discord.com/api/v9/";

        Requests(string uToken) {
            token = uToken;  // Set token
        }

        int login_user();
    private:
        string token;
        

        void load_friends();
        void load_guilds();
        void load_channels();
        void load_messages();

        void handle_status_code(int status_code);
};