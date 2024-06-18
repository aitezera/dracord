#include "logging.h"

Log* Log::instance = 0;

Log* Log::getInstance() {
    if (instance == 0) {
        instance = new Log();
    }
    return instance;
}

void Log::destroyInstance() {
    if (instance != 0) {
        instance->Info("Destroying Log instance"); // Call Info function using instance pointer
        delete instance;
        instance = nullptr;
    }
}

void Log::setLevel(Level level)
{
    m_LogLevel = level;
}


std::string Log::getDate()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d | %X");

    return ss.str();
}

void Log::setFile(const char* fileName)
{
    m_File = fopen(fileName, "r");
    if (m_File == NULL)
    {
        Log::Error(("Error opening file: " + std::string(fileName) + " will create file and use it.").c_str());
        std::ofstream { fileName }; // Create file
        Log::Info(("Created file: " + std::string(fileName)).c_str());
        Log::Info(("Now using file: " + std::string(fileName)).c_str());
    }
    else {
        Log::Info(("Opened file: " + std::string(fileName)).c_str());
    }
    m_File = fopen(fileName, "a");
}

void Log::Warn(const char* message)
{
    if (m_LogLevel >= LevelWarning)
        if (m_File != NULL)
            fprintf(m_File, "(%s) [WARNING]: %s\n", getDate().c_str(), message);
        else
            std::cout << ("(%s) [WARNING]: ", getDate().c_str()) << message << std::endl;
            std::flush(std::cout);
}

void Log::Error(const char* message)
{
    if (m_LogLevel >= LevelError)
        if (m_File != NULL)
            fprintf(m_File, "(%s) [ERROR]: %s\n", getDate().c_str(), message);
        else
            std::cout << ("(%s) [ERROR]: ", getDate().c_str()) << message << std::endl;
            std::flush(std::cout);
}

void Log::Info(const char* message)
{
    if (m_LogLevel >= LevelInfo)
        if (m_File != NULL)
            fprintf(m_File, "(%s) [INFO]: %s\n", getDate().c_str(), message);
        else
            std::cout << ("(%s) [INFO]: ", getDate().c_str()) << message << std::endl;
            std::flush(std::cout);
}