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

template<typename T>
void push_back_only_if_vector(T t){};

template<typename T,  typename X>
void push_back_only_if_vector(T t,  X x){};

template<typename X>
void push_back_only_if_vector(std::vector<X>& t){
    t.push_back(nullptr);
};

template<>
void push_back_only_if_vector(std::vector<bool>& t){
    t.push_back(false);
};

template<typename X>
void push_back_only_if_vector(std::vector<X>& t,  X x){
    t.push_back(x);
};
#include <stdio.h>
template<typename A,  typename B,  typename C,  typename D>
char* convert_named_groups(
    char* src,
    char* dst,
    std::vector<char*>& reason_name2id,
    std::vector<A>& groupindx2reason,
    B& record_contents,
    C& group_starts,   // = 0 (effectively)
    D& group_ends      // = 0 (effectively)
){
    char* tmp = dst;
    
    bool last_char_was_bracket = false;
    bool last_chars_were_brckt_qstn = false;
    bool last_chars_were_brckt_qstn_P = false;
    bool last_char_was_backslash = false;
    char group_name[128];
    int bracket_depth = 0;
    std::vector<bool> group_bracket_depths; // ((:?( -> true, false, true
    
    groupindx2reason.push_back(1); // First match - match[0] - is the entire match.
    
    push_back_only_if_vector(group_starts); // Skip first entry
    push_back_only_if_vector(group_ends);   // Skip first entry
    push_back_only_if_vector(record_contents); // Skip first entry
    
    while(*src != 0){
        if (last_chars_were_brckt_qstn_P  &&  *src == '<'){
            push_back_only_if_vector(record_contents,  ((*(src + 1) != '!'))); // Do not record contents of the comments if the capture group name begins with !
            
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
            push_back_only_if_vector(group_starts, dst);
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
                    groupindx2reason.push_back(1); // 1 for Unknown reason matched
                    push_back_only_if_vector(record_contents, true);
                    push_back_only_if_vector(group_starts, dst);
                }
                group_bracket_depths.push_back(b);
                ++bracket_depth;
            } else if (*src == ')'){
                --bracket_depth;
                if (group_bracket_depths.back()){
                    push_back_only_if_vector(group_ends, dst);
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

template<typename A>
char* convert_named_groups(char* src,  char* dst,  std::vector<char*>& reason_name2id,  std::vector<A>& groupindx2reason,  std::vector<bool>& record_contents){
    constexpr static const char dummy1 = 0;
    constexpr static const char dummy2 = 0;
    convert_named_groups(src, dst, reason_name2id, groupindx2reason, record_contents, dummy1, dummy2);
};

template<typename A>
char* convert_named_groups(char* src,  char* dst,  std::vector<char*>& reason_name2id,  std::vector<A>& groupindx2reason){
    constexpr static const char dummy1 = 0;
    constexpr static const char dummy2 = 0;
    constexpr static const char dummy3 = 0;
    convert_named_groups(src, dst, reason_name2id, groupindx2reason, dummy1, dummy2, dummy3);
};

}
}


#endif
