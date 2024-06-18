#pragma once

#include <string>
#include <iostream>
#include <vector>

class Messages {
    public:
        Messages() {}
        ~Messages() {}

        // Overloaded send_message functions
        virtual void send_message(std::string message) = 0;
        virtual void send_message(std::string message, std::string attachment) = 0;
        virtual void send_message(std::string message, std::vector<std::string> attachments) = 0;
        virtual void send_message(std::string message, std::vector<std::string> attachments, std::vector<std::string> mentions) = 0;
        virtual void send_message(std::string message, std::vector<std::string> attachments, std::vector<std::string> mentions, std::string tts) = 0;

        void reply_message();
        void edit_message();
        void delete_message();
        void pin_message();
        void unpin_message();
        void add_reaction();
        void remove_reaction();
};