#ifndef LIBCOMPSKY_ASCIIFY_ASCIIFY_H_HPP
#define LIBCOMPSKY_ASCIIFY_ASCIIFY_H_HPP


#include <inttypes.h> // for u?int[0-9]{1,2}_t

#ifdef ASCIIFY_TIME
# include "compsky/asciify/time_h.hpp"
#endif

#ifdef QT_GUI_LIB
# include <QString>
#endif

#include <vector>
#include <array>
#include <string_view>

#include "compsky/asciify/flags.hpp"
#include "compsky/asciify/types.hpp"
#include "compsky/asciify/utils.hpp"


namespace compsky {
namespace asciify {
namespace _detail {


template<typename Str>
void asciify(Str& ITR);

/* Base Case to Override (must precede Base Cases) */
template<typename Str,  typename... Args>
void asciify(Str& ITR,  uint64_t t,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  int64_t t,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  uint32_t t,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  int32_t t,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  uint16_t t,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  int16_t t,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  uint8_t t,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  int8_t t,  Args... args);

#ifdef _WIN32
template<typename Str,  typename... Args>
void asciify(Str& ITR,  unsigned long t,  Args... args);
// unsigned long is a different type than both uin32_t and uint64_t
// Without this, you would get 'ambiguous overloaded function' errors in Visual Studio, as the cast would be equally valid for any integer.
#endif

template<unsigned base = 10,  typename Str,  typename Int,  typename... Args>
void asciify(Str& ITR,  const flag::FillWithLeadingZeros,  Int min_n_digits,  const int n,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const bool b,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const char c,  Args... args);
// We want uintN_t to be translated into human-readable, but char to be pasted as its literal value
// Unfortunately, uint8_t and char are usually the same type, so we cannot differentiate between them

/*template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  char(&str)[sz],  Args... args);*/

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const char* s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const char** s,  const int n,  Args... args);

#ifdef QT_GUI_LIB
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const QString& qs,  Args... args);
#endif


template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Hex,  const uint8_t c,  Args... args);

template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::Hex,  const std::array<uint8_t, sz>& str,  Args... args);

template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::grammatical_case::Lower,  const flag::Hex,  const std::array<uint8_t, sz>& str,  Args... args);

template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::esc::Null,  const std::array<uint8_t, sz>& str,  Args... args);

template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c,  const flag::esc::Null f,  const std::array<uint8_t, sz>& str,  Args... args);
template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c,  const char c2,  const flag::esc::Null f,  const std::array<uint8_t, sz>& str,  Args... args);
template<typename Str,  size_t sz,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c,  const char c2,  const char c3,  const flag::esc::Null f,  const std::array<uint8_t, sz>& str,  Args... args);


template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::StrLen,  const char* s,  const size_t sz,  Args... args);


/* Base Integer Cases */
template<unsigned base = 10,  typename Str,  typename T>
void asciify_integer(Str& ITR,  T n);



template<typename T>
bool operator <(T t,  fake_type::Infinity x);

template<typename T>
bool operator >(fake_type::Infinity x,  T t);

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  double d,  T precision,  Args... args);

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  float f,  T precision,  Args... args);

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::guarantee::BetweenZeroAndOneInclusive,  double d,  T precision,  Args... args);

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::guarantee::BetweenZeroAndOneExclusive,  double d,  T precision,  Args... args);

template<typename Str,  typename Int,  typename... Args>
void asciify(Str& ITR,  const flag::AlphaNumeric,  Int m,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::EnvExpand,  char* s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  flag::Repeat,  const char c,  const char* s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::until::NullOrNthChar,  size_t limit,  const char* s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::until::NullOr,  const char d,  const std::string_view& v,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::until::NullOr,  const char d,  const char* s,  Args... args);

template<typename... Chars,  typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const flag::until::NullOr,  const char d,  const char* s,  Args... args);
template<typename... Chars,  typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const flag::until::NullOr,  const char d,  const char* s,  Args... args);
template<typename... Chars,  typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const flag::until::NullOr,  const char d,  const char* s,  Args... args);

template<typename... Chars,  typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char* s,  Args... args);
template<typename... Chars,  typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char* s,  Args... args);
template<typename... Chars,  typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const char* s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const std::string_view s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const std::string_view s,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const std::string_view s,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const std::string_view s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const flag::StrLen,  const size_t sz,  const char* const s,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const flag::StrLen,  const size_t sz,  const char* const s,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const char c1,  const char c2,  const char c3,  const flag::StrLen,  const size_t sz,  const char* const s,  Args... args);

#ifdef QT_GUI_LIB
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const QChar c1,  const QString& s,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const QChar c1,  const QChar c2,  const QString& s,  Args... args);
template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::Escape,  const QChar c1,  const QChar c2,  const QChar c3,  const QString& s,  Args... args);
#endif

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::TerminatedBy f,  const char c,  const char* s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  void* ptr,  Args... args);

template<
	typename Str,  typename... Chars,  typename... Args,
	std::enable_if_t<(std::is_same_v<const char, Chars>&&...), bool> = true
>
void asciify(Str& ITR,  const flag::Escape,  Chars... chars,  const flag::TerminatedBy g,  const char t,  const char* s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::esc::DoubleQuote,  const char c,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::esc::DoubleQuote _f_esc_dblqt,  const flag::esc::URI_until_space::Unescape,  const flag::grammatical_case::Upper,  const char* s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::esc::URI_until_space::Unescape,  const flag::grammatical_case::Upper,  const char* s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::esc::SpacesAndNonAscii,  const char* const s,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::JSONEscape,  const char* const s,  Args... args);


template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::NElements,  const size_t n,  const std::vector<T>& a,  Args... args);


template<typename Str,  unsigned N,  typename... Args>
void asciify(Str& ITR,  const flag::Zip<N> f,  const size_t n,  Args... args);




/* Concatenation */
template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char* s,  const int sz,  T t,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const flag::concat::End,  Args... args);

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const char** ss,  T n,  Args... args);

template<typename Str,  typename SZ,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char* s,  SZ sz,  const std::vector<const char*>& ss,  T n,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char c,  const flag::concat::End,  Args... args);

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char c,  T t,  Args... args);

template<typename Str,  typename T,  typename Precision,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char c,  const flag::guarantee::BetweenZeroAndOneInclusive,  T t,  Precision precision,  Args... args);

template<typename Str,  typename T,  typename Precision,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char c,  const flag::guarantee::BetweenZeroAndOneExclusive,  T t,  Precision precision,  Args... args);


/* Concatenation with other flag types */
template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const flag::prefix::Start,  const char* ps,  const size_t psz,  T t,  Args... args);

template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const flag::prefix::Start,  const char* ps,  const size_t psz,  const char** ss,  T t,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const flag::prefix::Start,  const char* ps,  const size_t psz,  const flag::prefix::End,  Args... args);



/* Prefixes */
template<typename Str,  typename T,  typename... Args>
void asciify(Str& ITR,  const flag::prefix::Start,  const char* s,  const size_t sz,  const char** ss,  T n,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::prefix::Start,  const char* s,  const size_t sz,  const char* ss,  Args... args);

template<typename Str,  typename... Args>
void asciify(Str& ITR,  const flag::prefix::Start,  const char* s,  const size_t sz,  const flag::prefix::End,  Args... args);


/* Convert to/from bases etc */
template<typename Str,  typename Int,  typename... Args>
void asciify(Str& ITR,  const flag::to::AlphaNumeric,  Int n,  Args... args);



} // namespace _detail
}
} // END: namespace compsky::asciify


#endif
