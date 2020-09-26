#pragma once

#include "query.hpp"


namespace compsky {
namespace mysql {


template<size_t col_indx>
void assign_next_column__alternating(MYSQL_ROW){}



template<size_t col_indx,  typename A,  typename B,  typename... Args>
void assign_next_column__alternating(MYSQL_ROW row,  const A,  const B b,  Args... args){
	assign_next_column<col_indx>(row, b);
	assign_next_column__alternating<col_indx+1>(row, args...);
}


template<typename... Args>
bool assign_next_row__no_free__alternating(MYSQL_RES* res,  MYSQL_ROW* row,  Args... args){
	if (likely((*row = mysql_fetch_row(res)))){
		assign_next_column__alternating<0>(*row, args...);
		return true;
	}
	return false;
};

template<typename... Args>
bool assign_next_row__alternating(MYSQL_RES* res,  MYSQL_ROW* row,  Args... args){
	if (likely((*row = mysql_fetch_row(res)))){
		assign_next_column__alternating<0>(*row, args...);
		return true;
	}
	mysql_free_result(res);
	return false;
};

}
}
