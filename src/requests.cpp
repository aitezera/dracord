#include <cpr/cpr.h>
#include <iostream>
using std::string;

class Requests {
    public:
        string base_api = "https://discord.com/api/v9/";

        Requests(string uToken) {
            token = uToken;  // Set token
        }

        int login_user() {
            if (token.empty()) {
                handle_status_code(401);
                return 1;
            }

            cpr::Header headers = {
                {"Content-Type", "application/json"},
                {"Authorization", token}
            };

            cpr::Response response = cpr::Get(cpr::Url{base_api + "users/@me"}, headers=headers);
            
            std::cout << "Using GET response with URL: " << response.url << std::endl;

            if (response.status_code != 200) {
                handle_status_code(response.status_code);
                return 1;
            }

            std::cout << response.text << std::endl;
            return 0;
        }

    private:
        string token;

        void load_friends() {}
        void load_guilds() {}
        void load_channels() {}
        void load_messages() {}

        void handle_status_code(int status_code) {
            switch (status_code) {
                case 400:
                    std::cerr << "Bad Request!" << std::endl;
                    break;
                case 401:
                    std::cerr << "Unauthorized! Check your token." << std::endl;
                    break;
                case 403:
                    std::cerr << "Forbidden! You don't have permission." << std::endl;
                    break;
                case 404:
                    std::cerr << "Not Found! The endpoint is incorrect." << std::endl;
                    break;
                case 429:
                    std::cerr << "Too Many Requests! You are being rate-limited." << std::endl;
                    break;
                default:
                    std::cerr << "Error! Status code: " << status_code << std::endl;
                    break;
            }
        }
};

int main() {
    // This is here for testing purposes only
    // Main will be removed from this once I have the requests finished
    string token;
    std::cout << "Enter your Discord token: ";
    std::cin >> token;

    Requests requests(token);
    return requests.login_user();
}