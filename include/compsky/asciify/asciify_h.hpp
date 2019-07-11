#ifndef LIBCOMPSKY_ASCIIFY_ASCIIFY_H_HPP
#define LIBCOMPSKY_ASCIIFY_ASCIIFY_H_HPP


#include <inttypes.h> // for u?int[0-9]{1,2}_t

#ifdef ASCIIFY_TIME
# include "compsky/asciify/time_h.hpp"
#endif

#ifdef QT_GUI_LIB
# include <QString>
#endif

#include "compsky/asciify/base.hpp"
#include "compsky/asciify/flags.hpp"
#include "compsky/asciify/types.hpp"
#include "compsky/asciify/utils.hpp"


namespace compsky {
namespace asciify {


template<typename... Args>
void asciify(Args... args);

/* Base Case to Override (must precede Base Cases) */
template<typename... Args>
void asciify(uint64_t t,  Args... args);
template<typename... Args>
void asciify(int64_t t,  Args... args);
template<typename... Args>
void asciify(uint32_t t,  Args... args);
template<typename... Args>
void asciify(int32_t t,  Args... args);
template<typename... Args>
void asciify(uint16_t t,  Args... args);
template<typename... Args>
void asciify(int16_t t,  Args... args);
template<typename... Args>
void asciify(uint8_t t,  Args... args);
template<typename... Args>
void asciify(int8_t t,  Args... args);

#ifdef _WIN32
template<typename... Args>
void asciify(unsigned long t,  Args... args);
// unsigned long is a different type than both uin32_t and uint64_t
// Without this, you would get 'ambiguous overloaded function' errors in Visual Studio, as the cast would be equally valid for any integer.
#endif

template<typename... Args>
void asciify(const char c,  Args... args);

template<typename... Args>
void asciify(const char* s,  Args... args);

template<typename... Args>
void asciify(const char** s,  const int n,  Args... args);

#ifdef QT_GUI_LIB
template<typename... Args>
void asciify(const QString& qs,  Args... args);
#endif


template<typename... Args>
void asciify(flag::StrLen f,  const char* s,  const size_t sz,  Args... args);


/* Base Integer Cases */
template<typename T>
void asciify_integer(T n);


/* Initialise Buffer */
template<typename... Args>
void asciify(flag::ResetIndex f,  Args... args);

template<typename... Args>
void asciify(flag::ChangeBuffer f,  char* buf,  Args... args);





template<typename T,  typename... Args>
void asciify(flag::FillWithLeadingZeros f,  const int min_digits,  T n,  Args... args);

template<typename T>
bool operator <(T t,  fake_type::Infinity x);

template<typename T>
bool operator >(fake_type::Infinity x,  T t);

template<typename Precision>
void asciify_subzero(double d,  Precision precision);

template<typename T,  typename P,  typename... Args>
void asciify_floaty(T d,  P precision);

template<typename T,  typename... Args>
void asciify(double d,  T precision,  Args... args);

template<typename T,  typename... Args>
void asciify(float f,  T precision,  Args... args);

template<typename T,  typename... Args>
void asciify(flag::guarantee::BetweenZeroAndOneInclusive f,  double d,  T precision,  Args... args);

template<typename T,  typename... Args>
void asciify(flag::guarantee::BetweenZeroAndOneExclusive f,  double d,  T precision,  Args... args);

template<typename... Args>
void asciify(flag::Escape f,  const char c,  const char* s,  Args... args);

#ifdef QT_GUI_LIB
template<typename... Args>
void asciify(flag::Escape f,  const char c,  const QString& qs,  Args... args);
#endif

template<typename... Args>
void asciify(void* ptr,  Args... args);




/* Concatenation */
template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* s,  const int sz,  T t,  Args... args);

template<typename... Args>
void asciify(flag::concat::Start e,  const char* s,  const int sz,  flag::concat::End f,  Args... args);

template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* s,  const int sz,  const char** ss,  T n,  Args... args);

#ifdef USE_VECTOR
template<typename SZ,  typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* s,  SZ sz,  const std::vector<const char*>& ss,  T n,  Args... args);
#endif

template<typename... Args>
void asciify(flag::concat::Start e,  const char c,  flag::concat::End f,  Args... args);

template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char c,  T t,  Args... args);

template<typename T,  typename Precision,  typename... Args>
void asciify(flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneInclusive g,  T t,  Precision precision,  Args... args);

template<typename T,  typename Precision,  typename... Args>
void asciify(flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneExclusive g,  T t,  Precision precision,  Args... args);


/* Concatenation with other flag types */
template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  T t,  Args... args);

template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  const char** ss,  T t,  Args... args);

template<typename... Args>
void asciify(flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  flag::prefix::End h,  Args... args);



/* Prefixes */
template<typename T,  typename... Args>
void asciify(flag::prefix::Start f,  const char* s,  const size_t sz,  const char** ss,  T n,  Args... args);

template<typename... Args>
void asciify(flag::prefix::Start f,  const char* s,  const size_t sz,  const char* ss,  Args... args);

template<typename... Args>
void asciify(flag::prefix::Start e,  const char* s,  const size_t sz,  flag::prefix::End f,  Args... args);


/* Convert to/from bases etc */
template<typename Int,  typename... Args>
void asciify(flag::to::AlphaNumeric f,  Int n,  Args... args);

}
} // END: namespace compsky::asciify


#endif
