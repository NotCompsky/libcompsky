#ifndef LIBCOMPSKY_ASCIIFY_TIME_HPP
#define LIBCOMPSKY_ASCIIFY_TIME_HPP


#include "time_h.hpp"

namespace compsky {
namespace asciify {

template<typename... Args>
void asciify(char*& ITR,  const tm* t,  Args... args){
    // Default format. Other formats will later be specifiable with flag::datetime:: or something
    asciify(ITR, day2str[t->tm_wday]);
    asciify(ITR, '\t');
    constexpr static const flag::FillWithLeadingZeros f;
    asciify(ITR, 
        1900 + t->tm_year,  '-',
        f, 2, t->tm_mon,  '-',
        f, 2, t->tm_mday,  ' ',
        f, 2, t->tm_hour, ':',
        f, 2, t->tm_min,  ':',
        f, 2, t->tm_sec
    );
};

}
}


#endif
