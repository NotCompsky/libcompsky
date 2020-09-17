#ifndef LIBCOMPSKY_ASCIIFY_TIME_H_HPP
#define LIBCOMPSKY_ASCIIFY_TIME_H_HPP

namespace compsky {
namespace asciify {

constexpr static const char* day2str[7] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

template<typename... Args>
void asciify(char*& ITR,  const tm* t,  Args... args);

}
}

#endif
