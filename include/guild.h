#pragma once

#include <string>
#include <iostream>
#include <vector>

class Guild {
    public:
        Guild() {}
        ~Guild() {}

        Guild(long id, long owner_id, std::string name, std::string icon, std::string banner) {
            this->g_id = id;
            this->g_owner_id = owner_id;
            this->g_name = name;
            this->g_icon = icon;
            this->g_banner = banner;
        }

        long g_id;
        long g_owner_id;
        std::string g_name;
        std::string g_icon;
        std::string g_banner;

        std::vector<long> g_members;
        std::vector<std::string> g_roles;
        std::vector<std::string> g_channels;
        std::vector<std::string> g_emojis;
        std::vector<std::string> g_stickers;
        
    private:
        void inv_member();
        void ban_member();
        void change_name();
        void change_description();
        void create_role();
        void delete_role();
        void add_role_to_member();
        void remove_role_from_member();
};