#ifndef LIBCOMPSKY_ASCIIFY_CONCAT_HPP
#define LIBCOMPSKY_ASCIIFY_CONCAT_HPP


namespace compsky {
namespace asciify {


template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* s,  const int sz,  T t,  Args... args){
    asciify(t);
    constexpr static const flag::StrLen g;
    asciify(g, s, sz);
    asciify(f, s, sz, args...);
};

template<typename... Args>
void asciify(flag::concat::Start e,  const char* s,  const int sz,  flag::concat::End f,  Args... args){
    // Overrides previous (more general) template
    ITR -= sz;
    asciify(args...);
};

template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* s,  const int sz,  const char** ss,  T n,  Args... args){
    constexpr static const flag::StrLen g;
    for (auto i = 0;  i < n;  ++i){
        asciify(ss[i]);
        asciify(g, s, sz);
    }
    asciify(f, s, sz, args...);
};

#ifdef USE_VECTOR
template<typename SZ,  typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* s,  SZ sz,  const std::vector<const char*>& ss,  T n,  Args... args){
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
    --ITR;
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
void asciify(flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  T t,  Args... args){
    asciify(g, ps, psz, t);
    constexpr static const flag::StrLen strlen;
    asciify(strlen, s, sz);
    asciify(f, s, sz, g, ps, psz, args...);
};

template<typename T,  typename... Args>
void asciify(flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  const char** ss,  T t,  Args... args){
    constexpr static const flag::StrLen strlen;
    for (auto i = 0;  i < t;  ++i){
        asciify(g, ps, psz, ss[i]);
        asciify(strlen, s, sz);
    }
    asciify(f, s, sz, g, ps, psz, args...);
};

template<typename... Args>
void asciify(flag::concat::Start f,  const char* s,  const int sz,  flag::prefix::Start g,  const char* ps,  const size_t psz,  flag::prefix::Start h,  Args... args){
    asciify(f, s, sz, args...);
};


}
}


#endif
