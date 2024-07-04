#include "requests.h"
#include "logging.h"

/*
    TODO:
        - Cache the information from the API (friends, guilds, channels, messages)
        - Move onto the SDL2 client and have the requests be used in the client (for the GUI and other things)
*/

/*
    This is the main file for the requests. This will handle all the requests to the Discord API.
    This will also handle the caching of the information from the API.

    
    I will most likely move to Websockets for messages and such, so this will only really act like a first time prompt to load information
    from the API, or whenever the user launches the application. Though will need to add a way to refresh the cache if the user wants to.

*/

// This needs to be here for now until I link it with the main file
Log* logger = Log::getInstance();


//
//_____________________________________________________________________________________________________________________________
int Requests::loginUser() {

    if (r_token.empty() && !std::ifstream(r_filename).good()) {
        handleStatusCode(401, "Token was Empty and no token file was found!");
        return 1;

    } else if (std::ifstream(r_filename).good()) {
        logger->Info("Loading token from file");
        loadToken();
    }

    logger->Info("Updating Headers to include Token");
    updateHeaders();

    logger->Info("Attempting to login user with Discord API");
    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me"}, r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handleStatusCode(response.status_code, response.error.message);
        return 1;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str()); // Remove this once I know what I need to do with the response
    logger->Info("User logged in successfully!");

    if (!std::ifstream(r_filename).good()) {
        saveToken();
    }

    logger->Info("Attemping to set up cache folders");
    setupCache();

    logger->Info("Loading Friends from the Discord API");
    // Load Friends
    logger->Info("Loading Guilds from the Discord API");
    // Load Guilds
    logger->Info("Loading Channels from the Discord API");
    // Load Channels
    logger->Info("Loading Messages from the Discord API");
    // Load Messages || Leave this to Websockets?

    Json::Value test;
    test = parseToJson(response.text);
    writeCache("friends", "test", test);
    readCache("friends", "test");

    return 0;
}

//
//_____________________________________________________________________________________________________________________________

void Requests::setupCache() {

    if (!std::filesystem::exists("cache")) {
        logger->Info("Creating folder for cache");
        std::filesystem::create_directory("cache");
    }

    if (!std::filesystem::exists("cache/friends")) {
        logger->Info("Creating folder for friends cache");
        std::filesystem::create_directory("cache/friends");
    }

    if (!std::filesystem::exists("cache/guilds")) {
        logger->Info("Creating folder for guilds cache");
        std::filesystem::create_directory("cache/guilds");
    }

    if (!std::filesystem::exists("cache/channels")) {
        logger->Info("Creating folder for channels cache");
        std::filesystem::create_directory("cache/channels");
    }

    if (!std::filesystem::exists("cache/messages")) {
        logger->Info("Creating folder for messages cache");
        std::filesystem::create_directory("cache/messages");
    }
}

//
//_____________________________________________________________________________________________________________________________

void Requests::readCache(std::string subdir, std::string file_name) {
    if (!std::filesystem::exists("cache/" + subdir + "/" + file_name + ".json")) {
        logger->Error(("Cache file doesn't exist: " + subdir + "/" + file_name + ".json").c_str());
        return;
    }
    
    logger->Info(("Reading cache file: " + subdir + "/" + file_name + ".json").c_str());

    Json::Value jsonRead;
    std::ifstream in("cache/" + subdir + "/" + file_name + ".json");
    in >> jsonRead;
    in.close();

    if (!jsonRead.isObject()) {
        logger->Error("Failed to parse cache file to JSON object");
        return;
    }

    if (jsonRead.type() == 7)  { // Object Value = 7
        for (const auto& key : jsonRead.getMemberNames()) {
            const Json::Value& value = jsonRead[key];
            std::string valueStr;

            switch (value.type()) {
                case 0: // Null
                    valueStr = "null";
                    break;
                case 1: // Int
                    valueStr = std::to_string(value.asInt());
                    break;
                case 2: // UInt
                    valueStr = std::to_string(value.asUInt());
                    break;
                case 3: // Double
                    valueStr = std::to_string(value.asDouble());
                    break;
                case 4: // String
                    valueStr = value.asString();
                    break;
                case 5: // Boolean
                    valueStr = value.asBool() ? "true" : "false";
                    break;
                case 6: // Array
                    valueStr = "Array";
                    break;
                case 7: // Object
                    valueStr = "Object";
                    break;
                default:
                    valueStr = "Unknown";
                    break;
            }

            logger->Info(("Key: " + key + " || Value: " + valueStr).c_str());
        }
    } else {
        logger->Error("Failed to read cache file");
        return;
    }

    logger->Info("Cache file read successfully");
}

//
//_____________________________________________________________________________________________________________________________

void Requests::writeCache(std::string subdir, std::string file_name, const Json::Value& data) {    
    Json::StreamWriterBuilder writer;
    writer["indentation"] = "\t";
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    
    logger->Info(("Writing cache file: " + subdir + "/" + file_name + ".json").c_str());

    std::ofstream out("cache/" + subdir + "/" + file_name + ".json");
    jsonWriter->write(data, &out);
    out.close();

    logger->Info("Cache file written successfully");
}

Json::Value Requests::parseToJson(const std::string& jsonString) {
    Json::Value root;
    std::string errs;
    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

    if (!reader->parse(jsonString.c_str(), jsonString.c_str() + jsonString.length(), &root, &errs)) {
        throw std::runtime_error("Failed to parse JSON: " + errs);
    }

    return root;
}

//
//_____________________________________________________________________________________________________________________________

void Requests::loadFriends() {
    logger->Info("Loading Friends");

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/relationships"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handleStatusCode(response.status_code, response.error.message);
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str()); 

}

//
//_____________________________________________________________________________________________________________________________

void Requests::loadGuilds() {
    logger->Info("Loading Guilds");

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/guilds"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handleStatusCode(response.status_code, response.error.message);
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str());
}

//
//_____________________________________________________________________________________________________________________________

void Requests::loadChannels() {
    logger->Info("Loading Channels");

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/channels"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handleStatusCode(response.status_code, response.error.message);
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str());
}

//
//_____________________________________________________________________________________________________________________________

void Requests::loadMessages() {
    logger->Info("Loading Messages");
    
    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/channels"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handleStatusCode(response.status_code, response.error.message);
        return;
    }
    
    logger->Info(("Response: " + std::string(response.text)).c_str());
}

//
//_____________________________________________________________________________________________________________________________

void Requests::handleStatusCode(int status_code, std::string message = "") {
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
            logger->Error(("Error!: " + message).c_str());
            break;
    }
}

//
//_____________________________________________________________________________________________________________________________

void Requests::saveToken() {
    logger->Info(("Saving Token at: " + r_filename).c_str());
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

//
//_____________________________________________________________________________________________________________________________

void Requests::loadToken() {
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

//
//_____________________________________________________________________________________________________________________________


// Comment this out for make all
int main() {
    // This is here for testing purposes only
    // Main will be removed from this once I have the requests finished
    std::string token;

    Requests requests(token);
    requests.loginUser();

    return 0;
}