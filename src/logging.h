#pragma once

#include <iostream>

class Log {

    public:
        enum Level : char {};
    
    private:
        Level m_LogLevel;
    
    public:
        void setLevel(Level level)
        {
            m_LogLevel = level;
        }

        void Warn(const char* message);

        void Error(const char* message);

        void Info(const char* message);
};