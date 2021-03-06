#ifndef LIBCOMPSKY_ASCIIFY_CONCAT_H_HPP
#define LIBCOMPSKY_ASCIIFY_CONCAT_H_HPP

namespace compsky {
namespace asciify {


template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* s,  const int sz,  T t,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  flag::concat::Start e,  const char* s,  const int sz,  flag::concat::End f,  Args... args);

template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* s,  const int sz,  const char** ss,  T n,  Args... args);

#ifdef USE_VECTOR
template<typename SZ,  typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* s,  SZ sz,  const std::vector<const char*>& ss,  T n,  Args... args);
#endif

template<typename... Args>
void asciify(char*& ITR,  flag::concat::Start e,  const char c,  flag::concat::End f,  Args... args);

template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char c,  T t,  Args... args);

template<typename T,  typename Precision,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneInclusive g,  T t,  Precision precision,  Args... args);

template<typename T,  typename Precision,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char c,  flag::guarantee::BetweenZeroAndOneExclusive g,  T t,  Precision precision,  Args... args);


/* Concatenation with other flag types */
template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  T t,  Args... args);

template<typename T,  typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  const char** ss,  T t,  Args... args);

template<typename... Args>
void asciify(char*& ITR,  flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  flag::prefix::End h,  Args... args);


}
}

#endif
