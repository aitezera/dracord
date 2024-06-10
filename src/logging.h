#pragma once

#include <iostream>
#include <fstream>
#

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

        // Set to NULL by default for printing to console
        FILE* m_File = NULL;

        Log() {}
    
    public:
        static Log* getInstance();

        void setLevel(Level level);

        void setFile(const char* fileName);

        void Warn(const char* message);

        void Error(const char* message);

        void Info(const char* message);
};