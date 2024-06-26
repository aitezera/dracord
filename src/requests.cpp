#include "requests.h"
#include "logging.h"

/*
    TODO:
        - Cache the information from the API (friends, guilds, channels, messages)
        - Move onto the SDL2 client and have the requests be used in the client (for the GUI and other things)
*/

// This needs to be here for now until I link it with the main file
Log* logger = Log::getInstance();

int Requests::login_user() {
    logger->Info("[!] Attempting logging user in with Discord API");
    
    if (r_token.empty() && !std::ifstream(r_filename).good()) {
        handle_status_code(401);
        return 1;
    } else {
        load_token();
    }

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + r_base_api + "users/@me").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return 1;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str());

    logger->Info("[!] Token was valid, updating headers");
    updateHeaders();

    logger->Info("[!] User logged in successfully!");

    load_friends();
    load_guilds();
    load_channels();
    load_messages();

    string idd = "";
    string msg = "";
    std::cout << "Enter the ID of the friend you want to message: ";
    std::cin >> idd;

    if (r_friends.find(idd) == r_friends.end()) {
        logger->Error("Friend not found!");
        return 1;
    }

    std::cout << "Enter the message you want to send: ";
    std::cin >> msg;

    Friend friendObj = r_friends[idd];
    friendObj.send_message(std::stol(idd), msg);

    if (!std::ifstream(r_filename).good()) {
        logger->Info("[!] Saving Token for future use");
        save_token();
    }

    return 0;
}

void Requests::load_friends() {
    logger->Info("[!] Loading Friends");

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/relationships"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + r_base_api + "users/@me/relationships").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str()); 

    Json::Value json;
    for (const auto& friends : json) {
        Friend friendObj(friends["id"].asInt64(), friends["username"].asString(),
                         friends["avatar"].asString(), friends["banner"].asString(),
                         friends["bio"].asString(), friends["status"].asInt());
        
        r_friends[friends["id"].asString()] = friendObj;
    }

    logger->Info("[!] Friends have been cached");
    logger->Info("[!] Friends loaded successfully");
}

void Requests::load_guilds() {
    logger->Info("[!] Loading Guilds");

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/guilds"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + r_base_api + "users/@me/guilds").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str());
    
    Json::Value json;
    for (const auto& guild : json) {
        Guild guildObj(guild["id"].asInt64(), guild["owner_id"].asInt64(),
                       guild["name"].asString(), guild["icon"].asString(),
                       guild["banner"].asString());
        
        r_guilds[guild["id"].asString()] = guildObj;
    }

    logger->Info("[!] Guilds have been cached");
    logger->Info("[!] Guilds loaded successfully");
}

void Requests::load_channels() {
    logger->Info("[!] Loading Channels");

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/channels"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + r_base_api + "users/@me/channels").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str());

    
    Json::Value json;
    for (const auto& channel : json ) {
        Channel channelObj(channel["id"].asInt64(), channel["name"].asString(),
                           channel["topic"].asString(), channel["nsfw"].asBool());
        
        r_channels[channel["id"].asString()] = channelObj;
    }

    logger->Info("[!] Channels have been cached");
    logger->Info("[!] Channels loaded successfully");
}

void Requests::load_messages() {
    logger->Info("[!] Loading Messages");
    

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/channels"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + r_base_api + "users/@me/channels").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }
    
    logger->Info(("Response: " + std::string(response.text)).c_str());

    // TODO: Cache it or something
}

void Requests::handle_status_code(int status_code) {
    switch (status_code) {
        case 400:
            logger->Error("Bad Request!");
            break;
        case 401:
            logger->Error("Unauthorized! Check your token.");
            break;
        case 403:
            logger->Error("Forbidden! You don't have permission.");
            break;
        case 404:
            logger->Error("Not Found! The resource you requested doesn't exist.");
            break;
        case 429:
            logger->Error("Too Many Requests! You are being rate-limited.");
            break;
        case 502:
            logger->Error("Bad Gateway! The server was acting as a gateway or proxy and received an invalid response from the upstream server.");
            break;
        default:
            logger->Error(("Error! Status code: " + std::to_string(status_code)).c_str());
            break;
    }
}

void Requests::save_token() {
    logger->Info(("[!] Saving Token at: " + r_filename).c_str());
    std::ofstream out(r_filename, std::ios::binary);
    if (out.is_open()) {
        std::string encrypt_token = r_token;
        for (char &c : encrypt_token) {
            c = c^0x5A;
        }
        out.write(encrypt_token.data(), r_token.size());
        out.close();
        logger->Info(("Token saved to file: " + r_filename).c_str());
    } else {
        logger->Error(("Failed to open file for writing: " + r_filename).c_str());
        out.close();
    }
}

void Requests::load_token() {
    logger->Info(("Loading token from file: " + r_filename).c_str());
    std::ifstream in(r_filename, std::ios::binary | std::ios::ate);
    if (in.is_open()) {
        std::streamsize size = in.tellg();
        in.seekg(0, std::ios::beg);

        r_token.resize(size);
        if (!in.read(r_token.data(), size)) {
            logger->Error(("Failed to read file: " + r_filename).c_str());
        } else {
            for (char &c : r_token) {
                c = c^0x5A;
            }
        }
        in.close();
        logger->Info(("Token loaded from file: " + r_filename).c_str());
    } else {
        logger->Error(("Failed to open file for reading: " + r_filename).c_str());
        in.close();
    }
}

cpr::Header Requests::getHeaders() {
    return r_headers;
}

// Comment this out for make all
int main() {
    // This is here for testing purposes only
    // Main will be removed from this once I have the requests finished
    logger->setFile("requests.log");

    Requests requests;
    return requests.login_user();
}