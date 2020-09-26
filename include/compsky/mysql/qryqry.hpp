#pragma once

// Cannot think of good non-ambigious name

#include <compsky/utils/streq.hpp>


namespace compsky {
namespace mysql {


template<size_t col_indx>
size_t in_results(const char* const check_for,  MYSQL_RES* const res){
	const char* dummy;
	const char* check_against;
	MYSQL_ROW row;
	
	size_t indx = 0;
	while(likely((row = mysql_fetch_row(res)))){
		assign_next_column<col_indx>(row, &check_against);
		if(utils::streq(check_for, check_against)){
			mysql_data_seek(res, 0); // Return to start of results set
			return indx+1;
		}
		++indx;
	}
	mysql_data_seek(res, 0); // Return to start of results set
	return 0;
}


} // namespace mysql
} // namespace compsky
