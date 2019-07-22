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

template<typename A>
int indexof(int,  A){return 0;};

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

inline
void write_char(char* dst,  const char c){
    *dst = c;
}

inline
void write_char(const size_t dst,  const char c){}

template<typename A,  typename B,  typename C,  typename G>
char* get_trailing_data_storage(
    char* src,
    char* dst,
    C reason_name2id,
    A groupindx2reason,
    B& record_contents,
    G& group_starts,   // = 0 (effectively)
    G& group_ends      // = 0 (effectively)
){
    return dst + convert_named_groups(src, (size_t)0, reason_name2id, groupindx2reason, record_contents, group_starts, group_ends);
};

template<typename A,  typename B,  typename C,  typename G>
size_t get_trailing_data_storage(
    char* src,
    size_t dst,
    C& reason_name2id,
    A& groupindx2reason,
    B& record_contents,
    G& group_starts,   // = 0 (effectively)
    G& group_ends      // = 0 (effectively)
){
    return 0;
};

template<typename A,  typename B,  typename C,  typename D,  typename G>
D convert_named_groups(
    char* src,
    D dst,             // either char* or const char*
    C& reason_name2id,
    A& groupindx2reason,
    B& record_contents,
    G& group_starts,   // = 0 (effectively)
    G& group_ends      // = 0 (effectively)
){
    bool last_char_was_bracket = false;
    bool last_chars_were_brckt_qstn = false;
    bool last_chars_were_brckt_qstn_P = false;
    bool last_char_was_backslash = false;
    std::vector<char> group_is_capturing; // ((:?( -> true, false, true // true is represented by 0, false by any other value
    
    push_back_only_if_vector(groupindx2reason, 1); // First match - match[0] - is the entire match.
    
    push_back_only_if_vector(group_starts); // Skip first entry
    push_back_only_if_vector(group_ends);   // Skip first entry
    push_back_only_if_vector(record_contents); // Skip first entry
    
    D trl = get_trailing_data_storage(src, dst, reason_name2id, groupindx2reason, record_contents, group_starts, group_ends);
    write_char(trl++, 0);
    /*
    Avoid heap allocations by re-using this buffer, by appending to the end of it.
    Everything we might wish to append originates from the src itself, so is guaranteed not to overflow, *so long as dst is at least as large as src*.
    */
    while(*src != 0){
        if (last_chars_were_brckt_qstn_P  &&  *src == '<'){
            push_back_only_if_vector(record_contents,  ((*(src + 1) != '!'))); // Do not record contents of the comments if the capture group name begins with !
            
            dst -= 2; // strlen("?P")
            
            char* const group_name = src + 1;
            
            while(*(++src) != '>'){
                if (*src == '\\')
                    ++src;
                write_char(trl++, *src);
            }
            write_char(trl++, 0);
            
            push_back_only_if_vector(groupindx2reason, indexof(reason_name2id, group_name));
            push_back_only_if_vector(group_starts, dst);
            push_back_only_if_vector(group_ends); // NOTE: While I would like to explicitly add 'nullptr' to this, it messes with the type.
            group_is_capturing[group_is_capturing.size()-1] = 0; // Overwrite the value set when '(' was being processed.
            
            last_chars_were_brckt_qstn_P = false;
            
            ++src; // Skip '>'

            // last_char_was_backslash = false; // necessary?
            
            continue;
        }
        
        write_char(dst, *src);
        ++dst;
        
        if (!last_char_was_backslash){
            if (*src == '('){
                const char b = (*(src+1) - '?');
                if (b == 0){
                    push_back_only_if_vector(groupindx2reason, 1); // 1 for Unknown reason matched
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
char* convert_named_groups(char* src,  char* dst,  std::vector<char*>& reason_name2id,  std::vector<A>& groupindx2reason,  std::vector<bool>& record_contents){
    constexpr static const int dummy = 0;
    return convert_named_groups(src, dst, reason_name2id, groupindx2reason, record_contents, dummy, dummy);
};

}
}


#endif
