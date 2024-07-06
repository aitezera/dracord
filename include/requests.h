#pragma once

#include <cpr/cpr.h>
#include <json/json.h>
#include <string>
#include <filesystem>

using std::string;
namespace fs = std::filesystem;

class WebSocket;

class Requests {
    friend class WebSocket;

    public:
        ~Requests() {};
        Requests() {};

        Requests(string uToken) {
            r_token = uToken;  // Set token
        }

        string r_api = "https://discord.com/api/v9/";
        int loginUser();

    private:
        string r_token;
        string r_filename = "token.bin";

        cpr::Header r_headers = {
            {"Content-Type", "application/json"},
            {"Authorization", r_token}
        };

        void updateHeaders() {
            // Set the Authorization header || IF we dont set it's empty
            r_headers["Authorization"] = r_token;
        }

        // Read and write token to file

        void saveToken();
        void loadToken();
    
        // API Requests
        
        void loadFriends();
        void loadGuilds();
        void loadChannels(std::string guildID);
        void loadMessages();
        void handleStatusCode(int r_status_code, string r_error);

        // Cache related functions

        void setupCache();
        void readCache(std::string subdir, std::string file_name);
        void writeCache(std::string subdir, std::string file_name, const Json::Value& data);
        Json::Value parseToJson(const std::string& jsonString);
};