#include "requests.h"
#include "logging.h"

/*
    TODO:
        - Cache the information from the API (friends, guilds, channels, messages)
        - Move onto the SDL2 client and have the requests be used in the client (for the GUI and other things)
*/

Log* log = Log::getInstance(); // Create a new instance of the Log class

int Requests::login_user() {
    log->Info("[!] Attempting logging user in with Discord API");
    
    if (token.empty() && !std::ifstream(m_filename).good()) {
        handle_status_code(401);
        return 1;
    } else {
        load_token();
    }

    log->Info("[!] Updating Headers to include the token");
    updateHeaders();

    cpr::Response response = cpr::Get(cpr::Url{base_api + "users/@me"}, headers=headers);
    log->Info(("Using GET response with URL: " + base_api + "users/@me").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return 1;
    }

    log->Info(("Response: " + std::string(response.text)).c_str());

    //load_friends();
    //load_guilds();
    //load_channels();
    //load_messages();

    log->Info("[!] User logged in successfully!");

    if (!std::ifstream(m_filename).good()) {
        log->Info("[!] Saving Token for future use");
        save_token();
    }

    return 0;
}

void Requests::load_friends() {
    log->Info("[!] Loading Friends");

    cpr::Response response = cpr::Get(cpr::Url{base_api + "users/@me/relationships"}, headers=headers);
    log->Info(("Using GET response with URL: " + base_api + "users/@me/relationships").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }

    log->Info(("Response: " + std::string(response.text)).c_str()); 

    // TODO: Cache it or something
}

void Requests::load_guilds() {
    log->Info("[!] Loading Guilds");

    cpr::Response response = cpr::Get(cpr::Url{base_api + "users/@me/guilds"}, headers=headers);
    log->Info(("Using GET response with URL: " + base_api + "users/@me/guilds").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }

    log->Info(("Response: " + std::string(response.text)).c_str());

    // TODO: Cache it or something    
}

void Requests::load_channels() {
    log->Info("[!] Loading Channels");

    cpr::Response response = cpr::Get(cpr::Url{base_api + "users/@me/channels"}, headers=headers);
    log->Info(("Using GET response with URL: " + base_api + "users/@me/channels").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }

    log->Info(("Response: " + std::string(response.text)).c_str());

    // TODO: Cache it or something
}

void Requests::load_messages() {
    log->Info("[!] Loading Messages");
    

    cpr::Response response = cpr::Get(cpr::Url{base_api + "users/@me/channels"}, headers=headers);
    log->Info(("Using GET response with URL: " + base_api + "users/@me/channels").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }
    

    log->Info(("Response: " + std::string(response.text)).c_str());

    // TODO: Cache it or something
}

void Requests::send_server_message(long channel_id, string message) {
    log->Info("[!] Sending Server Message");

    Json::Value json;
    json["content"] = message;
    json["tts"] = false;

    Json::StreamWriterBuilder builder;
    std::string json_string = Json::writeString(builder, json);

    cpr::Response response = cpr::Post(cpr::Url{base_api + "channels/" + std::to_string(channel_id) + "/messages"}, cpr::Body{json_string}, headers=headers);
    log->Info(("Using POST response with URL: " + base_api + "channels/" + std::to_string(channel_id) + "/messages").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }

    log->Info(("Response: " + std::string(response.text)).c_str());
}


void Requests::send_friend_message(long channel_id, string message) {
    log->Info("[!] Sending Friend Message");

    Json::Value json;
    json["content"] = message;
    json["tts"] = false;

    Json::StreamWriterBuilder builder;
    std::string json_string = Json::writeString(builder, json);

    cpr::Response response = cpr::Post(cpr::Url{base_api + "channels/" + std::to_string(channel_id) + "/messages"}, cpr::Body{json_string}, headers=headers);
    log->Info(("Using POST response with URL: " + base_api + "channels/" + std::to_string(channel_id) + "/messages").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }

    log->Info(("Response: " + std::string(response.text)).c_str());
}


void Requests::handle_status_code(int status_code) {
    switch (status_code) {
        case 400:
            log->Error("Bad Request!");
            break;
        case 401:
            log->Error("Unauthorized! Check your token.");
            break;
        case 403:
            log->Error("Forbidden! You don't have permission.");
            break;
        case 404:
            log->Error("Not Found! The resource you requested doesn't exist.");
            break;
        case 429:
            log->Error("Too Many Requests! You are being rate-limited.");
            break;
        case 502:
            log->Error("Bad Gateway! The server was acting as a gateway or proxy and received an invalid response from the upstream server.");
            break;
        default:
            log->Error(("Error! Status code: " + std::to_string(status_code)).c_str());
            break;
    }
}

void Requests::save_token() {
    log->Info(("[!] Saving Token at: " + m_filename).c_str());
    std::ofstream out(m_filename, std::ios::binary);
    if (out.is_open()) {
        std::string encrypt_token = token;
        for (char &c : encrypt_token) {
            c = c^0x5A;
        }
        out.write(encrypt_token.data(), token.size());
        out.close();
        log->Info(("Token saved to file: " + m_filename).c_str());
    } else {
        log->Error(("Failed to open file for writing: " + m_filename).c_str());
        out.close();
    }
}

void Requests::load_token() {
    log->Info(("Loading token from file: " + m_filename).c_str());
    std::ifstream in(m_filename, std::ios::binary | std::ios::ate);
    if (in.is_open()) {
        std::streamsize size = in.tellg();
        in.seekg(0, std::ios::beg);

        token.resize(size);
        if (!in.read(token.data(), size)) {
            log->Error(("Failed to read file: " + m_filename).c_str());
        } else {
            for (char &c : token) {
                c = c^0x5A;
            }
        }
        in.close();
        log->Info(("Token loaded from file: " + m_filename).c_str());
    } else {
        log->Error(("Failed to open file for reading: " + m_filename).c_str());
        in.close();
    }
}

// Comment this out for make all
int main() {
    // This is here for testing purposes only
    // Main will be removed from this once I have the requests finished

    string token = ""; // Set empty so that token grabs from file
    log->setFile("requests.log");

    Requests requests(token);
    return requests.login_user();
}