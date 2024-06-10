#include "logging.h"
// TODO: allow to customise the way the logger is displayed e.g (time, date, etc)

void Log::setLevel(Level level)
{
    m_LogLevel = level;
}

void Log::setFile(const char* fileName)
{
    m_File = fopen(fileName, "r");
    if (m_File == NULL)
    {
        Log::Error(("Error opening file: " + std::string(fileName) + " creating file.").c_str());
        std::ofstream { fileName }; // Create file
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
            fprintf(m_File, "[WARNING]: %s\n", message);
        else
            std::cout << "[WARNING]: " << message << std::endl;
            std::flush(std::cout);
}

void Log::Error(const char* message)
{
    if (m_LogLevel >= LevelError)
        if (m_File != NULL)
            fprintf(m_File, "[ERROR]: %s\n", message);
        else
            std::cout << "[ERROR]: " << message << std::endl;
            std::flush(std::cout);
}

void Log::Info(const char* message)
{
    if (m_LogLevel >= LevelInfo)
        if (m_File != NULL)
            fprintf(m_File, "[INFO]: %s\n", message);
        else
            std::cout << "[INFO]: " << message << std::endl;
            std::flush(std::cout);
}