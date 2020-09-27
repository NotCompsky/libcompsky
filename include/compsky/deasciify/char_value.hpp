#pragma once


constexpr
char char_value(const char* s){
	return *s;
}
constexpr
char char_value(const char** s){
	return **s;
}

constexpr
char char_value(char* s){
	return *s;
}
constexpr
char char_value(char** s){
	return **s;
}
