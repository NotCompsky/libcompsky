#pragma once


constexpr
void increment(const char** s){
	++(*s);
}
constexpr
void increment(const char*** s){
	++(**s);
}

constexpr
void increment(char** s){
	++(*s);
}
constexpr
void increment(char*** s){
	++(**s);
}
