#include "compsky/regex/named_groups.hpp"


auto indexof(std::vector<const char*>& ls,  char* x){
    auto i = 0;
    while(i < ls.size()){
        char* itr = ls[i];
        char* y = x;
        while(*itr != 0){
            if (*itr != *y)
                goto break_twice;
            ++y;
            ++itr;
        }
        return i;
        
        break_twice:
        ++i;
    }
    return i;
};
