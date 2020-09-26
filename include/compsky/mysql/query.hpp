#ifndef LIBCOMPSKY_MYSQL_QUERY_HPP
#define LIBCOMPSKY_MYSQL_QUERY_HPP

#include <cstddef> // for size_t
#include <stdint.h> // for uintN_t
#include <string.h> // for strlen

#include <compsky/asciify/asciify.hpp> // for asciify::*
#include <compsky/asciify/flags.hpp> // for compsky::asciify::flag::*

#include <compsky/mysql/flags.hpp>
#include <compsky/mysql/mysql.hpp>


// Assumes mysql/mysql.h, utils.hpp and mymysql.hpp are included in main scope of main program

namespace compsky {
namespace mysql {

template<typename... Args>
void exec(MYSQL* const obj,  char* const buf,  Args... args);

template<typename... Args>
void query(MYSQL* const obj,  MYSQL_RES*& res,  char* const buf,  Args... args);





template<typename T>
T ascii2n(MYSQL_ROW row,  T m);


template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  flag::SizeOfAssigned f,  size_t*& sz,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  uint64_t*& n,  Args... args);
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  int64_t*& n,  Args... args);
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  uint32_t*& n,  Args... args);
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  int32_t*& n,  Args... args);
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  uint16_t*& n,  Args... args);
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  int16_t*& n,  Args... args);
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  uint8_t*& n,  Args... args);
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  int8_t*& n,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  char*& s,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  bool*& s,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  const char** s,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  char**& s,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  float*& d,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  double*& d,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  asciify::flag::guarantee::BetweenZeroAndOneExclusive f,  double*& d,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  asciify::flag::guarantee::BetweenZeroAndOneInclusive f,  double*& d,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  asciify::flag::guarantee::BetweenZeroAndTenLeftInclusive f,  double*& d,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  asciify::flag::StrLen f,  size_t*& sz,  Args... args);

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  const flag::CopyTo f,  char**& to,  Args... args);

template<size_t col_indx>
void assign_next_column(MYSQL_ROW row){};

template<typename... Args>
bool assign_next_row(MYSQL_RES* res,  MYSQL_ROW* row,  Args... args);

template<typename... Args>
bool assign_next_row__no_free(MYSQL_RES* res,  MYSQL_ROW* row,  Args... args);

template<typename Int>
Int n_results(MYSQL_RES* res);



















































template<typename... Args>
void exec(MYSQL* const obj,  char* const buf,  Args... args){
    char* itr = buf;
    asciify::asciify(itr, args...);
    exec_buffer(obj,  buf,  (uintptr_t)itr - (uintptr_t)buf);
};

template<typename... Args>
void query(MYSQL* const obj,  MYSQL_RES*& res,  char* const buf,  Args... args){
    char* itr = buf;
    asciify::asciify(itr, args...);
    query_buffer(obj,  res,  buf,  (uintptr_t)itr - (uintptr_t)buf);
};





template<size_t col_indx,  typename T>
T ascii2n(MYSQL_ROW row,  T m){
    T n = 0;
	const char* s = row[col_indx];
    while (*s != 0){
        n *= 10;
        n += *s - '0';
        ++s;
    }
    return n;
};



template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  flag::SizeOfAssigned f,  size_t*& sz,  Args... args){
    // TODO: Look into using `*sz = row[*col+1] - row[*col];`.
    // Not currently used as I am unsure as to when rows are guaranteed to be contiguous
	*sz = strlen(row[col_indx]);
	assign_next_column<col_indx+1>(row,  args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  uint64_t*& n,  Args... args){
	*n = ascii2n<col_indx>(row, *n);
	assign_next_column<col_indx+1>(row,  args...);
};
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  int64_t*& n,  Args... args){
	*n = ascii2n<col_indx>(row, *n);
	assign_next_column<col_indx+1>(row,  args...);
};
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  uint32_t*& n,  Args... args){
	*n = ascii2n<col_indx>(row, *n);
	assign_next_column<col_indx+1>(row,  args...);
};
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  int32_t*& n,  Args... args){
	*n = ascii2n<col_indx>(row, *n);
	assign_next_column<col_indx+1>(row,  args...);
};
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  uint16_t*& n,  Args... args){
	*n = ascii2n<col_indx>(row, *n);
	assign_next_column<col_indx+1>(row,  args...);
};
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  int16_t*& n,  Args... args){
	*n = ascii2n<col_indx>(row, *n);
	assign_next_column<col_indx+1>(row,  args...);
};
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  uint8_t*& n,  Args... args){
	*n = ascii2n<col_indx>(row, *n);
	assign_next_column<col_indx+1>(row,  args...);
};
template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  int8_t*& n,  Args... args){
	*n = ascii2n<col_indx>(row, *n);
	assign_next_column<col_indx+1>(row,  args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  char*& s,  Args... args){
	*s = row[col_indx][0];
	assign_next_column<col_indx+1>(row,  args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  bool*& b,  Args... args){
    *b = !(row[col_indx][0] == '0');
	assign_next_column<col_indx+1>(row,  args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  const char** s,  Args... args){
	*s = row[col_indx];
	assign_next_column<col_indx+1>(row,  args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  char**& s,  Args... args){
	*s = row[col_indx];
	assign_next_column<col_indx+1>(row,  args...);
};

template<size_t col_indx,  typename T,  typename... Args>
void assign_next_column__floaty(MYSQL_ROW row,  T d,  Args... args){
	char* s = row[col_indx];
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
    
	assign_next_column<col_indx>(row, args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  float*& d,  Args... args){
	assign_next_column__floaty<col_indx>(row, d, args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  double*& d,  Args... args){
	assign_next_column__floaty<col_indx>(row, d, args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  asciify::flag::guarantee::BetweenZeroAndOneExclusive f,  double*& d,  Args... args){
	char* s = row[col_indx];
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
    
	assign_next_column<col_indx+1>(row,  args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  asciify::flag::guarantee::BetweenZeroAndOneInclusive f,  double*& d,  Args... args){
    // Mostly seperated from `BetweenZeroAndTenLeftInclusive` case for clarity - might be confusing to declare some things as between 0 and 10 when they are between 0 and 1.
    // Maybe some performance gain with `*d = (*s == '0') ? 0 : 1;`... but not worth it
    constexpr static const asciify::flag::guarantee::BetweenZeroAndTenLeftInclusive bzatli;
	assign_next_column<col_indx>(row, bzatli, d, args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  asciify::flag::guarantee::BetweenZeroAndTenLeftInclusive f,  double*& d,  Args... args){
	char* s = row[col_indx];
    
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
    
	assign_next_column<col_indx+1>(row,  args...);
};


template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  asciify::flag::StrLen f,  size_t*& sz,  Args... args){
	*sz = strlen(row[col_indx]);
	assign_next_column<col_indx+1>(row,  args...);
};

template<size_t col_indx,  typename... Args>
void assign_next_column(MYSQL_ROW row,  const flag::CopyTo f,  char**& to_ptr,  Args... args){
	char* to = *to_ptr;
	char* from = row[col_indx];
	while(*from != 0){
		*to = *from;
		++to;
		++from;
	}
	*to_ptr = to;
	assign_next_column<col_indx+1>(row,  args...);
};




template<typename... Args>
bool assign_next_row__no_free(MYSQL_RES* res,  MYSQL_ROW* row,  Args... args){
    if (likely((*row = mysql_fetch_row(res)))){
		assign_next_column<0>(*row, args...);
        return true;
    }
    return false;
};

template<typename... Args>
bool assign_next_row(MYSQL_RES* res,  MYSQL_ROW* row,  Args... args){
    if (likely((*row = mysql_fetch_row(res)))){
		assign_next_column<0>(*row, args...);
		return true;
	}
	mysql_free_result(res);
	return false;
};

template<typename Int>
Int n_results(MYSQL_RES* res){
	Int n = 0;
	MYSQL_ROW row;
	while (likely((row = mysql_fetch_row(res))))
		++n;
	mysql_data_seek(res, 0); // Return to start of results set
	return n;
};

}
} // END namespace compsky::mysql








#endif
