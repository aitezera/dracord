#pragma once

#include <string>
#include <iostream>
#include <vector>

class Friend {
    public:
        Friend() {}
        ~Friend() {}

        void edit_friend();
        void delete_friend();
        void add_friend();
        void block_friend();
        void unblock_friend();
        void view_friend();
};