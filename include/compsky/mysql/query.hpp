#ifndef LIBCOMPSKY_MYSQL_QUERY_HPP
#define LIBCOMPSKY_MYSQL_QUERY_HPP

#include <cstddef> // for size_t
#include <stdint.h> // for uintN_t
#include <string.h> // for strlen

#include <compsky/asciify/asciify.hpp> // for asciify::*
#include <compsky/asciify/core.hpp> // for compsky::asciify::BUF_INDX
#include <compsky/asciify/flags.hpp> // for compsky::asciify::flag::*

#include <compsky/mysql/mysql.hpp>

#ifdef DEBUG
  #include <stdio.h> // for printf
#endif


// Assumes mysql/mysql.h, utils.hpp and mymysql.hpp are included in main scope of main program

namespace compsky {
namespace mysql {

template<typename... Args>
void exec(Args... args);

template<typename... Args>
void query(MYSQL_RES** res,  Args... args);





template<typename T>
T ascii2n(MYSQL_ROW row,  int* col,  T m);


template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  flag::SizeOfAssigned f,  size_t*& sz,  Args... args);

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  uint64_t*& n,  Args... args);
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  int64_t*& n,  Args... args);
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  uint32_t*& n,  Args... args);
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  int32_t*& n,  Args... args);
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  uint16_t*& n,  Args... args);
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  int16_t*& n,  Args... args);
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  uint8_t*& n,  Args... args);
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  int8_t*& n,  Args... args);

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  char**& s,  Args... args);

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  float*& d,  Args... args);

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  double*& d,  Args... args);

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  asciify::flag::guarantee::BetweenZeroAndOneInclusive f,  double*& d,  Args... args);

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  asciify::flag::guarantee::BetweenZeroAndTenLeftInclusive f,  double*& d,  Args... args);

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  asciify::flag::StrLen f,  size_t*& d,  Args... args);

template<typename... Args>
bool assign_next_row(MYSQL_RES* res,  MYSQL_ROW* row,  Args... args);

} // END namespace compsky::mysql


















































namespace mysql {

template<typename... Args>
void exec(Args... args){
    constexpr static const asciify::flag::ChangeBuffer change_buffer;
    asciify::asciify(change_buffer, asciify::BUF, 0, args...);
  #ifdef DEBUG
    printf("%s\n", asciify::BUF);
  #endif
    exec_buffer(asciify::BUF, asciify::BUF_INDX);
};

template<typename... Args>
void query(MYSQL_RES** res,  Args... args){
    constexpr static const asciify::flag::ChangeBuffer change_buffer;
    asciify::asciify(change_buffer, asciify::BUF, 0, args...);
    query_buffer(res, asciify::BUF, asciify::BUF_INDX);
};





template<typename T>
T ascii2n(MYSQL_ROW row,  int col,  T m){
    T n = 0;
    char* s = row[col];
    while (*s != 0){
        n *= 10;
        n += *s - '0';
        ++s;
    }
    return n;
};



template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  flag::SizeOfAssigned f,  size_t*& sz,  Args... args){
    // TODO: Look into using `*sz = row[*col+1] - row[*col];`.
    // Not currently used as I am unsure as to when rows are guaranteed to be contiguous
    *sz = strlen(row[*col]);
    return assign_next_column(row,  col,  args...);
};

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  uint64_t*& n,  Args... args){
    *n = ascii2n(row, (*col)++, *n);
    assign_next_column(row,  col,  args...);
};
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  int64_t*& n,  Args... args){
    *n = ascii2n(row, (*col)++, *n);
    assign_next_column(row,  col,  args...);
};
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  uint32_t*& n,  Args... args){
    *n = ascii2n(row, (*col)++, *n);
    assign_next_column(row,  col,  args...);
};
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  int32_t*& n,  Args... args){
    *n = ascii2n(row, (*col)++, *n);
    assign_next_column(row,  col,  args...);
};
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  uint16_t*& n,  Args... args){
    *n = ascii2n(row, (*col)++, *n);
    assign_next_column(row,  col,  args...);
};
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  int16_t*& n,  Args... args){
    *n = ascii2n(row, (*col)++, *n);
    assign_next_column(row,  col,  args...);
};
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  uint8_t*& n,  Args... args){
    *n = ascii2n(row, (*col)++, *n);
    assign_next_column(row,  col,  args...);
};
template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  int8_t*& n,  Args... args){
    *n = ascii2n(row, (*col)++, *n);
    assign_next_column(row,  col,  args...);
};

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  char**& s,  Args... args){
    *s = row[(*col)++];
    assign_next_column(row,  col,  args...);
};

template<typename T,  typename... Args>
void assign_next_column__floaty(MYSQL_ROW row,  int* col,  T d,  Args... args){
    char* s = row[(*col)++];
    *d = *s - '0';
    ++s;
    
    uint64_t dot_encountered_yet = 1;
    uint64_t div_by = 1;
    while(*s != 0){
        if (*s == '.'){
            dot_encountered_yet = 10;
            ++s;
            continue;
        }
        *d *= 10;
        *d += *s - '0';
        div_by *= dot_encountered_yet;
        ++s;
    }
    *d /= (div_by+1);
    
    assign_next_column(row, col, args...);
};

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  float*& d,  Args... args){
    assign_next_column__floaty(row, col, d, args...);
};

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  double*& d,  Args... args){
    assign_next_column__floaty(row, col, d, args...);
};

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  asciify::flag::guarantee::BetweenZeroAndOneExclusive f,  double*& d,  Args... args){
    char* s = row[(*col)++];
    ++s; // Skip the "0"
    
    *d = 0;
    
    if (*s == 0)
        return;
    
    ++s; // Skip the "."
    int n_digits = strlen(s);
    
    for (auto i = n_digits - 1;  i >= 0;  --i){
        *d += s[i] - '0';
        *d /= 10;
    }
    
    assign_next_column(row,  col,  args...);
};

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  asciify::flag::guarantee::BetweenZeroAndOneInclusive f,  double*& d,  Args... args){
    // Mostly seperated from `BetweenZeroAndTenLeftInclusive` case for clarity - might be confusing to declare some things as between 0 and 10 when they are between 0 and 1.
    // Maybe some performance gain with `*d = (*s == '0') ? 0 : 1;`... but not worth it
    constexpr static const asciify::flag::guarantee::BetweenZeroAndTenLeftInclusive bzatli;
    assign_next_column(row, col, bzatli, d, args...);
};

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  asciify::flag::guarantee::BetweenZeroAndTenLeftInclusive f,  double*& d,  Args... args){
    char* s = row[(*col)++];
    
    *d = *s - '0';
    
    ++s;
    
    if (*s == 0)
        return;
    
    ++s; // Skip the "."
    int n_digits = strlen(s);
    
    for (auto i = n_digits - 1;  i >= 0;  --i){
        *d += s[i] - '0';
        *d /= 10;
    }
    
    assign_next_column(row,  col,  args...);
};


template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  asciify::flag::StrLen f,  size_t*& sz,  Args... args){
    *sz = strlen(row[*col]);
    assign_next_column(row,  col,  args...);
};



template<typename... Args>
bool assign_next_row(MYSQL_RES* res,  MYSQL_ROW* row,  Args... args){
    if (likely((*row = mysql_fetch_row(res)))){
        int col = 0;
        assign_next_column(*row, &col, args...);
        return true;
    }
    mysql_free_result(res);
    return false;
};

}
} // END namespace compsky::mysql








#endif
