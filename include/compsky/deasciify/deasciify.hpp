namespace compsky {
namespace deasciify {


template<typename T>
T ascii2n(MYSQL_ROW row,  int col,  T& m){
	T n = 0;
	const char* s = row[col];
	while (*s != 0){
		n *= 10;
		n += *s - '0';
		++s;
	}
	m = n;
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
void assign_next_column(MYSQL_ROW row,  int* col,  char*& s,  Args... args){
	*s = row[(*col)++][0];
	assign_next_column(row,  col,  args...);
};

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  bool*& b,  Args... args){
	*b = !(row[(*col)++][0] == '0');
	assign_next_column(row,  col,  args...);
};

template<typename... Args>
void assign_next_column(MYSQL_ROW row,  int* col,  const char**& s,  Args... args){
	*s = row[(*col)++];
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
void assign_next_column(MYSQL_ROW row,  int* col,  const flag::CopyTo f,  char**& to_ptr,  Args... args){
	char* to = *to_ptr;
	char* from = row[(*col)++];
	while(*from != 0){
		*to = *from;
		++to;
		++from;
	}
	*to_ptr = to;
	assign_next_column(row,  col,  args...);
};

} // namespace deasciify
} // namespace compsky
