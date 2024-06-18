#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

class Log {
    public:
        enum Level : char {
            LevelError = 0,
            LevelWarning = 1,
            LevelInfo = 2,
        };
        

    private:
        // Default logging level
        Level m_LogLevel = LevelInfo;
        static Log* instance;

        // Set to nullptr by default for printing to console
        FILE* m_File = nullptr;
        
        std::string getDate();

        Log() {}
        
        // Destructor to clean up resources
        ~Log() {
            if (m_File != nullptr) {
                fclose(m_File);
                m_File = nullptr;
            }
}
    
    public:
        static Log* getInstance();

        static void destroyInstance();

        void setLevel(Level level);

        void setFile(const char* fileName);

        void Warn(const char* message);

        void Error(const char* message);

        void Info(const char* message);
};