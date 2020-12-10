#ifndef LIBCOMPSKY_ASCIIFY_ASCIIFY_HPP
#define LIBCOMPSKY_ASCIIFY_ASCIIFY_HPP

#include "compsky/asciify/asciify_h.hpp"

#include <string.h> // for memcpy
#include <stdexcept> // for std::runtime_error
#include <vector>
#include "compsky/macros/likely.hpp"


namespace compsky {
namespace asciify {


inline
void asciify(char*& ITR){};

/* Getter functions */
template<typename... Args>
size_t get_index(char*& ITR,  char*& BUF){
    return (uintptr_t)ITR - (uintptr_t)BUF;
};

template<typename C>
void append(char*& ITR,  C c){
    *(ITR++) = c;
};

/* Base Case to Override (must precede Base Cases) */
template<typename... Args>
void asciify(char*& ITR,  uint64_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename... Args>
void asciify(char*& ITR,  int64_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename... Args>
void asciify(char*& ITR,  uint32_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename... Args>
void asciify(char*& ITR,  int32_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename... Args>
void asciify(char*& ITR,  uint16_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename... Args>
void asciify(char*& ITR,  int16_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename... Args>
void asciify(char*& ITR,  uint8_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename... Args>
void asciify(char*& ITR,  int8_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
#ifdef _WIN32
template<typename... Args>
void asciify(char*& ITR,  unsigned long t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
#endif

template<unsigned base = 10,  typename Int,  typename... Args>
void asciify(char*& ITR,  const flag::FillWithLeadingZeros,  Int min_n_digits,  const int n,  Args... args){
	Int m = n;
	do {
		if (min_n_digits == 0)
			break;
		--min_n_digits;
	} while (m /= base);
	while(min_n_digits != 0){
		asciify(ITR, '0');
		--min_n_digits;
	}
	asciify_integer<base>(ITR, n);
	asciify(ITR, args...);
}

template<typename... Args>
void asciify(char*& ITR,  const bool b,  Args... args){
	return asciify(ITR,  b ? "True" : "False",  args...);
};

template<typename... Args>
void asciify(char*& ITR,  const char c,  Args... args){
    *(ITR++) = c;
    return asciify(ITR,  args...);
};

/*template<size_t sz,  typename... Args>
void asciify(char*& ITR,  char(&str)[sz],  Args... args){
#warning "Using template<size_t>"
	memcpy(ITR, str, sz);
	ITR += sz;
	asciify(ITR, args...);
}*/

template<typename... Args>
void asciify(char*& ITR,  const char* __restrict s,  Args... args){
    memcpy(ITR, s, strlen(s));
    ITR += strlen(s);
    return asciify(ITR,  args...);
};

template<typename... Args>
void asciify(char*& ITR,  const char** __restrict ss,  const int n,  Args... args){
    for (auto i = 0;  i < n;  ++i)
        asciify(ITR, ss[i]);
    return asciify(ITR,  args...);
};

#ifdef QT_GUI_LIB
template<typename... Args>
void asciify(char*& ITR,  const QString& qs,  Args... args){
    const QByteArray ba = qs.toLocal8Bit();
    const char* s = ba.data();
    return asciify(ITR,  s, args...);
};
#endif

/*
template<typename T,  typename... Args>
void asciify(char*& ITR,  T t,  Args... args){
    if constexpr (
        // I am told: ‘if constexpr’ only available with -std=c++1z or -std=gnu++1z
        // But it *greatly* simplifies the code
        (std::is_same<T, uint64_t>::value) ||
        (std::is_same<T, int64_t>::value) ||
        (std::is_same<T, uint32_t>::value) ||
        (std::is_same<T, int32_t>::value) ||
        (std::is_same<T, uint16_t>::value) ||
        (std::is_same<T, int16_t>::value) ||
        (std::is_same<T, uint8_t>::value) ||
        (std::is_same<T, int8_t>::value)
    ){
        asciify_integer(ITR, t);
        return asciify(ITR,  args...);
    }
    if constexpr (
        (std::is_same<T, const char*>::value)
    ){
        asciify(ITR, t);
        return asciify(ITR,  args...);
    }
};
*/

inline
void asciify_hex(const char map[16],  char*& ITR,  const uint8_t c){
	*(ITR++) = map[((uint8_t)c & 0xf0) >> 4];
	*(ITR++) = map[ (uint8_t)c & 0x0f ];
}

template<size_t sz>
void asciify_hex(const char map[16],  char*& ITR,  const std::array<uint8_t, sz>& str){
	for(size_t i = 0;  i < sz;  ++i){
		asciify_hex(map, ITR, str.at(i));
	}
}

template<typename... Args>
void asciify(char*& ITR,  const flag::Hex,  const uint8_t c,  Args... args){
	unsigned n = 0;
	constexpr static const char map[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	asciify_hex(map, ITR, c);
	asciify(ITR, args...);
}

template<size_t sz,  typename... Args>
void asciify(char*& ITR,  const flag::Hex,  const std::array<uint8_t, sz>& str,  Args... args){
	unsigned n = 0;
	constexpr static const char map[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	asciify_hex(map, ITR, str);
	asciify(ITR, args...);
}

template<size_t sz,  typename... Args>
void asciify(char*& ITR,  const flag::grammatical_case::Lower,  const flag::Hex,  const std::array<uint8_t, sz>& str,  Args... args){
	unsigned n = 0;
	constexpr static const char map[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	asciify_hex(map, ITR, str);
	asciify(ITR, args...);
}


template<typename... Args>
void asciify(char*& ITR,  flag::StrLen f,  const char* __restrict s,  const size_t sz,  Args... args){
	// WARNING: Not sure the __restrict is viable here, probably depends on whether args expansion contains another cstring
    memcpy(ITR,  s,  sz);
    ITR += sz;
    asciify(ITR, args...);
};


/* Base Integer Cases */
template<unsigned base = 10,  typename T>
void asciify_integer(char*& ITR,  T n){
	printf("asciify_integer<%u> %u == %c\n", base, (unsigned)n, (char)n);
	char* const _itr = ITR;
	if (n < 0){
		*(ITR++) = '-';
		return asciify_integer(ITR, -n);
	}
    auto n_digits = count_digits<base>(n);
    auto i = n_digits;
    ITR += i;
    do {
		static_assert(base <= 10 + 26);
		if constexpr(base <= 10){
			*(--ITR) = '0' + (n % base);
		} else if constexpr(base <= 10 + 26){
			const char c = (n % base);
			*(--ITR) = (c <= 10) ? '0' + c : 'a' + c - 10;
		}
		n /= base;
    } while (n != 0);
    ITR += n_digits;
	*ITR = 0;
	printf(">>>%s<<<\n", _itr);
	if (n == 100)
		exit(33);
};




template<typename T>
bool operator <(T t,  fake_type::Infinity x){
    return true;
};

template<typename T>
bool operator >(fake_type::Infinity x,  T t){
    return false;
};

template<typename Precision>
void asciify_subzero(char*& ITR,  double d,  Precision precision){
    auto i = 0;
    do {
        d *= 10;
        char c = d;
        asciify(ITR, (char)('0' + c));
        d -= c;
        ++i;
    } while (d > 0  &&  i < precision);
};

template<typename T,  typename P,  typename... Args>
void asciify_floaty(char*& ITR,  T d,  P precision){
    if (d < 0)
        return asciify(ITR,  -d, precision);
    asciify(ITR, (uint64_t)d);
    d -= (uint64_t)d;
    asciify(ITR, '.');
    asciify_subzero(ITR, d, precision);
};

template<typename T,  typename... Args>
void asciify(char*& ITR,  double d,  T precision,  Args... args){
    asciify_floaty(ITR, d, precision);
    asciify(ITR, args...);
};

template<typename T,  typename... Args>
void asciify(char*& ITR,  float f,  T precision,  Args... args){
    asciify_floaty(ITR, f, precision);
    asciify(ITR, args...);
};

template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::guarantee::BetweenZeroAndOneInclusive f,  double d,  T precision,  Args... args){
    asciify(ITR, (char)('0' + (char)d),  '.');
    d -= (char)d;
    asciify_subzero(ITR, d, precision);
    asciify(ITR, args...);
};

template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::guarantee::BetweenZeroAndOneExclusive f,  double d,  T precision,  Args... args){
    asciify(ITR, "0.");
    asciify_subzero(ITR, d, precision);
    asciify(ITR, args...);
};

template<typename Int,  typename... Args>
void asciify(char*& ITR,  flag::AlphaNumeric f,  Int m,  Args... args){
	size_t n_digits = 0;
	Int n = m;
	do {
		++n_digits;
		n /= 36;
	} while (n != 0);
	ITR += n_digits;
	do {
		char digit = m % 36;
		*(--ITR) = digit + ((digit<10) ? '0' : 'a' - 10);
		m /= 36;
	} while (m != 0);
	ITR += n_digits;
	asciify(ITR, args...);
};

struct NoSuchEnvVar : public std::runtime_error {
	NoSuchEnvVar(const char* const s)
	: std::runtime_error(std::string("No such environmental variable: ").append(s))
	{}
};
struct UnescapedDollar : public std::runtime_error {
	UnescapedDollar()
	: std::runtime_error(std::string("Unescaped dollar sign"))
	{}
};
struct PrematureNullByte : public std::runtime_error {
	PrematureNullByte()
	: std::runtime_error(std::string("No matching end bracket for ${"))
	{}
};
template<typename... Args>
void asciify(char*& ITR,  flag::EnvExpand f,  char* s,  Args... args){
	while(*s != 0){
		if (unlikely(*s == '$')){
			++s;
			if (*s == '{'){
				++s;
				char* const var_name = s;
				while(*s != '}'){
					if (*s == 0)
						throw PrematureNullByte();
					++s;
				}
				*s = 0;
				char* var_value = getenv(var_name);
				if (var_value == nullptr)
					throw NoSuchEnvVar(var_name);
				asciify(ITR, var_value);
				++s;
				continue;
			} else if (*s == '$'){
				;
			} else {
				throw UnescapedDollar();
			}
		}
		*(ITR++) = *s;
		++s;
	}
	asciify(ITR, args...);
};
#ifdef QT_GUI_LIB
template<typename... Args>
void asciify(char*& ITR,  flag::EnvExpand f,  const QString& s,  Args... args){
	for(auto i = 0;  i < s.size();  ){
		if (unlikely(s.at(i) == QChar('$'))){
			++i;
			if (s.at(i) == QChar('{')){
				++i;
				const auto index_of_var_name = i;
				while(s.at(i) != QChar('}')){
					++i;
				}
				const QStringRef qstr_ref(&s,  index_of_var_name,  i - index_of_var_name);
				const QByteArray qba = qstr_ref.toLocal8Bit();
				const char* const var_name = qba.data();
				char* var_value = getenv(var_name);
				if (var_value == nullptr)
					throw NoSuchEnvVar(var_name);
				asciify(ITR, var_value);
				++i;
				continue;
			} else if (s.at(i) == QChar('$')){
				;
			} else {
				throw UnescapedDollar();
			}
		}
		asciify(ITR, s.at(i));
		++i;
	}
	asciify(ITR, args...);
};
#endif

template<typename... Args>
void asciify(char*& ITR,  flag::Repeat,  const char c,  const char* s,  Args... args){
	while(*s != 0){
		if (*s == c)
			*(ITR++) = c;
		*(ITR++) = *s;
		++s;
	}
	asciify(ITR, args...);
}

template<typename... Args>
void asciify(char*& ITR,  const flag::until::NullOrNthChar,  size_t limit,  const char* s,  Args... args){
	while((*s != 0) and (limit-- != 0)){
		*(ITR++) = *s;
		++s;
	}
	asciify(ITR, args...);
}

template<typename... Args>
void asciify(char*& ITR,  const flag::until::NullOr,  const char d,  const char* s,  Args... args){
	while((*s != 0) and (*s != d)){
		*(ITR++) = *s;
		++s;
	}
	asciify(ITR, args...);
}

template<typename... Args>
void asciify(char*& ITR,  const flag::until::NullOr,  const char d,  const std::string_view& v,  Args... args){
	for(const char c : v){
		if (c == d)
			break;
		*(ITR++) = c;
	}
	asciify(ITR, args...);
}

template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const flag::until::NullOr,  const char d,  const char* s,  Args... args){
	while((*s != 0) and (*s != d)){
		if (unlikely(*s == c  ||  *s == '\\'))
			*(ITR++) = '\\';
		*(ITR++) = *s;
		++s;
	}
	asciify(ITR, args...);
}

template<typename... Args>
void asciify(char*& ITR,  flag::Escape f,  const char c,  const char* __restrict s,  Args... args){
    while(*s != 0){
        if (unlikely(*s == c  ||  *s == '\\'))
            *(ITR++) = '\\';
        *(ITR++) = *s;
        ++s;
    }
    asciify(ITR, args...);
};

template<typename... Args>
void asciify(char*& ITR,  const flag::Escape3,  const char c1,  const char c2,  const char c3,  const char* s,  Args... args){
    while(*s != 0){
        if (unlikely(*s == c1  ||  *s == c2  ||  *s == c3  ||  *s == '\\'))
            *(ITR++) = '\\';
        *(ITR++) = *s;
        ++s;
    }
    asciify(ITR, args...);
};

#ifdef LIBCOMPSKY_INCLUDES_STRING_VIEW
template<typename... Args>
void asciify(char*& ITR,  const std::string_view s,  Args... args){
	constexpr flag::StrLen _strlen;
	asciify(ITR, _strlen, s.data(), s.size(), args...);
};

template<typename... Args>
void asciify(char*& ITR,  flag::Escape f,  const char c,  const std::string_view s,  Args... args){
	constexpr flag::StrLen _strlen;
	asciify(ITR, f, c, _strlen, s.size(), s.data(), args...);
};
#endif

template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const flag::StrLen,  const size_t sz,  const char* const s,  Args... args){
	size_t i = 0;
	while(i < sz){
		const char _c = s[i];
		if (unlikely(_c == c  ||  _c == '\\'))
			*(ITR++) = '\\';
		*(ITR++) = _c;
		++i;
    }
    asciify(ITR, args...);
};

#ifdef QT_GUI_LIB
template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const QString& qs,  Args... args){
    const QByteArray ba = qs.toLocal8Bit();
    const char* s = ba.data();
	constexpr static const flag::Escape f; // Why not reuse the parameter? I worry that the compiler might not realise that it is entirely unused if it is referred to by multiple functions.
    asciify(ITR, f, c, s, args...);
};
#endif


template<typename... Args>
void asciify(char*& ITR,  const flag::TerminatedBy,  const char c,  const char* __restrict s,  Args... args){
	while(*s != c){
		*(ITR++) = *s;
		++s;
	}
	asciify(ITR, args...);
};


template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const flag::TerminatedBy,  const char t,  const char* __restrict s,  Args... args){
	while(*s != t){
		if (unlikely(*s == c  ||  *s == '\\'))
			*(ITR++) = '\\';
		*(ITR++) = *s;
		++s;
	}
	asciify(ITR, args...);
};


template<typename... Args>
void asciify(char*& ITR,  const flag::esc::DoubleQuote,  const char c,  Args... args){
	if (c == '"')
		asciify(ITR, '\\');
	asciify(ITR, c);
}


template<typename... Args>
void asciify(char*& ITR,  const flag::esc::DoubleQuote _esc_dblqt,  const flag::esc::URI_until_space::Unescape,  const flag::grammatical_case::Upper,  const char* s,  Args... args){
	while(true){
		char c = *(s++);
		char d;
		bool escaped_parity = false;
		switch(c){
			case 0:
			case ' ':
				if (escaped_parity)
					// Last character is an unescaped escape
					// Ignore for security reasons
					--ITR;
				return asciify(ITR, args...);
			case '%':
				c = *(s++);
				
				if (c == 0)
					// Invalid
					return;
				
				if ((c >= '0') and (c <= '9'))
					c -= '0';
				else
					c += 10 - 'A';
				
				c *= 16;
				
				if (*s == 0)
					return;
				
				d = *s;
				
				if ((d >= '0') and (d <= '9'))
					d -= '0';
				else
					d += 10 - 'A';
				
				c |= d;
				
				++s;
				// No break
			default:
				escaped_parity = ((c == '/') and not escaped_parity);
				asciify(ITR, _esc_dblqt, c);
				break;
		}
	}
}


template<typename... Args>
void asciify(char*& ITR,  const flag::esc::URI_until_space::Unescape,  const flag::grammatical_case::Upper,  const char* s,  Args... args){
	while(true){
		char c = *(s++);
		char d;
		switch(c){
			case 0:
			case ' ':
				return asciify(ITR, args...);
			case '%':
				c = *(s++);
				
				if (c == 0)
					// Invalid
					return;
				
				if ((c >= '0') and (c <= '9'))
					c -= '0';
				else
					c -= 'A';
				
				c *= 16;
				
				if (*s == 0)
					return;
				
				d = *s;
				
				if ((d >= '0') and (d <= '9'))
					d -= '0';
				else
					d -= 'A';
				
				c |= d;
				
				asciify(ITR, c);
				
				++s;
				
				break;
			default:
				asciify(ITR, c);
				break;
		}
	}
	asciify(ITR, args...);
}


template<typename... Args>
void asciify(char*& ITR,  const flag::esc::SpacesAndNonAscii,  const char* s,  Args... args){
	constexpr static const flag::Hex f_hex;
	while(true){
		const char c = *(s++);
		switch(c){
			case 0:
				return asciify(ITR, args...);
			case ' ':
				asciify(ITR, '%', '2', '0');
				break;
			case -127 ... -1:
				asciify(ITR, '%', f_hex, (uint8_t)c);
				break;
			default:
				asciify(ITR, c);
				break;
		}
	}
}


template<typename... Args>
void asciify(char*& ITR,  const flag::JSONEscape,  const char* s,  Args... args){
	while(true){
		const char c = *(s++);
		switch(c){
			case 0:
				return asciify(ITR, args...);
			case '"':
			case '\\':
				asciify(ITR, '\\');
				*(ITR++) = c;
				break;
			case '\n':
				asciify(ITR, '\\', 'n');
				break;
			case '\r':
				asciify(ITR, '\\', 'r');
				break;
			case '\t':
				asciify(ITR, '\\', 't');
				break;
			case '\b':
				asciify(ITR, '\\', 'b');
				break;
			case '\f':
				asciify(ITR, '\\', 'f');
				break;
			// case 0x01 ... 0x1f: // Compiler whines about duplicate cases.
			default:
				if (unlikely((0x01 <= c) and (c <= 0x1f))){
					asciify(ITR, '\\', 'u');
					constexpr flag::FillWithLeadingZeros _f_lpad;
					asciify<16>(ITR, _f_lpad, 4, c);
				} else
					*(ITR++) = c;
				break;
		}
	}
}


template<typename... Args>
void asciify(char*& ITR,  void* ptr,  Args... args){
    *(ITR++) = '0';
    *(ITR++) = 'x';
    uintptr_t n = (uintptr_t)ptr;
    auto n_digits = count_digits(n);
    for (char* itr = ITR + n_digits;  itr != ITR;  ){
        const uint8_t m = (n % 16);
        const char c  =  (m < 10)  ?  '0' + m  :  'a' + m - 10;
        *(--itr) = c;
        n /= 16;
    }
    ITR += n_digits;
    asciify(ITR, args...);
};



template<typename... Args>
void asciify(char*& ITR,  const flag::NElements,  const size_t n,  const std::vector<const char*>& a,  Args... args){
	for (size_t i = 0;  i < n;  ++i){
		asciify(ITR, a.at(i));
	}
	asciify(ITR, args...);
}


template<typename T>
void zip_item_at_index(char*& ITR,  const T& v,  const size_t i){
	asciify(ITR, v);
}
template<typename T>
void zip_item_at_index(char*& ITR,  const std::vector<T>& v,  const size_t i){
	asciify(ITR, v.at(i));
}

template<unsigned N>
struct Zipper {
	template<typename T,  typename... Args>
	static
	void zip(char*& ITR,  const size_t indx,  const T& iterable_or_singleton,  Args... args){
		zip_item_at_index(ITR, iterable_or_singleton, indx);
		Zipper<N-1>::zip(ITR, indx, args...);
	}
};

template<>
struct Zipper<0> {
	template<typename... Args>
	static
	void zip(char*& ITR,  Args... args){}
};

template<unsigned N>
struct Skipper {
	template<typename T,  typename... Args>
	static
	void skip_then_asciify(char*& ITR,  T,  Args... args){
		Skipper<N-1>::skip_then_asciify(ITR, args...);
	}
};

template<>
struct Skipper<0> {
	template<typename... Args>
	static
	void skip_then_asciify(char*& ITR,  Args... args){
		asciify(ITR, args...);
	}
};


template<unsigned N,  typename... Args>
void asciify(char*& ITR,  const flag::Zip<N> f,  const size_t n,  Args... args){
	for (size_t i = 0;  i < n;  ++i){
		Zipper<N>::zip(ITR, i, args...);
	}
	Skipper<N+1>::skip_then_asciify(ITR, n, args...);
}




/* Concatenation */
template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* __restrict s,  const int sz,  T t,  Args... args){
    asciify(ITR, t);
    constexpr static const flag::StrLen g;
    asciify(ITR, g, s, sz);
    asciify(ITR, f, s, sz, args...);
};

template<typename... Args>
void asciify(char*& ITR,  flag::concat::Start e,  const char* __restrict s,  const int sz,  flag::concat::End f,  Args... args){
    // Overrides previous (more general) template
    ITR -= sz;
    asciify(ITR, args...);
};

template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* __restrict s,  const int sz,  const char** __restrict ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(ITR, ss[i]);
        asciify(ITR, g, s, sz);
    }
    asciify(ITR, f, s, sz, args...);
};

template<typename SZ,  typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* __restrict s,  SZ sz,  const std::vector<const char*>& ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(ITR, ss[i]);
        asciify(ITR, g, s, sz);
    }
    asciify(ITR, f, s, sz, args...);
};

template<typename... Args>
void asciify(char*& ITR,  flag::concat::Start e,  const char c,  flag::concat::End f,  Args... args){
    --ITR;
    asciify(ITR, args...);
};

template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char c,  T t,  Args... args){
    asciify(ITR, t);
    asciify(ITR, c);
    asciify(ITR, f, c, args...);
};

template<typename T,  typename Precision,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneInclusive g,  T t,  Precision precision,  Args... args){
    asciify(ITR, g, t, precision);
    asciify(ITR, c);
    asciify(ITR, f, c, args...);
};

template<typename T,  typename Precision,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneExclusive g,  T t,  Precision precision,  Args... args){
    asciify(ITR, g, t, precision);
    asciify(ITR, c);
    asciify(ITR, f, c, args...);
};



/* Concatenation with other flag types */
template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* __restrict s,  const int sz,  flag::prefix::Start g,  const char* __restrict ps,  const size_t psz,  T t,  Args... args){
    asciify(ITR, g, ps, psz, t);
    constexpr static const flag::StrLen strlen;
    asciify(ITR, strlen, s, sz);
    asciify(ITR, f, s, sz, g, ps, psz, args...);
};

template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* __restrict s,  const int sz,  flag::prefix::Start g,  const char* __restrict ps,  const size_t psz,  const char** __restrict ss,  T t,  Args... args){
    constexpr static const flag::StrLen strlen;
    for (auto i = 0;  i < t;  ++i){
        asciify(ITR, g, ps, psz, ss[i]);
        asciify(ITR, strlen, s, sz);
    }
    asciify(ITR, f, s, sz, g, ps, psz, args...);
};

template<typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* __restrict s,  const int sz,  flag::prefix::Start g,  const char* __restrict ps,  const size_t psz,  flag::prefix::Start h,  Args... args){
    asciify(ITR, f, s, sz, args...);
};



/* Prefixes */
template<typename... Args>
void asciify(char*& ITR,  flag::prefix::Start f,  const char* __restrict s,  const size_t sz,  const char* __restrict ss,  Args... args){
    asciify(ITR, f, s, sz, ss);
    asciify(ITR, f, s, sz, args...);
};

template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::prefix::Start f,  const char* __restrict s,  const size_t sz,  const char** __restrict ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(ITR, g, s, sz);
        asciify(ITR, ss[i]);
    }
    asciify(ITR, f, s, sz, args...);
};

template<typename... Args>
void asciify(char*& ITR,  flag::prefix::Start e,  const char* __restrict s,  const size_t sz,  flag::prefix::End f,  Args... args){
    asciify(ITR, args...);
};





/* Convert to/from bases etc */
template<typename Int,  typename... Args>
void asciify(char*& ITR,  flag::to::AlphaNumeric f,  Int n,  Args... args){
    size_t n_digits = 0;
    
    Int m = n;
    do {
        ++n_digits;
        m /= 36;
    } while (m != 0);
    
    ITR += n_digits;
    
    do {
        const char digit = n % 36;
        *(--ITR) = digit + ((digit<10) ? '0' : 'a' - 10);
        n /= 36;
    } while (n != 0);
    
    ITR += n_digits;
    
    asciify(ITR, args...);
};



/* Debug */
template<typename... Args>
void asciify(char*& ITR,  const flag::debug::PrintfStdOut,  Args... args){
	char* const buf = ITR;
	asciify(ITR, args...);
	printf("%s\n", buf);
};



template<size_t sz,  typename... Args>
void asciify(char(& buf)[sz],  Args... args){
	// Don't actually care about the array size (yet) - only used to avoid compiler whining about ambigious overloads.
	char* itr = buf;
	asciify(itr, args...);
}



}
} // END: namespace compsky::asciify




#ifdef ASCIIFY_TIME
# include "compsky/asciify/time.hpp"
#endif


#endif
