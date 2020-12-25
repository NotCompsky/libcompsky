#ifndef LIBCOMPSKY_ASCIIFY_ASCIIFY_HPP
#define LIBCOMPSKY_ASCIIFY_ASCIIFY_HPP

#include "compsky/asciify/asciify_h.hpp"

#include <string.h> // for memcpy
#include <stdexcept> // for std::runtime_error
#include <vector>
#include "compsky/macros/likely.hpp"


namespace compsky {
namespace asciify {
namespace _detail {


template<typename Str>
void put(Str ITR,  const char c){
	if constexpr (std::is_same<Str, char*>::value)
		*ITR = c;
}
template<typename Str>
void put(Str ITR,  const char* const str){
	if constexpr (std::is_same<Str, char*>::value)
		memcpy(ITR, str, std::char_traits<char>::length(str));
}
template<typename Str>
void put(Str ITR,  const std::string_view v){
	if constexpr (std::is_same<Str, char*>::value)
		memcpy(ITR, v.data(), v.size());
}

inline
bool is_eq(const char c){
	return false;
}
template<
	typename... Chars,
	std::enable_if_t<(std::is_same_v<const char, Chars>&&...), bool> = true
>
bool is_eq(const char c,  const char c1,  Chars... chars){
	return (c == c1) or is_eq(c, chars...);
}


template<typename Str>
void asciify(Str& ITR){};

/* Getter functions */
template<typename Str,  typename... Args>
size_t get_index(Str const ITR,  Str const BUF){
    return (uintptr_t)ITR - (uintptr_t)BUF;
};

/* Base Case to Override (must precede Base Cases) */
template<typename Str,  typename... Args>
void asciify(Str& ITR,  uint64_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  int64_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  uint32_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  int32_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  uint16_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  int16_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  uint8_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  int8_t t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
#ifdef _WIN32
template<typename Str,  typename... Args>
void asciify(Str& ITR,  unsigned long t,  Args... args){
    asciify_integer(ITR, t);
    return asciify(ITR,  args...);
};
#endif

template<unsigned base = 10,  typename Str,  typename Int,  typename... Args>
void asciify(Str& ITR,  const flag::FillWithLeadingZeros,  Int min_n_digits,  const int n,  Args... args){
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

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const bool b,  Args... args){
	return asciify(ITR,  b ? "True" : "False",  args...);
};

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const char c,  Args... args){
	_detail::put(ITR++, c);
    return asciify(ITR,  args...);
};

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const char* s,  Args... args){
	_detail::put(ITR, s);
    ITR += strlen(s);
    return asciify(ITR,  args...);
};

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const char** ss,  const int n,  Args... args){
    for (auto i = 0;  i < n;  ++i)
        asciify(ITR, ss[i]);
    return asciify(ITR,  args...);
};

#ifdef QT_GUI_LIB
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const QString& qs,  Args... args){
    const QByteArray ba = qs.toLocal8Bit();
    const char* s = ba.data();
    return asciify(ITR,  s, args...);
};
#endif

template<typename Str>
void asciify_hex(const char map[16],  Str& ITR,  const uint8_t c){
	_detail::put(ITR++, map[((uint8_t)c & 0xf0) >> 4]);
	_detail::put(ITR++, map[ (uint8_t)c & 0x0f ]);
}

template<typename Str,  size_t sz>
void asciify_hex(const char map[16],  Str& ITR,  const std::array<uint8_t, sz>& str){
	for(size_t i = 0;  i < sz;  ++i){
		asciify_hex(map, ITR, str.at(i));
	}
}

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Hex,  const uint8_t c,  Args... args){
	unsigned n = 0;
	constexpr static const char map[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	asciify_hex(map, ITR, c);
	asciify(ITR, args...);
}

template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::Hex,  const std::array<uint8_t, sz>& str,  Args... args){
	unsigned n = 0;
	constexpr static const char map[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	asciify_hex(map, ITR, str);
	asciify(ITR, args...);
}

template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::grammatical_case::Lower,  const flag::Hex,  const std::array<uint8_t, sz>& str,  Args... args){
	unsigned n = 0;
	constexpr static const char map[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
	asciify_hex(map, ITR, str);
	asciify(ITR, args...);
}

template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::esc::Null,  const std::array<uint8_t, sz>& str,  Args... args){
	for (const uint8_t c : str){
		if (c == 0)
			asciify(ITR, '\\', '0');
		else if (c == '\\')
			asciify(ITR, '\\', '\\');
		else
			asciify(ITR, c);
	}
}

template<typename... Chars,  typename Str,  size_t sz,  typename... Args>
void asciify_escape(Str& ITR,  Chars... chars,  const flag::esc::Null,  const std::array<uint8_t, sz>& str,  Args... args){
	for (const uint8_t c : str){
		if (c == 0)
			asciify(ITR, '\\', '0');
		else if (_detail::is_eq(c, '\\', chars...))
			asciify(ITR, '\\', c);
		else
			asciify(ITR, c);
	}
}
template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const flag::esc::Null f,  const std::array<uint8_t, sz>& str,  Args... args){
	asciify_escape<char>(ITR, c1, f, str, args...);
}
template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const flag::esc::Null f,  const std::array<uint8_t, sz>& str,  Args... args){
	asciify_escape<char, char>(ITR, c1, c2, f, str, args...);
}
template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const flag::esc::Null f,  const std::array<uint8_t, sz>& str,  Args... args){
	asciify_escape<char, char>(ITR, c1, c2, c3, f, str, args...);
}


template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::StrLen f,  const char* s,  const size_t sz,  Args... args){
	// WARNING: Not sure the is viable here, probably depends on whether args expansion contains another cstring
	_detail::put(ITR, std::string_view(s, sz));
    ITR += sz;
    asciify(ITR, args...);
};


/* Base Integer Cases */
template<unsigned base = 10,  typename Str,  typename T>
void asciify_integer(Str& ITR,  T n){
	Str const _itr = ITR;
	if (n < 0){
		_detail::put(ITR++, '-');
		return asciify_integer(ITR, -n);
	}
    auto n_digits = count_digits<base>(n);
    auto i = n_digits;
    ITR += i;
    do {
		static_assert(base <= 10 + 26);
		if constexpr(base <= 10){
			_detail::put(--ITR,  '0' + (n % base));
		} else if constexpr(base <= 10 + 26){
			const char c = (n % base);
			_detail::put(--ITR,  (c <= 10) ? '0' + c : 'a' + c - 10);
		}
		n /= base;
    } while (n != 0);
    ITR += n_digits;
};




template<typename T>
bool operator <(T t,  fake_type::Infinity x){
    return true;
};

template<typename T>
bool operator >(fake_type::Infinity x,  T t){
    return false;
};

template<typename Str,  typename Precision>
void asciify_subzero(Str& ITR,  double d,  Precision precision){
    auto i = 0;
    do {
        d *= 10;
        char c = d;
        asciify(ITR, (char)('0' + c));
        d -= c;
        ++i;
    } while (d > 0  &&  i < precision);
};

template<typename Str,  typename T,  typename P,  typename... Args>
void asciify_floaty(Str& ITR,  T d,  P precision){
    if (d < 0)
        return asciify(ITR,  -d, precision);
    asciify(ITR, (uint64_t)d);
    d -= (uint64_t)d;
    asciify(ITR, '.');
    asciify_subzero(ITR, d, precision);
};

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  double d,  T precision,  Args... args){
    asciify_floaty(ITR, d, precision);
    asciify(ITR, args...);
};

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  float f,  T precision,  Args... args){
    asciify_floaty(ITR, f, precision);
    asciify(ITR, args...);
};

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  flag::guarantee::BetweenZeroAndOneInclusive f,  double d,  T precision,  Args... args){
    asciify(ITR, (char)('0' + (char)d),  '.');
    d -= (char)d;
    asciify_subzero(ITR, d, precision);
    asciify(ITR, args...);
};

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  flag::guarantee::BetweenZeroAndOneExclusive f,  double d,  T precision,  Args... args){
    asciify(ITR, "0.");
    asciify_subzero(ITR, d, precision);
    asciify(ITR, args...);
};

template<typename Str,  typename Int,  typename... Args>
void asciify(Str& ITR,  flag::AlphaNumeric f,  Int m,  Args... args){
	size_t n_digits = 0;
	Int n = m;
	do {
		++n_digits;
		n /= 36;
	} while (n != 0);
	ITR += n_digits;
	do {
		char digit = m % 36;
		_detail::put(--ITR,  digit + ((digit<10) ? '0' : 'a' - 10));
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
template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::EnvExpand f,  char* s,  Args... args){
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
		_detail::put(ITR++, *s);
		++s;
	}
	asciify(ITR, args...);
};
#ifdef QT_GUI_LIB
template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::EnvExpand f,  const QString& s,  Args... args){
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

template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::Repeat,  const char c,  const char* s,  Args... args){
	while(*s != 0){
		if (*s == c)
			_detail::put(ITR++, c);
		_detail::put(ITR++, *s);
		++s;
	}
	asciify(ITR, args...);
}

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::until::NullOrNthChar,  size_t limit,  const char* s,  Args... args){
	while((*s != 0) and (limit-- != 0)){
		_detail::put(ITR++, *s);
		++s;
	}
	asciify(ITR, args...);
}

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::until::NullOr,  const char d,  const char* s,  Args... args){
	while((*s != 0) and (*s != d)){
		_detail::put(ITR++, *s);
		++s;
	}
	asciify(ITR, args...);
}

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::until::NullOr,  const char d,  const std::string_view& v,  Args... args){
	for(const char c : v){
		if (c == d)
			break;
		_detail::put(ITR++, c);
	}
	asciify(ITR, args...);
}

template<typename... Chars,  typename Str,  typename... Args>
void asciify_escape(Str& ITR,  Chars... chars,  const flag::until::NullOr,  const char d,  const char* s,  Args... args){
	while((*s != 0) and (*s != d)){
		if (unlikely(_detail::is_eq(*s, chars...)))
			_detail::put(ITR++, '\\');
		_detail::put(ITR++, *s);
		++s;
	}
	asciify(ITR, args...);
}
template<typename... Chars,  typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const flag::until::NullOr f,  const char d,  const char* s,  Args... args){
	asciify_escape<char>(ITR, c1, f, d, s, args...);
}
template<typename... Chars,  typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const flag::until::NullOr f,  const char d,  const char* s,  Args... args){
	asciify_escape<char, char>(ITR, c1, c2, f, d, s, args...);
}
template<typename... Chars,  typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const flag::until::NullOr f,  const char d,  const char* s,  Args... args){
	asciify_escape<char, char, char>(ITR, c1, c2, c3, f, d, s, args...);
}

template<typename... Chars,  typename Str,  typename... Args>
void asciify_escape(Str& ITR,  Chars... chars,  const char* s,  Args... args){
    while(*s != 0){
		if (unlikely(_detail::is_eq(*s, '\\', chars...)))
            _detail::put(ITR++, '\\');
        _detail::put(ITR++, *s);
        ++s;
    }
    asciify(ITR, args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char* s,  Args... args){
	asciify_escape<char>(ITR, c1, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char* s,  Args... args){
	asciify_escape<char, char>(ITR, c1, c2, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const char* s,  Args... args){
	asciify_escape<char, char, char>(ITR, c1, c2, c3, s, args...);
}

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const std::string_view s,  Args... args){
	constexpr flag::StrLen _strlen;
	asciify(ITR, _strlen, s.data(), s.size(), args...);
};

template<typename... Chars,  typename Str,  typename... Args>
void asciify_escape(Str& ITR,  Chars... chars,  const std::string_view s,  Args... args){
	constexpr flag::StrLen _strlen;
	asciify_escape(ITR, chars..., _strlen, s.size(), s.data(), args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const std::string_view s,  Args... args){
	asciify_escape<char>(ITR, c1, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const std::string_view s,  Args... args){
	asciify_escape<char, char>(ITR, c1, c2, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const std::string_view s,  Args... args){
	asciify_escape<char, char, char>(ITR, c1, c2, c3, s, args...);
}

template<typename... Chars,  typename Str,  typename... Args>
void asciify_escape(Str& ITR,  Chars... chars,  const flag::StrLen,  const size_t sz,  const char* const s,  Args... args){
	size_t i = 0;
	while(i < sz){
		const char _c = s[i];
		if (unlikely(_detail::is_eq(_c, '\\', chars...)))
			_detail::put(ITR++, '\\');
		_detail::put(ITR++, _c);
		++i;
    }
    asciify(ITR, args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const flag::StrLen f,  const size_t sz,  const char* const s,  Args... args){
	asciify_escape<char>(ITR, c1, f, sz, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const flag::StrLen f,  const size_t sz,  const char* const s,  Args... args){
	asciify_escape<char, char>(ITR, c1, c2, f, sz, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const flag::StrLen f,  const size_t sz,  const char* const s,  Args... args){
	asciify_escape<char, char, char>(ITR, c1, c2, c3, f, sz, s, args...);
}

#ifdef QT_GUI_LIB
template<typename... Chars,  typename Str,  typename... Args>
void asciify_escape(Str& ITR,  Chars... chars,  const QString& qs,  Args... args){
	size_t i = 0;
	while(i < sz){
		const QChar _c = qs.at(i);
		if (unlikely(_detail::is_eq(_c, QChar('\\'), chars...)))
			_detail::put(ITR++, '\\');
		_detail::put(ITR++, _c);
		++i;
	}
	asciify(ITR, args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const QChar c1,  const QString& s,  Args... args){
	asciify_escape<QChar>(ITR, c1, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const QChar c1,  const QChar c2,  const QString& s,  Args... args){
	asciify_escape<QChar, QChar>(ITR, c1, c2, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const QChar c1,  const QChar c2,  const QChar c3,  const QString& s,  Args... args){
	asciify_escape<QChar, QChar, QChar>(ITR, c1, c2, c3, s, args...);
}
#endif


template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::TerminatedBy,  const char c,  const char* s,  Args... args){
	while(*s != c){
		_detail::put(ITR++, *s);
		++s;
	}
	asciify(ITR, args...);
};


template<typename... Chars,  typename Str,  typename... Args>
void asciify_escape(Str& ITR,  Chars... chars,  const flag::TerminatedBy,  const char t,  const char* s,  Args... args){
	while(*s != t){
		if (unlikely(_detail::is_eq(*s, '\\', chars...)))
			_detail::put(ITR++, '\\');
		_detail::put(ITR++, *s);
		++s;
	}
	asciify(ITR, args...);
};
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const flag::TerminatedBy f,  const char t,  const char* s,  Args... args){
	asciify_escape<char>(ITR, c1, f, t, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const flag::TerminatedBy f,  const char t,  const char* s,  Args... args){
	asciify_escape<char, char>(ITR, c1, c2, f, t, s, args...);
}
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const flag::TerminatedBy f,  const char t,  const char* s,  Args... args){
	asciify_escape<char, char, char>(ITR, c1, c2, c3, f, t, s, args...);
}


template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::esc::DoubleQuote,  const char c,  Args... args){
	if (c == '"')
		asciify(ITR, '\\');
	asciify(ITR, c);
}


template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::esc::DoubleQuote _esc_dblqt,  const flag::esc::URI_until_space::Unescape,  const flag::grammatical_case::Upper,  const char* s,  Args... args){
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


template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::esc::URI_until_space::Unescape,  const flag::grammatical_case::Upper,  const char* s,  Args... args){
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


template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::esc::SpacesAndNonAscii,  const char* s,  Args... args){
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


template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::JSONEscape,  const char* s,  Args... args){
	while(true){
		const char c = *(s++);
		switch(c){
			case 0:
				return asciify(ITR, args...);
			case '"':
			case '\\':
				asciify(ITR, '\\');
				_detail::put(ITR++, c);
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
					_detail::put(ITR++, c);
				break;
		}
	}
}


template<typename Str,  typename... Args>
void asciify(Str& ITR,  void* ptr,  Args... args){
	_detail::put(ITR++, '0');
	_detail::put(ITR++, 'x');
    uintptr_t n = (uintptr_t)ptr;
    auto n_digits = count_digits(n);
    for (Str itr = ITR + n_digits;  itr != ITR;  ){
        const uint8_t m = (n % 16);
        const char c  =  (m < 10)  ?  '0' + m  :  'a' + m - 10;
		_detail::put(--itr, c);
        n /= 16;
    }
    ITR += n_digits;
    asciify(ITR, args...);
};



template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::NElements,  const size_t n,  const std::vector<T>& a,  Args... args){
	for (size_t i = 0;  i < n;  ++i){
		asciify(ITR, a.at(i));
	}
	asciify(ITR, args...);
}


template<typename Str,  typename T>
void zip_item_at_index(Str& ITR,  const T& v,  const size_t i){
	asciify(ITR, v);
}
template<typename Str,  typename T>
void zip_item_at_index(Str& ITR,  const std::vector<T>& v,  const size_t i){
	asciify(ITR, v.at(i));
}

template<unsigned N>
struct Zipper {
	template<typename Str,  typename T,  typename... Args>
	static
	void zip(Str& ITR,  const size_t indx,  const T& iterable_or_singleton,  Args... args){
		zip_item_at_index(ITR, iterable_or_singleton, indx);
		Zipper<N-1>::zip(ITR, indx, args...);
	}
};

template<>
struct Zipper<0> {
	template<typename Str,  typename... Args>
	static
	void zip(Str& ITR,  Args... args){}
};

template<unsigned N>
struct Skipper {
	template<typename Str,  typename T,  typename... Args>
	static
	void skip_then_asciify(Str& ITR,  T,  Args... args){
		Skipper<N-1>::skip_then_asciify(ITR, args...);
	}
};

template<>
struct Skipper<0> {
	template<typename Str,  typename... Args>
	static
	void skip_then_asciify(Str& ITR,  Args... args){
		asciify(ITR, args...);
	}
};


template<typename Str,  unsigned N,  typename... Args>
void asciify(Str& ITR,  const flag::Zip<N> f,  const size_t n,  Args... args){
	for (size_t i = 0;  i < n;  ++i){
		Zipper<N>::zip(ITR, i, args...);
	}
	Skipper<N+1>::skip_then_asciify(ITR, n, args...);
}




/* Concatenation */
template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start f,  const char* s,  const int sz,  T t,  Args... args){
    asciify(ITR, t);
    constexpr static const flag::StrLen g;
    asciify(ITR, g, s, sz);
    asciify(ITR, f, s, sz, args...);
};

template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start e,  const char* s,  const int sz,  flag::concat::End f,  Args... args){
    // Overrides previous (more general) template
    ITR -= sz;
    asciify(ITR, args...);
};

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start f,  const char* s,  const int sz,  const char** ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(ITR, ss[i]);
        asciify(ITR, g, s, sz);
    }
    asciify(ITR, f, s, sz, args...);
};

template<typename Str,  typename SZ,  typename T,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start f,  const char* s,  SZ sz,  const std::vector<const char*>& ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(ITR, ss[i]);
        asciify(ITR, g, s, sz);
    }
    asciify(ITR, f, s, sz, args...);
};

template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start e,  const char c,  flag::concat::End f,  Args... args){
    --ITR;
    asciify(ITR, args...);
};

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start f,  const char c,  T t,  Args... args){
    asciify(ITR, t);
    asciify(ITR, c);
    asciify(ITR, f, c, args...);
};

template<typename Str,  typename T,  typename Precision,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneInclusive g,  T t,  Precision precision,  Args... args){
    asciify(ITR, g, t, precision);
    asciify(ITR, c);
    asciify(ITR, f, c, args...);
};

template<typename Str,  typename T,  typename Precision,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneExclusive g,  T t,  Precision precision,  Args... args){
    asciify(ITR, g, t, precision);
    asciify(ITR, c);
    asciify(ITR, f, c, args...);
};



/* Concatenation with other flag types */
template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  T t,  Args... args){
    asciify(ITR, g, ps, psz, t);
    constexpr static const flag::StrLen strlen;
    asciify(ITR, strlen, s, sz);
    asciify(ITR, f, s, sz, g, ps, psz, args...);
};

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  const char** ss,  T t,  Args... args){
    constexpr static const flag::StrLen strlen;
    for (auto i = 0;  i < t;  ++i){
        asciify(ITR, g, ps, psz, ss[i]);
        asciify(ITR, strlen, s, sz);
    }
    asciify(ITR, f, s, sz, g, ps, psz, args...);
};

template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  flag::prefix::Start h,  Args... args){
    asciify(ITR, f, s, sz, args...);
};



/* Prefixes */
template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::prefix::Start f,  const char* s,  const size_t sz,  const char* ss,  Args... args){
    asciify(ITR, f, s, sz, ss);
    asciify(ITR, f, s, sz, args...);
};

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  flag::prefix::Start f,  const char* s,  const size_t sz,  const char** ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(ITR, g, s, sz);
        asciify(ITR, ss[i]);
    }
    asciify(ITR, f, s, sz, args...);
};

template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::prefix::Start e,  const char* s,  const size_t sz,  flag::prefix::End f,  Args... args){
    asciify(ITR, args...);
};





/* Convert to/from bases etc */
template<typename Str,  typename Int,  typename... Args>
void asciify(Str& ITR,  flag::to::AlphaNumeric f,  Int n,  Args... args){
    size_t n_digits = 0;
    
    Int m = n;
    do {
        ++n_digits;
        m /= 36;
    } while (m != 0);
    
    ITR += n_digits;
    
    do {
        const char digit = n % 36;
		_detail::put(--ITR,  digit + ((digit<10) ? '0' : 'a' - 10));
        n /= 36;
    } while (n != 0);
    
    ITR += n_digits;
    
    asciify(ITR, args...);
};


} // namespace _detail



template<typename... Args>
void asciify(char*& buf,  Args... args){
	// Pass pointer by reference in order to avoid "ambiguous overload" with an array (because if both an overload of an array and of a pointer exists, C++ has absolutely no clue which overload we could possibly be expecting to use)
	_detail::asciify(buf, args...);
}


template<size_t sz,  typename... Args>
void asciify(char(& buf)[sz],  Args... args){
	// Don't actually care about the array size (yet) - only used to avoid compiler whining about ambigious overloads.
	char* itr = buf;
	_detail::asciify(itr, args...);
}


template<typename... Args>
std::size_t asciify_strlen(Args... args){
	// Dry run asciify - get the length of the string that would be written by asciify, without writing
	const char* const buf = 0;
	const char* itr = buf;
	_detail::asciify(itr, args...);
	return (uintptr_t)itr;
}



}
} // END: namespace compsky::asciify




#ifdef ASCIIFY_TIME
# include "compsky/asciify/time.hpp"
#endif


#endif
