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
    int i = 0;
    const int sz = ls.size();
    while(i < sz){
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

template<typename A,  typename B,  typename C,  typename D,  typename G>
D convert_named_groups(
    char* src,
    D dst,             // either char* or const char*
    size_t& trailing_data_size,
    C& reason_name2id,
    A& groupindx2reason,
    B& record_contents,
    G& group_starts,   // = 0 (effectively)
    G& group_ends      // = 0 (effectively)
);

inline
char* get_final_length_and_trailing_data_size(
    char* src,
    char* dst,
    size_t& trailing_data_size
){
    constexpr static const int dummy = 0;
    return dst + convert_named_groups(src, (size_t)0, trailing_data_size, dummy, dummy, dummy, dummy, dummy); // The latter returns the number of resulting elements that will inhabit dst
};

inline
size_t get_final_length_and_trailing_data_size(char*,  size_t dst, size_t&){ return 0; }

inline
char* alloc_trailing_data(char* const dst,  const size_t trailing_data_size){
    void* dummy = malloc(trailing_data_size);
    if (dummy == nullptr)
        exit(4096);
    return (char*)dummy;
}

inline
size_t alloc_trailing_data(const size_t dst,  const size_t trailing_data_size){
    return 0;
}

inline
void free_trailing_data(char* const s){
    free(s);
}

inline
void free_trailing_data(size_t const s){}

inline
void memcpy_trailing_data(char* const dst,  char* const trl_orig,  size_t const trailing_data_size){
    *(dst) = 0;
    memcpy(dst + 1,  trl_orig,  trailing_data_size);
    dst[trailing_data_size] = 0;
    // NOTE: We at worst add the equivalent of "<>", which is obviously less than that removed from 'src': "?P<>"
}

inline
void memcpy_trailing_data(size_t const,  size_t const,  size_t const){}

template<typename A,  typename B,  typename C,  typename D,  typename G>
D convert_named_groups(
    char* src,
    D dst,             // either char* or const char*
    size_t& trailing_data_size,
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
    std::vector<bool> group_is_capturing; // ((:?( -> true, false, true
    
    push_back_only_if_vector(groupindx2reason, 1); // First match - match[0] - is the entire match.
    
    push_back_only_if_vector(group_starts); // Skip first entry
    push_back_only_if_vector(group_ends);   // Skip first entry
    push_back_only_if_vector(record_contents); // Skip first entry
    
    trailing_data_size = 1; // Skip the first null byte
    D trl_final = get_final_length_and_trailing_data_size(src, dst, trailing_data_size) + 1; // Location of contents in trl, after trl has been memcpy'd to dst
    D trl = alloc_trailing_data(dst, trailing_data_size); // Location of contents in trl, before it has been memcpy'd
    D trl_orig = trl;
    /*
    Avoid heap allocations by re-using this buffer, by appending to the end of it.
    Everything we might wish to append originates from the src itself, so is guaranteed not to overflow, *so long as dst is at least as large as src*.
    */
    while(*src != 0){
        if (last_chars_were_brckt_qstn_P  &&  *src == '<'){
            push_back_only_if_vector(record_contents,  ((*(src + 1) != '!'))); // Do not record contents of the comments if the capture group name begins with !
            
            dst -= 2; // strlen("?P")
            
            D const group_name = trl_final;
            
            while(*(++src) != '>'){
                if (*src == '\\')
                    ++src;
                write_char(trl++, *src);
                ++trl_final;
                ++trailing_data_size;
            }
            write_char(trl++, 0);
            ++trl_final;
            ++trailing_data_size;
            
            push_back_only_if_vector(groupindx2reason, indexof(reason_name2id, group_name));
            push_back_only_if_vector(group_starts, dst);
            push_back_only_if_vector(group_ends); // NOTE: While I would like to explicitly add 'nullptr' to this, it messes with the type.
            group_is_capturing[group_is_capturing.size()-1] = true; // Overwrite the value set when '(' was being processed.
            
            last_chars_were_brckt_qstn_P = false;
            
            ++src; // Skip '>'

            // last_char_was_backslash = false; // necessary?
            
            continue;
        }
        
        write_char(dst, *src);
        ++dst;
        
        if (!last_char_was_backslash){
            if (*src == '('){
                const bool b = (*(src+1) != '?');
                if (b){
                    push_back_only_if_vector(groupindx2reason, 1); // 1 for Unknown reason matched
                    push_back_only_if_vector(record_contents, true);
                    push_back_only_if_vector(group_starts, dst);
                    push_back_only_if_vector(group_ends); // NOTE: While I would like to explicitly add 'nullptr' to this, it messes with the type.
                }
                group_is_capturing.push_back(b);
            } else if (*src == ')'){
                if (group_is_capturing.back()){
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
    memcpy_trailing_data(dst, trl_orig, trailing_data_size);
    free_trailing_data(trl_orig);
    return dst;
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
    size_t dummy = 0;
    return convert_named_groups(src, dst, dummy, reason_name2id, groupindx2reason, record_contents, group_starts, group_ends);
};

template<typename A>
char* convert_named_groups(char* src,  char* dst,  std::vector<char*>& reason_name2id,  std::vector<A>& groupindx2reason,  std::vector<bool>& record_contents){
    size_t dummy = 0;
    return convert_named_groups(src, dst, dummy, reason_name2id, groupindx2reason, record_contents, dummy, dummy);
};

}
}


#endif
