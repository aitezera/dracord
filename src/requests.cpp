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

int Requests::login_user() {

    if (r_token.empty() && !std::ifstream(r_filename).good()) {
        handle_status_code(401, "Token was Empty and no token file was found!");
        return 1;

    } else if (std::ifstream(r_filename).good()) {
        logger->Info("Loading token from file");
        load_token();
    }

    logger->Info("Updating Headers to include Token");
    updateHeaders();

    logger->Info("Attempting to login user with Discord API");
    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me"}, r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code, response.error.message);
        return 1;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str()); // Remove this once I know what I need to do with the response
    logger->Info("User logged in successfully!");

    if (!std::ifstream(r_filename).good()) {
        save_token();
    }

    logger->Info("Attemping to set up cache folders");
    setup_cache();

    logger->Info("Loading Friends from the Discord API");
    // Load Friends
    logger->Info("Loading Guilds from the Discord API");
    // Load Guilds
    logger->Info("Loading Channels from the Discord API");
    // Load Channels
    logger->Info("Loading Messages from the Discord API");
    // Load Messages || Leave this to Websockets?


    write_cache("friends", "test", response.text);
    read_cache("friends", "test");

    return 0;
}

void Requests::setup_cache() {

    if (!std::filesystem::exists("cache")) {
        logger->Info("Creating folder for cache");
        std::filesystem::create_directory("cache");
    } else {
        logger->Info("Cache folder already exists");
    }

    if (!std::filesystem::exists("cache/friends")) {
        logger->Info("Creating folder for friends cache");
        std::filesystem::create_directory("cache/friends");
    } else {
        logger->Info("Friends cache folder already exists");
    }

    if (!std::filesystem::exists("cache/guilds")) {
        logger->Info("Creating folder for guilds cache");
        std::filesystem::create_directory("cache/guilds");
    } else {
        logger->Info("Guilds cache folder already exists");
    }

    if (!std::filesystem::exists("cache/channels")) {
        logger->Info("Creating folder for channels cache");
        std::filesystem::create_directory("cache/channels");
    } else {
        logger->Info("Channels cache folder already exists");
    }

    if (!std::filesystem::exists("cache/messages")) {
        logger->Info("Creating folder for messages cache");
        std::filesystem::create_directory("cache/messages");
    } else {
        logger->Info("Messages cache folder already exists");
    }
}

void Requests::read_cache(std::string subdir, std::string file_name) {
    if (!std::filesystem::exists("cache/" + subdir + "/" + file_name + ".json")) {
        logger->Error(("Cache file doesn't exist: " + subdir + "/" + file_name + ".json").c_str());
        return;
    }

    Json::Value json;
    std::ifstream in("cache/" + subdir + "/" + file_name + ".json");
    in >> json;
    in.close();

    if (json.isObject()) {
        for (const auto& key : json.getMemberNames()) {
            logger->Info(("Key: " + key).c_str());
        }
    } else {
        logger->Error("Cache file is not a JSON object");
        return;
    }

    logger->Info("Cache file read successfully");
}

void Requests::write_cache(std::string subdir, std::string file_name, const Json::Value& data) {    
    Json::StreamWriterBuilder writer;
    writer["indentation"] = "\t";
    std::unique_ptr<Json::StreamWriter> jsonWriter(writer.newStreamWriter());
    
    std::ofstream out("cache/" + subdir + "/" + file_name + ".json");
    jsonWriter->write(data, &out);
    out.close();

    logger->Info("Cache file written successfully");
}


void Requests::load_friends() {
    logger->Info("Loading Friends");

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/relationships"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code, response.error.message);
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str()); 

    Json::Value json;
    for (const auto& friends : json) {
        Friend friendObj(friends["id"].asInt64(), friends["username"].asString(),
                         friends["avatar"].asString(), friends["banner"].asString(),
                         friends["bio"].asString(), friends["status"].asInt());
        
        r_friends[friends["id"].asString()] = friendObj;
        std::cout << friends["id"].asString() << std::endl;        
    }

}

void Requests::load_guilds() {
    logger->Info("Loading Guilds");

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/guilds"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code, response.error.message);
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

    logger->Info("Guilds have been cached");
    logger->Info("Guilds loaded successfully");
}

void Requests::load_channels() {
    logger->Info("Loading Channels");

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/channels"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code, response.error.message);
        return;
    }

    logger->Info(("Response: " + std::string(response.text)).c_str());

    Json::Value json;
    for (const auto& channel : json ) {
        Channel channelObj(channel["id"].asInt64(), channel["name"].asString(),
                           channel["topic"].asString(), channel["nsfw"].asBool());
        
        r_channels[channel["recipents"]["id"].asString()] = channelObj;
    }

    logger->Info("Channels have been cached");
    logger->Info("Channels loaded successfully");
}

void Requests::load_messages() {
    logger->Info("Loading Messages");
    

    cpr::Response response = cpr::Get(cpr::Url{r_base_api + "users/@me/channels"}, r_headers=r_headers);
    logger->Info(("Using GET response with URL: " + std::string(response.url)).c_str());

    if (response.status_code != 200) {
        handle_status_code(response.status_code, response.error.message);
        return;
    }
    
    logger->Info(("Response: " + std::string(response.text)).c_str());

    // TODO: Cache it or something
}

void Requests::handle_status_code(int status_code, std::string message = "") {
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

void Requests::save_token() {
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
    std::string token = "";
    //logger->setFile("requests.log");

    Requests requests(token);
    requests.login_user();

    return 0;
}