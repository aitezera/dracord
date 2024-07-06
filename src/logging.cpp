#include "logging.h"

//
//_____________________________________________________________________________________________________________________________


Log* Log::instance = nullptr;

//
//_____________________________________________________________________________________________________________________________


Log* Log::getInstance() {
    if (instance == nullptr) {
        instance = new Log();
    }
    return instance;
}

//
//_____________________________________________________________________________________________________________________________


void Log::destroyInstance() {
    if (instance != 0) {
        instance->Info("Destroying Log instance");
        delete instance;
        instance = nullptr;
    }
}

//
//_____________________________________________________________________________________________________________________________


void Log::setLevel(Level level)
{
    m_LogLevel = level;
}

//
//_____________________________________________________________________________________________________________________________


std::string Log::getDate()
{
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d | %X");

    return ss.str();
}

//
//_____________________________________________________________________________________________________________________________


void Log::setFile(const char* fileName)
{
    if (m_File != nullptr) {
        fclose(m_File); // Close any files that are open
        m_File = nullptr;
    }

    m_File = fopen(fileName, "r");
    if (m_File == nullptr)
    {
        Log::Error(("Error opening file: " + std::string(fileName) + " creating file and using file").c_str());
        std::ofstream { fileName }; // Create file
        Log::Info(("Created file: " + std::string(fileName)).c_str());
    }
    else {
        Log::Info(("Opened file: " + std::string(fileName)).c_str());
    }

    if (m_File != nullptr)
        fclose(m_File); // Close read mode file

    m_File = fopen(fileName, "a");
}

//
//_____________________________________________________________________________________________________________________________


void Log::Warn(const char* message) {
    if (m_LogLevel >= LevelWarning) {
        if (m_File != nullptr) {
            fprintf(m_File, "(%s) [WARNING]: %s\n", getDate().c_str(), message);
            fflush(m_File);
        } else {
            std::cout << "(" << getDate() << ") [WARNING]: " << message << std::endl;
        }
    }
}

//
//_____________________________________________________________________________________________________________________________


void Log::Error(const char* message) {
    if (m_LogLevel >= LevelError) {
        if (m_File != nullptr) {
            fprintf(m_File, "(%s) [ERROR]: %s\n", getDate().c_str(), message);
            fflush(m_File);
        } else {
            std::cout << "(" << getDate() << ") [ERROR]: " << message << std::endl;
        }
    }
}

//
//_____________________________________________________________________________________________________________________________


void Log::Info(const char* message) {
    if (m_LogLevel >= LevelInfo) {
        if (m_File != nullptr) {
            fprintf(m_File, "(%s) [INFO]: %s\n", getDate().c_str(), message);
            fflush(m_File);
        } else {
            std::cout << "(" << getDate() << ") [INFO]: " << message << std::endl;
        }
    }
}

//
//_____________________________________________________________________________________________________________________________
