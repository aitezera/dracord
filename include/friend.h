#pragma once

#include <string>
#include <iostream>
#include <vector>

class Friend {
    public:
        Friend() {}
        ~Friend() {}

        enum Status {
                online = 0,
                offline,
                idle,
                dnd
        };

        Friend(long id, std::string username, std::string avatar, std::string banner, std::string bio, Status status) {
            this->f_id = id;
            this->f_username = username;
            this->f_avatar = avatar;
            this->f_banner = banner;
            this->f_bio = bio;
            this->f_status = status;
        }

        long f_id;
        std::string f_username; 
        std::string f_avatar;
        std::string f_banner;
        std::string f_bio; // This might be removed later on >> Keeping for now
        Status f_status;

    private:
        void edit_friend();
        void delete_friend();
        void add_friend();
        void block_friend();
        void unblock_friend();
        void view_friend();
};