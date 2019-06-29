#include "time_h.hpp"

namespace compsky {
namespace asciify {

template<typename... Args>
void asciify(const tm* t,  Args... args){
    // Default format. Other formats will later be specifiable with flag::datetime:: or something
    asciify(day2str[t->tm_wday]);
    asciify('\t');
    constexpr static const flag::FillWithLeadingZeros f;
    asciify(
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
