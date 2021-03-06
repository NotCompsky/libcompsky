/*
Copyright 2020 Adam Gray
This file is part of the tagem program.
The tagem program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by the
Free Software Foundation version 3 of the License.
The tagem program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
This copyright notice should be included in any copy or substantial copy of the tagem source code.
The absense of this copyright notices on some other files in this project does not indicate that those files do not also fall under this license, unless they have a different license written at the top of the file.
*/
#pragma once


#define HEADER__CONTENT_TYPE__JSON "Content-Type: application/json\n"
#define HEADER__CONTENT_TYPE__PNG  "Content-Type: image/png\n"
#define HEADER__CONTENT_TYPE__TEXT "Content-Type: text/plain\n"
#define HEADER__CONTENT_TYPE__ICO  "Content-Type: image/ico\n"
#define HEADER__CONTENT_TYPE__HTML "Content-Type: text/html\n"
#define HEADER__CONTENT_TYPE__CSS  "Content-Type: text/css\n"
#define HEADER__CONTENT_TYPE__JS   "Content-Type: text/javascript\n"


#define HEADER__RETURN_CODE__BAD           "HTTP/1.1 400 Bad Request\n"
#define HEADER__RETURN_CODE__CONTINUE      "HTTP/1.1 100 Continue\n" 
#define HEADER__RETURN_CODE__NOT_FOUND     "HTTP/1.1 404 Not Found\n"
#define HEADER__RETURN_CODE__OK            "HTTP/1.1 200 OK\n"
#define HEADER__RETURN_CODE__SERVER_ERR    "HTTP/1.1 500 Internal Server Error\n"
#define HEADER__RETURN_CODE__UNAUTHORISED  "HTTP/1.1 401 Not Found\n"
#define HEADER__RETURN_CODE__UNPROCESSABLE "HTTP/1.1 422 Unprocessable Entity\n"


namespace compsky {
namespace server {
namespace _r {
	constexpr static const std::string_view not_found =
		HEADER__RETURN_CODE__NOT_FOUND
		HEADER__CONTENT_TYPE__TEXT
		"\n"
		"Not Found"
	;
	
	constexpr static const std::string_view unauthorised =
		HEADER__RETURN_CODE__UNAUTHORISED
		HEADER__CONTENT_TYPE__TEXT
		"\n"
		"Not Authorised"
	;
	
	constexpr static const std::string_view requires_login =
		HEADER__RETURN_CODE__UNAUTHORISED
		HEADER__CONTENT_TYPE__TEXT
		"\n"
		"Requires login"
	;
	
	constexpr static const std::string_view invalid_file = 
		HEADER__RETURN_CODE__NOT_FOUND
		HEADER__CONTENT_TYPE__TEXT
		"\n"
		"File does not exist"
	;
	
	constexpr static const std::string_view server_error =
		HEADER__RETURN_CODE__SERVER_ERR
		HEADER__CONTENT_TYPE__TEXT
		"\n"
		"Server error"
	;
	
	constexpr static const std::string_view not_implemented_yet =
		HEADER__RETURN_CODE__SERVER_ERR
		HEADER__CONTENT_TYPE__TEXT
		"\n"
		"Not implemented yet"
	;
	
	constexpr static const std::string_view post_ok =
		HEADER__RETURN_CODE__OK
		HEADER__CONTENT_TYPE__TEXT
		"\n"
		"OK"
	;
	
	constexpr static const std::string_view post_not_necessarily_malicious_but_invalid =
		HEADER__RETURN_CODE__UNPROCESSABLE
		HEADER__CONTENT_TYPE__TEXT
		"\n"
		"Invalid syntax"
	;
	
	constexpr static const std::string_view expect_100_continue =
		HEADER__RETURN_CODE__CONTINUE
		HEADER__CONTENT_TYPE__TEXT
		"\n"
		"why even bother asking"
	;
	
	/*
	constexpr static const std::string_view bad_request =
		#include "headers/return_code/BAD_REQUEST.c"
		"\n"
		"Bad Request"
	;
	*/
	constexpr static const std::string_view bad_request = not_found;
	
	constexpr static const std::string_view banned_client =
		HEADER__RETURN_CODE__UNAUTHORISED
		// Dirty scoundrels don't even deserve a Content-Type header!!!
		"\n"
		"Your IP address has been temporarily banned"
	;
	
	constexpr static const char* const json_init =
		HEADER__RETURN_CODE__OK
		HEADER__CONTENT_TYPE__JSON
		"\n"
	;
	
	constexpr static const std::string_view EMPTY_JSON_LIST = 
		HEADER__RETURN_CODE__OK // To encourage browsers to cache it.
		HEADER__CONTENT_TYPE__JSON
		"Content-Length: 2\n"
		"\n"
		"[]"
	;
} // namespace _r
}
}
