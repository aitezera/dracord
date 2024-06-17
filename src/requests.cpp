#include "requests.h"
#include "logging.h"

/*
    TODO:
        - Save the users token to a file for future uses (maybe a config file?)
        - Cache the information from the API (friends, guilds, channels, messages)
        - Update the TODO to contain the date and time (to help with debugging and tracking changes)
        - Move onto the SDL2 client and have the requests be used in the client (for the GUI and other things)
*/

Log* log = Log::getInstance(); // Get the instance of the logger

int Requests::login_user() {
    log->Info("[!] Logging user in with Discord API");
    
    if (token.empty()) {
        load_token("token");
    }

    if (token.empty()) {
        handle_status_code(401);
        return 1;
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

    // TODO: Cache the user information

    load_friends();
    load_guilds();
    load_channels();
    load_messages();

    log->Info("[!] User logged in successfully!");
    log->Info("[!] Saving Token to file");

    if (!std::ifstream("token").good()) {
        log->Info("[!] Token has not been saved. Saving it now");
        save_token("token");
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
            log->Error("Too Many Requests! You are being rate-limited.");
            break;
        default:
            log->Error(("Error! Status code: " + std::to_string(status_code)).c_str());
            break;
    }
}

void Requests::save_token(const std::string& filename) {
    log->Info(("Saving token to file: " + filename).c_str());
    std::ofstream out(filename, std::ios::binary);
    if (out.is_open()) {
        out.write(token.data(), token.size());
        out.close();
        log->Info(("Token saved to file: " + filename).c_str());
    } else {
        log->Error(("Failed to open file for writing: " + filename).c_str());
    }
}

void Requests::load_token(const std::string& filename) {
    log->Info(("Loading token from file: " + filename).c_str());
    std::ifstream in(filename, std::ios::binary | std::ios::ate);
    if (in.is_open()) {
        std::streamsize size = in.tellg();
        in.seekg(0, std::ios::beg);

        token.resize(size);
        if (!in.read(token.data(), size)) {
            log->Error(("Failed to read file: " + filename).c_str());
        }
        in.close();
        log->Info(("Token loaded from file: " + filename).c_str());
        log->Info(("Token: " + token).c_str());
    } else {
        log->Error(("Failed to open file for reading: " + filename).c_str());
    }
}

// Comment this out for make all
int main() {
    // This is here for testing purposes only
    // Main will be removed from this once I have the requests finished
    string token;
    std::cout << "Enter your Discord token: ";
    std::cin >> token;

    log->setFile("requests.log");

    Requests requests(token);
    return requests.login_user();
}