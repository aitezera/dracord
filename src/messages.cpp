#include "messages.h"

// Send message overloaders
void Messages::send_message(std::string message) {};

void Messages::send_message(std::string message, std::string attachment) {};

void Messages::send_message(std::string message, std::vector<std::string> attachments) {};

void Messages::send_message(std::string message, std::vector<std::string> attachments, std::vector<std::string> mentions) {};

void Messages::send_message(std::string message, std::vector<std::string> attachments, std::vector<std::string> mentions, std::string tts) {};


void Messages::reply_message() {};

void Messages::edit_message() {};

void Messages::delete_message() {};

void Messages::pin_message() {};

void Messages::unpin_message() {};

void Messages::add_reaction() {};

void Messages::remove_reaction() {};
