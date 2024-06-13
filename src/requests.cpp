#include "requests.h"
#include "logging.h"

int Requests::login_user() {
    /*
    - Called upon first initialisation of the client
    - Sends a GET request to the Discord API to check if the token is valid
    - Return information for the user and store their token for later use
    */

    Log* log = Log::getInstance();
    log->Info("[!] Logging user in with Discord API");

    if (token.empty()) {
        handle_status_code(401);
        return 1;
    }

    cpr::Header headers = {
            {"Content-Type", "application/json"},
            {"Authorization", token}
        };

    cpr::Response response = cpr::Get(cpr::Url{base_api + "users/@me"}, headers=headers);
    log->Info(("Using GET response with URL: " + base_api + "users/@me").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return 1;
    }

    log->Info(("Response: " + std::string(response.text)).c_str());

    // TODO: Cache the user information

    load_friends();
    load_guilds();
    load_channels();
    load_messages();

    log->Info("[!] User logged in successfully!");
    log->Info("[!] Saving Token to file");

    // TODO: Save the token to a file

    return 0;
}

void Requests::load_friends() {
    Log* log = Log::getInstance();
    log->Info("[!] Loading Friends");

    cpr::Header headers = {
            {"Content-Type", "application/json"},
            {"Authorization", token}
    };

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
    Log* log = Log::getInstance();
    log->Info("[!] Loading Guilds");

    cpr::Header headers = {
            {"Content-Type", "application/json"},
            {"Authorization", token}
    };

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
    Log* log = Log::getInstance();
    log->Info("[!] Loading Channels");

    cpr::Header headers = {
            {"Content-Type", "application/json"},
            {"Authorization", token}
    };

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
    Log* log = Log::getInstance();
    log->Info("[!] Loading Messages");

    cpr::Header headers = {
            {"Content-Type", "application/json"},
            {"Authorization", token}
    };

    cpr::Response response = cpr::Get(cpr::Url{base_api + "users/@me/channels"}, headers=headers);
    log->Info(("Using GET response with URL: " + base_api + "users/@me/channels").c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code);
        return;
    }
    

    log->Info(("Response: " + std::string(response.text)).c_str());

    // TODO: Cache it or something
}


void Requests::handle_status_code(int status_code) {
    Log* log = Log::getInstance();

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
            log->Error("Too Many Requests! You are being rate-limited.");
            break;
        default:
            log->Error(("Error! Status code: " + std::to_string(status_code)).c_str());
            break;
    }
}

int main() {
    // This is here for testing purposes only
    // Main will be removed from this once I have the requests finished
    string token;
    std::cout << "Enter your Discord token: ";
    std::cin >> token;
    
    Log* log = Log::getInstance();
    log->setFile("requests.log");

    Requests requests(token);
    return requests.login_user();
}