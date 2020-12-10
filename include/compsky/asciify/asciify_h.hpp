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

#include "compsky/asciify/flags.hpp"
#include "compsky/asciify/types.hpp"
#include "compsky/asciify/utils.hpp"


#if (defined COMPSKY_STD_STRING_VIEW || defined basic_string)
# include <string_view>
# define LIBCOMPSKY_INCLUDES_STRING_VIEW
#else
# warning "std::string_view treated as not defined"
#endif


namespace compsky {
namespace asciify {


inline
void asciify(char*& ITR);

/* Getter functions */
template<typename... Args>
size_t get_index(char*& ITR,  char*& BUF);

/* Base Case to Override (must precede Base Cases) */
template<typename... Args>
void asciify(char*& ITR,  uint64_t t,  Args... args);
template<typename... Args>
void asciify(char*& ITR,  int64_t t,  Args... args);
template<typename... Args>
void asciify(char*& ITR,  uint32_t t,  Args... args);
template<typename... Args>
void asciify(char*& ITR,  int32_t t,  Args... args);
template<typename... Args>
void asciify(char*& ITR,  uint16_t t,  Args... args);
template<typename... Args>
void asciify(char*& ITR,  int16_t t,  Args... args);
template<typename... Args>
void asciify(char*& ITR,  uint8_t t,  Args... args);
template<typename... Args>
void asciify(char*& ITR,  int8_t t,  Args... args);

#ifdef _WIN32
template<typename... Args>
void asciify(char*& ITR,  unsigned long t,  Args... args);
// unsigned long is a different type than both uin32_t and uint64_t
// Without this, you would get 'ambiguous overloaded function' errors in Visual Studio, as the cast would be equally valid for any integer.
#endif

template<unsigned base = 10,  typename Int,  typename... Args>
void asciify(char*& ITR,  const flag::FillWithLeadingZeros,  Int min_n_digits,  const int n,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const bool b,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const char c,  Args... args);
// We want uintN_t to be translated into human-readable, but char to be pasted as its literal value
// Unfortunately, uint8_t and char are usually the same type, so we cannot differentiate between them

/*template<size_t sz,  typename... Args>
void asciify(char*& ITR,  char(&str)[sz],  Args... args);*/

template<typename... Args>
void asciify(char*& ITR,  const char* s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const char** s,  const int n,  Args... args);

#ifdef QT_GUI_LIB
template<typename... Args>
void asciify(char*& ITR,  const QString& qs,  Args... args);
#endif


template<typename... Args>
void asciify(char*& ITR,  const flag::Hex,  const uint8_t c,  Args... args);

template<size_t sz,  typename... Args>
void asciify(char*& ITR,  const flag::Hex,  const std::array<uint8_t, sz>& str,  Args... args);

template<size_t sz,  typename... Args>
void asciify(char*& ITR,  const flag::grammatical_case::Lower,  const flag::Hex,  const std::array<uint8_t, sz>& str,  Args... args);


template<typename... Args>
void asciify(char*& ITR,  const flag::StrLen,  const char* s,  const size_t sz,  Args... args);


/* Base Integer Cases */
template<unsigned base = 10,  typename T>
void asciify_integer(char*& ITR,  T n);



template<typename T>
bool operator <(T t,  fake_type::Infinity x);

template<typename T>
bool operator >(fake_type::Infinity x,  T t);

template<typename Precision>
void asciify_subzero(char*& ITR,  double d,  Precision precision);

template<typename T,  typename P,  typename... Args>
void asciify_floaty(char*& ITR,  T d,  P precision);

template<typename T,  typename... Args>
void asciify(char*& ITR,  double d,  T precision,  Args... args);

template<typename T,  typename... Args>
void asciify(char*& ITR,  float f,  T precision,  Args... args);

template<typename T,  typename... Args>
void asciify(char*& ITR,  const flag::guarantee::BetweenZeroAndOneInclusive,  double d,  T precision,  Args... args);

template<typename T,  typename... Args>
void asciify(char*& ITR,  const flag::guarantee::BetweenZeroAndOneExclusive,  double d,  T precision,  Args... args);

template<typename Int,  typename... Args>
void asciify(char*& ITR,  const flag::AlphaNumeric,  Int m,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::EnvExpand,  char* s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  flag::Repeat,  const char c,  const char* s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::until::NullOrNthChar,  size_t limit,  const char* s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::until::NullOr,  const char d,  const std::string_view& v,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::until::NullOr,  const char d,  const char* s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const flag::until::NullOr,  const char d,  const char* s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const char* s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::Escape3,  const char c1,  const char c2,  const char c3,  const char* s,  Args... args);

#ifdef LIBCOMPSKY_INCLUDES_STRING_VIEW
template<typename... Args>
void asciify(char*& ITR,  const std::string_view s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const std::string_view s,  Args... args);
#endif

template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const flag::StrLen,  const size_t sz,  const char* const s,  Args... args);

#ifdef QT_GUI_LIB
template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const QString& qs,  Args... args);
#endif

template<typename... Args>
void asciify(char*& ITR,  const flag::TerminatedBy f,  const char c,  const char* __restrict s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  void* ptr,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::Escape,  const char c,  const flag::TerminatedBy g,  const char t,  const char* __restrict s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::esc::DoubleQuote,  const char c,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::esc::DoubleQuote _f_esc_dblqt,  const flag::esc::URI_until_space::Unescape,  const flag::grammatical_case::Upper,  const char* s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::esc::URI_until_space::Unescape,  const flag::grammatical_case::Upper,  const char* s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::esc::SpacesAndNonAscii,  const char* const s,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::JSONEscape,  const char* const s,  Args... args);


template<typename... Args>
void asciify(char*& ITR,  const flag::NElements,  const size_t n,  const std::vector<const char*>& a,  Args... args);


template<unsigned N,  typename... Args>
void asciify(char*& ITR,  const flag::Zip<N> f,  const size_t n,  Args... args);




/* Concatenation */
template<typename T,  typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char* s,  const int sz,  T t,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const flag::concat::End,  Args... args);

template<typename T,  typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const char** ss,  T n,  Args... args);

template<typename SZ,  typename T,  typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char* s,  SZ sz,  const std::vector<const char*>& ss,  T n,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char c,  const flag::concat::End,  Args... args);

template<typename T,  typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char c,  T t,  Args... args);

template<typename T,  typename Precision,  typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char c,  const flag::guarantee::BetweenZeroAndOneInclusive,  T t,  Precision precision,  Args... args);

template<typename T,  typename Precision,  typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char c,  const flag::guarantee::BetweenZeroAndOneExclusive,  T t,  Precision precision,  Args... args);


/* Concatenation with other flag types */
template<typename T,  typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const flag::prefix::Start,  const char* ps,  const size_t psz,  T t,  Args... args);

template<typename T,  typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const flag::prefix::Start,  const char* ps,  const size_t psz,  const char** ss,  T t,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::concat::Start,  const char* s,  const int sz,  const flag::prefix::Start,  const char* ps,  const size_t psz,  const flag::prefix::End,  Args... args);



/* Prefixes */
template<typename T,  typename... Args>
void asciify(char*& ITR,  const flag::prefix::Start,  const char* s,  const size_t sz,  const char** ss,  T n,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::prefix::Start,  const char* s,  const size_t sz,  const char* ss,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  const flag::prefix::Start,  const char* s,  const size_t sz,  const flag::prefix::End,  Args... args);


/* Convert to/from bases etc */
template<typename Int,  typename... Args>
void asciify(char*& ITR,  const flag::to::AlphaNumeric,  Int n,  Args... args);



/* Debug */
template<typename... Args>
void asciify(char*& ITR,  const flag::debug::PrintfStdOut f,  Args... args);


// Copy into a fixed pointer (such as a fixed-length buffer)
template<size_t sz,  typename... Args>
void asciify(char(& buf)[sz],  Args... args);



}
} // END: namespace compsky::asciify


#endif
