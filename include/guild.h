#pragma once

#include <string>
#include <iostream>
#include <vector>

class Guild {
    public:
        Guild() {}
        ~Guild() {}
        
        void add_member();
        void remove_member();
        void promote_member();
        void demote_member();
        void change_leader();
        void change_name();
        void change_description();
        void change_motd();
        void change_rank_name();
        void change_rank_permissions();
        void change_rank_order();
        void change_rank_icon();
        void change_rank_color();
        void change_rank_prefix();
        void change_rank_suffix();
        void change_rank_inheritance();
        void change_rank_members();
};