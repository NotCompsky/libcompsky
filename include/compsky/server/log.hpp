#pragma once

#include <compsky/asciify/asciify.hpp>


namespace compsky {
namespace server {

template<typename... Args>
void log(Args... args){
	char buf[4096];
	compsky::asciify::asciify(buf, args..., '\0');
	fprintf(stderr, "%s\n", buf);
	fflush(stderr);
}

}
}
