#ifndef __COMPSKY__ASCIIFY__
#define __COMPSKY__ASCIIFY__

#include "compsky/asciify/asciify_h.hpp"

#include <string.h> // for memcpy
#include <vector>

#ifndef likely
    #ifdef __GNUC__
        #define likely(x)       __builtin_expect(!!(x), 1)
        #define unlikely(x)     __builtin_expect(!!(x), 0)
    #else
        #define likely(x)       (x)
        #define unlikely(x)     (x)
    #endif
#endif


namespace compsky {
namespace asciify {


/* Base Case to Override (must precede Base Cases) */
template<typename... Args>
void asciify(uint64_t t,  Args... args){
    asciify_integer(t);
    return asciify(args...);
};
template<typename... Args>
void asciify(int64_t t,  Args... args){
    asciify_integer(t);
    return asciify(args...);
};
template<typename... Args>
void asciify(uint32_t t,  Args... args){
    asciify_integer(t);
    return asciify(args...);
};
template<typename... Args>
void asciify(int32_t t,  Args... args){
    asciify_integer(t);
    return asciify(args...);
};
template<typename... Args>
void asciify(uint16_t t,  Args... args){
    asciify_integer(t);
    return asciify(args...);
};
template<typename... Args>
void asciify(int16_t t,  Args... args){
    asciify_integer(t);
    return asciify(args...);
};
template<typename... Args>
void asciify(uint8_t t,  Args... args){
    asciify_integer(t);
    return asciify(args...);
};
template<typename... Args>
void asciify(int8_t t,  Args... args){
    asciify_integer(t);
    return asciify(args...);
};
#ifdef _WIN32
template<typename... Args>
void asciify(unsigned long t,  Args... args){
    asciify_integer(t);
    return asciify(args...);
};
#endif

template<typename... Args>
void asciify(const char c,  Args... args){
    asciify(c);
    return asciify(args...);
};

template<typename... Args>
void asciify(const char* __restrict s,  Args... args){
    asciify(s);
    return asciify(args...);
};

template<typename... Args>
void asciify(const char** __restrict ss,  const int n,  Args... args){
    for (auto i = 0;  i < n;  ++i)
        asciify(ss[i]);
    return asciify(args...);
};

#ifdef QT_GUI_LIB
template<typename... Args>
void asciify(const QString& qs,  Args... args){
    const QByteArray ba = qs.toLocal8Bit();
    const char* s = ba.data();
    return asciify(s, args...);
};
#endif

/*
template<typename T,  typename... Args>
void asciify(T t,  Args... args){
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
        asciify_integer(t);
        return asciify(args...);
    }
    if constexpr (
        (std::is_same<T, const char*>::value)
    ){
        asciify(t);
        return asciify(args...);
    }
};
*/


template<typename... Args>
void asciify(flag::StrLen f,  const char* __restrict s,  const size_t sz,  Args... args){
    memcpy(BUF + BUF_INDX,  s,  sz);
    BUF_INDX += sz;
    asciify(args...);
};


/* Base Integer Cases */
template<typename T>
void asciify_integer(T n){
    auto n_digits = count_digits(n);
    auto i = n_digits;
    BUF_INDX += i;
    do {
        BUF[--BUF_INDX] = '0' + (n % 10);
        n /= 10;
    } while (n != 0);
    BUF_INDX += n_digits;
};


/* Initialise Buffer */
template<typename... Args>
void asciify(flag::ChangeBuffer f,  char* buf,  size_t indx,  Args... args){
    BUF = buf;
    BUF_INDX = indx;
    asciify(args...);
};





template<typename T,  typename... Args>
void asciify(flag::FillWithLeadingZeros f,  const int min_digits,  T n,  Args... args){
    int n_digits = count_digits(n);
    for (auto i = n_digits;  i < min_digits;  ++i)
        BUF[BUF_INDX++] = '0';
    asciify(n, args...);
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
void asciify_subzero(double d,  Precision precision){
    auto i = 0;
    do {
        d *= 10;
        char c = d;
        asciify((char)('0' + c));
        d -= c;
        ++i;
    } while (d > 0  &&  i < precision);
};

template<typename T,  typename P,  typename... Args>
void asciify_floaty(T d,  P precision){
    if (d < 0)
        return asciify(-d, precision);
    asciify((uint64_t)d);
    d -= (uint64_t)d;
    asciify('.');
    asciify_subzero(d, precision);
};

template<typename T,  typename... Args>
void asciify(double d,  T precision,  Args... args){
    asciify_floaty(d, precision);
    asciify(args...);
};

template<typename T,  typename... Args>
void asciify(float f,  T precision,  Args... args){
    asciify_floaty(f, precision);
    asciify(args...);
};

template<typename T,  typename... Args>
void asciify(flag::guarantee::BetweenZeroAndOneInclusive f,  double d,  T precision,  Args... args){
    asciify((char)('0' + (char)d),  '.');
    d -= (char)d;
    asciify_subzero(d, precision);
    asciify(args...);
};

template<typename T,  typename... Args>
void asciify(flag::guarantee::BetweenZeroAndOneExclusive f,  double d,  T precision,  Args... args){
    asciify("0.");
    asciify_subzero(d, precision);
    asciify(args...);
};

template<typename... Args>
void asciify(flag::Escape f,  const char c,  const char* __restrict s,  Args... args){
    while(*s != 0){
        if (unlikely(*s == c  ||  *s == '\\'))
            BUF[BUF_INDX++] = '\\';
        BUF[BUF_INDX++] = *s;
        ++s;
    }
    asciify(args...);
};

#ifdef QT_GUI_LIB
template<typename... Args>
void asciify(flag::Escape f,  const char c,  const QString& qs,  Args... args){
    const QByteArray ba = qs.toLocal8Bit();
    const char* s = ba.data();
    asciify(f, c, s, args...);
};
#endif

template<typename... Args>
void asciify(void* ptr,  Args... args){
    BUF[BUF_INDX++] = '0';
    BUF[BUF_INDX++] = 'x';
    uintptr_t n = (uintptr_t)ptr;
    auto n_digits = count_digits(n);
    for (auto buf_indx = BUF_INDX + n_digits;  buf_indx != BUF_INDX;  ){
        const uint8_t m = (n % 16);
        const char c  =  (m < 10)  ?  '0' + m  :  'a' + m - 10;
        BUF[--buf_indx] = c;
        n /= 16;
    }
    BUF_INDX += n_digits;
    asciify(args...);
};




/* Concatenation */
template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* __restrict s,  const int sz,  T t,  Args... args){
    asciify(t);
    constexpr static const flag::StrLen g;
    asciify(g, s, sz);
    asciify(f, s, sz, args...);
};

template<typename... Args>
void asciify(flag::concat::Start e,  const char* __restrict s,  const int sz,  flag::concat::End f,  Args... args){
    // Overrides previous (more general) template
    BUF_INDX -= sz;
    asciify(args...);
};

template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* __restrict s,  const int sz,  const char** __restrict ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(ss[i]);
        asciify(g, s, sz);
    }
    asciify(f, s, sz, args...);
};

#ifdef USE_VECTOR
template<typename SZ,  typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* __restrict s,  SZ sz,  const std::vector<const char*>& ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(ss[i]);
        asciify(g, s, sz);
    }
    asciify(f, s, sz, args...);
};
#endif

template<typename... Args>
void asciify(flag::concat::Start e,  const char c,  flag::concat::End f,  Args... args){
    --BUF_INDX;
    asciify(args...);
};

template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char c,  T t,  Args... args){
    asciify(t);
    asciify(c);
    asciify(f, c, args...);
};

template<typename T,  typename Precision,  typename... Args>
void asciify(flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneInclusive g,  T t,  Precision precision,  Args... args){
    asciify(g, t, precision);
    asciify(c);
    asciify(f, c, args...);
};

template<typename T,  typename Precision,  typename... Args>
void asciify(flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneExclusive g,  T t,  Precision precision,  Args... args){
    asciify(g, t, precision);
    asciify(c);
    asciify(f, c, args...);
};



/* Concatenation with other flag types */
template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* __restrict s,  const int sz,  flag::prefix::Start g,  const char* __restrict ps,  const size_t psz,  T t,  Args... args){
    asciify(g, ps, psz, t);
    constexpr static const flag::StrLen strlen;
    asciify(strlen, s, sz);
    asciify(f, s, sz, g, ps, psz, args...);
};

template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* __restrict s,  const int sz,  flag::prefix::Start g,  const char* __restrict ps,  const size_t psz,  const char** __restrict ss,  T t,  Args... args){
    constexpr static const flag::StrLen strlen;
    for (auto i = 0;  i < t;  ++i){
        asciify(g, ps, psz, ss[i]);
        asciify(strlen, s, sz);
    }
    asciify(f, s, sz, g, ps, psz, args...);
};

template<typename... Args>
void asciify(flag::concat::Start f,  const char* __restrict s,  const int sz,  flag::prefix::Start g,  const char* __restrict ps,  const size_t psz,  flag::prefix::Start h,  Args... args){
    asciify(f, s, sz, args...);
};



/* Prefixes */
template<typename... Args>
void asciify(flag::prefix::Start f,  const char* __restrict s,  const size_t sz,  const char* __restrict ss,  Args... args){
    asciify(f, s, sz, ss);
    asciify(f, s, sz, args...);
};

template<typename T,  typename... Args>
void asciify(flag::prefix::Start f,  const char* __restrict s,  const size_t sz,  const char** __restrict ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(g, s, sz);
        asciify(ss[i]);
    }
    asciify(f, s, sz, args...);
};

template<typename... Args>
void asciify(flag::prefix::Start e,  const char* __restrict s,  const size_t sz,  flag::prefix::End f,  Args... args){
    asciify(args...);
};





/* Convert to/from bases etc */
template<typename Int,  typename... Args>
void asciify(flag::to::AlphaNumeric f,  Int n,  Args... args){
    size_t n_digits = 0;
    
    Int m = n;
    do {
        ++n_digits;
        m /= 36;
    } while (m != 0);
    
    size_t buf_indx = BUF_INDX + n_digits;
    
    do {
        const char digit = n % 36;
        BUF[--buf_indx] = digit + ((digit<10) ? '0' : 'a' - 10);
        n /= 36;
    } while (n != 0);
    
    asciify(args...);
};


}
} // END: namespace compsky::asciify




#ifdef ASCIIFY_TIME
# include "compsky/asciify/time.hpp"
#endif


#endif
