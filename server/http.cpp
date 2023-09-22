#include "http.h"
#include "errors.h"
#include <bits/ranges_util.h>
#include <ranges>
#include <regex>
#include <sstream>
#include <vector>
using std::string;
using std::vector;
constexpr const char CRLF = '\n';
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

void HttpParser::parse_request_headers(
    const string &token,
    std::unordered_map<std::string, std::string> &headers) {

  if (token == "") {
    matching_state = CHECK_STATE_CONTENT;
  } else {
    size_t colonPos = token.find(':');
    string key = token.substr(0, colonPos);
    string value = token.substr(colonPos + 1);
    headers[key] = value;
    matching_state = CHECK_STATE_HEADER;
  }
}

void HttpParser::parse_request_content(string &&text, HttpRequest &req) {
  if (req.header.headers["Content-Type"].empty()) {
    req.body.contentType = req.header.headers["Content-Type"];
  } else {
    throw HttpRequestParseException(
        HttpRequestParseException::ErrorCode::InvalidHeader,
        "Unknown HTTP content type.");
  }
  auto body_data = spilt(text, '&');
  for (int i = 0; i < body_data.size(); ++i) {
    size_t colonPos = body_data[i].find('=');
    string key = body_data[i].substr(0, colonPos);
    string value = body_data[i].substr(colonPos + 1);
    req.body.content_kv[key] = value;
  }
  req.body.contentType = std::move(text);
  matching_state = END;
}

HttpRequest HttpParser::parse(string &&metadata) {
  HttpRequest request;
  matching_state = CHECK_STATE_REQUESTLINE;
  auto http_lines = spilt(metadata, CRLF);

  for (int i = 0; i < http_lines.size(); ++i) {
    switch (matching_state) {
    case CHECK_STATE_REQUESTLINE:
      parse_request_line(http_lines[i], request);
      break;
    case CHECK_STATE_HEADER:
      parse_request_headers(http_lines[i], request.header.headers);
      break;
    case CHECK_STATE_CONTENT:
      parse_request_content(std::move(http_lines[i]), request);
      break;
    case END:
      break;
    }
  }
  return request;
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
  matching_state = CHECK_STATE_HEADER;
}
