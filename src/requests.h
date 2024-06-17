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

        void updateHeaders() {
            headers["Authorization"] = token; // Set Headers upon receiving token. Otherwise this will be empty and fail
        }

    private:
        string token;
        string m_filename = "token";

        cpr::Header headers = {
            {"Content-Type", "application/json"},
            {"Authorization", token}
        };

        // Read and write token to file
        void save_token();
        void load_token();
        

        void load_friends();
        void load_guilds();
        void load_channels();
        void load_messages();

        void handle_status_code(int status_code);
};