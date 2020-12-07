#pragma once

#include <compsky/asciify/asciify.hpp>
#include <compsky/os/write.hpp>


namespace compsky {
namespace server {

template<typename... Args>
void log(Args... args){
	char buf[4096];
	compsky::os::write::write_to_stderr(buf, args..., '\n');
}

}
}
