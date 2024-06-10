#include "requests.h"
#include "logging.h"


int Requests::login_user() {
    /*
    - Called upon first initialisation of the client
    - Sends a GET request to the Discord API to check if the token is valid
    - Return information for the user and store their token for later use
    */

    Log* log = Log::getInstance();

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
    return 0;
}

void Requests::load_friends() {}
void Requests::load_guilds() {}
void Requests::load_channels() {}
void Requests::load_messages() {}

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
            log->Error("Not Found! The endpoint is incorrect.");
            break;
        case 429:
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

    Requests requests(token);
    return requests.login_user();
}