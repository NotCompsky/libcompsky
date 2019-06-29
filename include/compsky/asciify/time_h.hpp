#ifndef __COMPSKY_ASCIIFY_TIME_H__
#define __COMPSKY_ASCIIFY_TIME_H__

namespace compsky {
namespace asciify {

constexpr static const char* day2str[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

template<typename... Args>
void asciify(const tm* t,  Args... args);

}
}

#endif
