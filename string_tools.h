#ifndef REPEAT_STRING_H
#define REPEAT_STRING_H

#include <sstream>
#include <string>
#include <algorithm>

void remove_whitespaces(std::string& str);

void remove_newlines(std::string& str);

bool string_replace(std::string& s, const std::string& _old, const std::string& _new);

std::string repeat_string(int n, const std::string& st);

std::string bool_to_string(bool b);

std::string double_to_string(double d, unsigned int prec);

std::string int_to_string(int i);

bool _in(const std::string& sub, const std::string& s);

#endif
