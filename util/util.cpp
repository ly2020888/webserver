#include "util.h"
#include <sstream>

std::vector<string> split(const string &text, char delimiter) {
  std::vector<string> tokens;
  std::istringstream stream(text);
  string token;
  while (std::getline(stream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}

std::vector<string> split(const string &text, string delimiter) {
  std::vector<string> tokens;
  auto start_pos = 0;
  auto end_pos = text.find(delimiter);
  while (end_pos != std::string::npos) {
    tokens.push_back(text.substr(start_pos, end_pos - start_pos));
    start_pos = end_pos + delimiter.length();
    end_pos = text.find(delimiter, start_pos);
  }
  std::string lastToken = text.substr(start_pos);
  tokens.push_back(lastToken);
  return tokens;
}