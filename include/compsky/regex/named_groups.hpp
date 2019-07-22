/*
This script finds named capture groups (which C++ boost regex does not support), and uses them to relate the group number to a map and vector.

Multiple groups can share the same name. Group names can include any character except '>', which must be escaped. Note that special escapes (such as \n, \t and \0) are not parsed as their usual values, but only as the literal value of the second char (in these cases, n, t and 0 respectively).
*/


#ifndef LIBCOMPSKY_REGEX_NAMED_GROUPS_HPP
#define LIBCOMPSKY_REGEX_NAMED_GROUPS_HPP

#include <map>
#include <vector>
#include <string.h> // for memcpy


namespace compsky {
namespace regex {

struct CharPaar {
    char* const  str;
    const size_t len;
    CharPaar(char* const s,  const size_t l) : str(s), len(l) {};
};

inline
auto indexof(std::vector<CharPaar>& ls,  char* const str,  const size_t len){
    auto i = 0;
    while(i < ls.size()){
        const CharPaar x = ls[i];
        for (auto j = 0;  j < len  &&  j < x.len;  ++j)
            if (x.str[j] != str[j])
                goto break_twice;
        return i;
        
        break_twice:
        ++i;
    }
    ls.emplace_back(str, len);
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

inline
void push_back_only_if_vector(std::vector<bool>& t){
    t.push_back(false);
}

template<typename X>
void push_back_only_if_vector(std::vector<X>& t,  X x){
    t.push_back(x);
};

template<typename T,  typename X,  typename N>
void assign_only_if_vector(T t,  N n,  X x){};

template<typename X,  typename N>
void assign_only_if_vector(std::vector<X>& t,  N n,  X x){
    t[n] = x;
};

template<typename T>
size_t vector_size(T t){
    return 0;
};

template<typename X>
size_t vector_size(std::vector<X> v){
    return v.size();
};

template<typename T,  typename N>
bool is_vector_element_nullptr(T t,  N n){
    return true;
};

template<typename X,  typename N>
bool is_vector_element_nullptr(std::vector<X*> v,  N n){
    return (v[n] == nullptr);
};

template<typename A,  typename B,  typename C,  typename D>
char* convert_named_groups(
    char* src,
    char* dst,
    std::vector<CharPaar>& reason_name2id,
    std::vector<A>& groupindx2reason,
    B& record_contents,
    C& group_starts,   // = 0 (effectively)
    D& group_ends      // = 0 (effectively)
){
    bool last_char_was_bracket = false;
    bool last_chars_were_brckt_qstn = false;
    bool last_chars_were_brckt_qstn_P = false;
    bool last_char_was_backslash = false;
    std::vector<char> group_is_capturing; // ((:?( -> true, false, true // true is represented by 0, false by any other value
    
    groupindx2reason.push_back(1); // First match - match[0] - is the entire match.
    
    push_back_only_if_vector(group_starts); // Skip first entry
    push_back_only_if_vector(group_ends);   // Skip first entry
    push_back_only_if_vector(record_contents); // Skip first entry
    
    while(*src != 0){
        if (last_chars_were_brckt_qstn_P  &&  *src == '<'){
            push_back_only_if_vector(record_contents,  ((*(src + 1) != '!'))); // Do not record contents of the comments if the capture group name begins with !
            
            dst -= 2; // strlen("?P")
            
            char* const group_name = src + 1;
            while(*(++src) != '>'){
                if (*src == '\\')
                    ++src;
            }
            const size_t len = (uintptr_t)src - (uintptr_t)group_name;
            
            groupindx2reason.push_back(indexof(reason_name2id, group_name, len));
            push_back_only_if_vector(group_starts, dst);
            push_back_only_if_vector(group_ends); // NOTE: While I would like to explicitly add 'nullptr' to this, it messes with the type.
            group_is_capturing[group_is_capturing.size()-1] = 0; // Overwrite the value set when '(' was being processed.
            
            last_chars_were_brckt_qstn_P = false;
            
            ++src; // Skip '>'

            // last_char_was_backslash = false; // necessary?
            
            continue;
        }
        
        *(dst++) = *src;
        
        if (!last_char_was_backslash){
            if (*src == '('){
                const char b = (*(src+1) - '?');
                if (b == 0){
                    groupindx2reason.push_back(1); // 1 for Unknown reason matched
                    push_back_only_if_vector(record_contents, true);
                    push_back_only_if_vector(group_starts, dst);
                    push_back_only_if_vector(group_ends); // NOTE: While I would like to explicitly add 'nullptr' to this, it messes with the type.
                }
                group_is_capturing.push_back(b);
            } else if (*src == ')'){
                if (group_is_capturing.back() == 0){
                    size_t k = vector_size(group_ends);
                    while(!is_vector_element_nullptr(group_ends, --k)); // Cycle through until encounter a nullptr
                    assign_only_if_vector(group_ends,  k,  dst);
                }
                group_is_capturing.pop_back();
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
char* convert_named_groups(char* src,  char* dst,  std::vector<CharPaar>& reason_name2id,  std::vector<A>& groupindx2reason,  std::vector<bool>& record_contents){
    constexpr static const char dummy1 = 0;
    constexpr static const char dummy2 = 0;
    return convert_named_groups(src, dst, reason_name2id, groupindx2reason, record_contents, dummy1, dummy2);
};

template<typename A>
char* convert_named_groups(char* src,  char* dst,  std::vector<CharPaar>& reason_name2id,  std::vector<A>& groupindx2reason){
    constexpr static const char dummy1 = 0;
    constexpr static const char dummy2 = 0;
    constexpr static const char dummy3 = 0;
    return convert_named_groups(src, dst, reason_name2id, groupindx2reason, dummy1, dummy2, dummy3);
};

}
}


#endif
