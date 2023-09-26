#pragma once
#include <string>
#include <vector>
using std::string;

// 字符串分割
std::vector<string> split(const string &text, char delimiter);
std::vector<string> split(const string &text, string delimiter);