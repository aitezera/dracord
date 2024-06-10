#include "logging.h"

class Log {
    public:
        enum Level : char {
            Error = 0,
            Warning = 1,
            Info = 2,
        };
    
    private:
        // Default logging level
        Level m_LogLevel = Info;
    
    public:
        void setLevel(Level level)
        {
            m_LogLevel = level;
        }

        void Warn(const char* message)
        {
            if (m_LogLevel >= Warning)
                std::cout << "[WARNING]: " << message << std::endl;
        }

        void Error(const char* message)
        {
            if (m_LogLevel >= Error)
                std::cout << "[ERROR]: " << message << std::endl;
        }

        void Info(const char* message)
        {
            if (m_LogLevel >= Info)
                std::cout << "[INFO]: " << message << std::endl;
        }
};