#pragma once


namespace compsky {
namespace utils {


constexpr
bool is_str_dblqt_escaped(const char*& s,  const char terminator1,  const char terminator2 = 0){
	if(*s != '"')
		return false;
	while(*(++s) != '"'){
		const char c = *s;
		if (c == '\\'){
			++s;
			if (*s == 0)
				return false;
		}
	}
	++s;
	return (*s == terminator1) or (*s == terminator2);
}


constexpr
bool is_str_csv_dblqt_escaped(const char*& csv,  const char terminator){
	while(*csv != terminator){
		if (not is_str_dblqt_escaped(csv, ',', '\0'))
			return false;
	}
	return true;
}

}
}
