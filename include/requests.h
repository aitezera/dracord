#pragma once

#include <cpr/cpr.h>
#include <json/json.h>
#include <string>
#include <filesystem>


using std::string;
namespace fs = std::filesystem;
class Requests {
    public:
        ~Requests() {}
        
        string r_base_api = "https://discord.com/api/v9/";
        string r_base_gateway = "wss://gateway.discord.gg/?v=9&encoding=json";
        string r_base_cdn = "https://cdn.discordapp.com/";
        string r_base_invite = "https://discord.gg/";

        Requests(string uToken) {
            r_token = uToken;  // Set token
        }

        cpr::Header getHeaders() {
            return r_headers;
        }

        static string getToken() {
            return r_token;
        }
        
        int loginUser();

    private:
        static string r_token;
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
        void loadChannels();
        void loadMessages();
        void handleStatusCode(int r_status_code, string r_error);

        // Cache related functions

        void setupCache();
        void readCache(std::string subdir, std::string file_name);
        void writeCache(std::string subdir, std::string file_name, const Json::Value& data);
        Json::Value parseToJson(const std::string& jsonString);
};