#include "http.h"
#include "errors.h"
#include <regex>
#include <sstream>
#include <vector>
/*
GET /index.html HTTP/1.1 （Request Line）
Host: www.example.com （Request Headers）
User-Agent: Mozilla/5.0
Accept: text/html
*/
/*

POST /api/users HTTP/1.1
Host: www.example.com
Content-Type: application/json
Content-Length: 45

(CRLF)

{
  "username": "exampleuser",
  "email": "user@example.com"
}

*/
std::vector<string> HttpParser::spilt(const string &text, char delimiter) {
  std::vector<string> tokens;
  std::istringstream stream(text);
  string token;
  while (std::getline(stream, token, delimiter)) {
    tokens.push_back(token);
  }
  return tokens;
}
void HttpParser::parse_request_line(const string &text, HttpRequest &req) {
  std::regex requestLinePattern(R"((\w+) (\S+) (HTTP/\d+\.\d+))");
  std::smatch match;
  if (std::regex_match(text, match, requestLinePattern)) {
    std::string method = match[1];
    std::string requestUri = match[2];
    std::string httpVersion = match[3];
    // http method match
    if (method == "GET") {
      req.header.method = GET;
    } else if (method == "POST") {
      req.header.method = POST;
    } else {
      throw HttpRequestParseException(
          HttpRequestParseException::ErrorCode::InvalidMethod,
          "Invalid request :invalid method.");
    }

    req.header.requestUri = requestUri;

    if (httpVersion == "HTTP/1.1") {
      req.header.httpVersion = 1;
    } else {
      throw HttpRequestParseException(
          HttpRequestParseException::ErrorCode::InvalidVersion,
          "Invalid request InvalidVersion.");
    }
  } else {
    throw HttpRequestParseException(
        HttpRequestParseException::ErrorCode::InvalidVersion,
        "Invalid Http request line.");
  }
}