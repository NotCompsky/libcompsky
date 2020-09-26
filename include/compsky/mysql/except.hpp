#ifndef LIBCOMPSKY_MYSQL_EXCEPT_HPP
#define LIBCOMPSKY_MYSQL_EXCEPT_HPP

#include <stdexcept>


namespace compsky {
namespace mysql {
namespace except {

struct FileOpen : public std::runtime_error {
	FileOpen(const char* const s)
	: std::runtime_error(std::string("Error opening file: ").append(s))
	{}
};

struct FileRead : public std::runtime_error {
	FileRead(const char* const s)
	: std::runtime_error(std::string("Error reading file: ").append(s))
	{}
};

struct SQLExec : public std::runtime_error {
	SQLExec(const char* const s,  const size_t sz)
	: std::runtime_error(std::string("Error executing SQL statement of length ").append(std::to_string(sz)).append(": ").append(std::string(s, sz)))
	{}
};

struct SQLLibraryInit : public std::runtime_error {
	SQLLibraryInit()
	: std::runtime_error("Failed to initialise SQL library")
	{}
};

struct ConnectToServer : public std::runtime_error {
	ConnectToServer(const char* const user,  const char* const pwrd,  const char* const host,  const unsigned int port_n,  const char* const path)
	: std::runtime_error(std::string("Failed to connect to server at ").append(user).append(":").append(pwrd).append("@").append(host).append(":").append(std::to_string(port_n)).append(path))
	{}
};

struct OOM : public std::runtime_error {
	OOM()
	: std::runtime_error("Out of memory")
	{}
};

struct Nullptr : public std::runtime_error {
	Nullptr()
	: std::runtime_error("Unexcepted nullptr")
	{}
};

} // namespace except
} // namespace mysql
} // namespace compsky

#endif
