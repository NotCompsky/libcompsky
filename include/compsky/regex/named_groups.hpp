/*
This script finds named capture groups (which C++ boost regex does not support), and uses them to relate the group number to a map and vector.

Multiple groups can share the same name. Group names can include any character except '>', which must be escaped. Note that special escapes (such as \n, \t and \0) are not parsed as their usual values, but only as the literal value of the second char (in these cases, n, t and 0 respectively).
*/


#ifndef __COMPSKY_REGEX_NAMED_GROUPS_H__
#define __COMPSKY_REGEX_NAMED_GROUPS_H__

#include <map>
#include <vector>


namespace compsky {
namespace regex {

template<typename A>
auto indexof(std::vector<A>& ls,  A x){
    auto i = 0;
    while(i < ls.size()){
        A itr = ls[i];
        A y = x;
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
    ls.push_back(x);
    return i;
};

template<typename B>
char* convert_named_groups(char* src,  char* dst,  std::vector<char*>& reason_name2id,  std::vector<B>& groupindx2reason,  std::vector<char*>& group_starts,  std::vector<char*>& group_ends){
    char* tmp = dst;
    
    bool last_char_was_bracket = false;
    bool last_chars_were_brckt_qstn = false;
    bool last_chars_were_brckt_qstn_P = false;
    bool last_char_was_backslash = false;
    char group_name[128];
    int bracket_depth = 0;
    std::vector<bool> group_bracket_depths; // ((:?( -> true, false, true
    
    groupindx2reason.push_back(1); // First match - match[0] - is the entire match.
    group_starts.push_back(0); // Skip first entry
    group_ends.push_back(0);   // Skip first entry
    
    while(*src != 0){
        if (last_chars_were_brckt_qstn_P  &&  *src == '<'){
            dst -= 2; // strlen("?P")
            
            char* itr = group_name;
            while(*(++src) != '>'){
                if (*src == '\\')
                    ++src;
                *(itr++) = *src;
            }
            *itr = 0;
            
            const size_t len = (size_t)itr - (size_t)group_name;
            
            void* dummy = malloc(len + 1);
            if (dummy == nullptr)
                exit(900);
            
            char* group_name_allocd = (char*)dummy;
            memcpy(group_name_allocd,  group_name,  len + 1); // Include terminating \0
            
            groupindx2reason.push_back(indexof(reason_name2id, group_name_allocd));
            //printf("Group %d is %s\n",  groupindx2reason.size() - 1,  group_name_allocd);
            group_starts.push_back(dst);
            group_bracket_depths.push_back(true);
            
            last_chars_were_brckt_qstn_P = false;
            
            ++src; // Skip '>'
            
            continue;
        }
        
        *(dst++) = *src;
        
        if (!last_char_was_backslash){
            if (*src == '('){
                const bool b = (*(src+1) != '?');
                if (b){
                    //printf("%.10s\n", src);
                    groupindx2reason.push_back(1); // 1 for Unknown reason matched
                    group_starts.push_back(dst);
                }
                group_bracket_depths.push_back(b);
                ++bracket_depth;
            } else if (*src == ')'){
                --bracket_depth;
                if (group_bracket_depths.back()){
                    group_ends.push_back(dst);
                }
                group_bracket_depths.pop_back();
            }
        }
        
        
        last_chars_were_brckt_qstn_P = (last_chars_were_brckt_qstn  &&  *src == 'P');
        last_chars_were_brckt_qstn = (last_char_was_bracket  &&  *src == '?');
        last_char_was_bracket = (*src == '('  &&  !last_char_was_backslash);
        last_char_was_backslash = (*src == '\\');
        
        ++src;
    }
    return dst;
};

template<typename B>
char* convert_named_groups(char* src,  char* dst,  std::vector<char*>& reason_name2id,  std::vector<B>& groupindx2reason){
    std::vector<char*> dummy1;
    std::vector<char*> dummy2;
    return convert_named_groups(src, dst, reason_name2id, groupindx2reason, dummy1, dummy2);
};

}
}


#endif
