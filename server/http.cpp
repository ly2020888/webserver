#include "http.h"
#include "errors.h"
#include "util.h"
#include <bits/ranges_util.h>

#include <ranges>
#include <regex>
#include <sstream>
#include <vector>

// for convenience
using std::string;
// using std::vector;
const string CRLF = "\r\n";
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

void HttpParser::parse_request_headers(
    const string &token,
    std::unordered_map<std::string, std::string> &headers) {

  if (token == "") {
    matching_state = CHECK_STATE_CONTENT;
  } else {
    size_t colonPos = token.find(':');
    string key = token.substr(0, colonPos);
    string value = token.substr(colonPos + 2); // 去掉前导空格
    headers[key] = value;
    logger.debugf("parsed key: {} , value: {}", key, value);
    matching_state = CHECK_STATE_HEADER;
  }
}

void HttpParser::multipart_form_data(const string &formData,
                                     const string &boundary, HttpRequest &req) {
  json formDataMap;
  size_t start = formData.find(boundary) + boundary.length();
  size_t end = formData.find(boundary, start);
  while (end != std::string::npos) {
    auto part = formData.substr(start, end - start);
    auto name_start = part.find("name=\"") + 6;
    auto name_end = part.find("\"", name_start);
    auto name = part.substr(name_start, name_end - name_start);
    if (part.find("filename=\"") != std::string::npos) {
      auto file_name_start = part.find("filename=\"") + 10;
      auto file_name_end = part.find("\"", file_name_start);
      auto filename =
          part.substr(file_name_start, file_name_end - file_name_start);

      formDataMap["filename"] = filename;
      size_t dataStart = part.find(CRLF + CRLF) + 4;
      size_t dataEnd = part.find(CRLF, dataStart);

      std::string fieldValue = part.substr(dataStart, dataEnd - dataStart);
      formDataMap[filename] = fieldValue;

    } else {
      // 处理文本字段，提取字段值并存储到map
      size_t dataStart = part.find(CRLF + CRLF) + 4;
      size_t dataEnd = part.find(CRLF, dataStart);

      std::string fieldValue = part.substr(dataStart, dataEnd - dataStart);
      formDataMap[name] = fieldValue;
    }

    start = end + boundary.length();
    end = formData.find(boundary, start);
  }
  req.body.content_json = formDataMap;
}

void HttpParser::parse_request_content(string &&text, HttpRequest &req) {
  if (!req.header.headers["Content-Type"].empty()) {
    req.body.contentType = req.header.headers["Content-Type"];
  } else {
    throw HttpRequestParseException(
        HttpRequestParseException::ErrorCode::InvalidHeader,
        "Unknown HTTP content type.");
  }
  if (req.header.headers["Content-Type"].find(
          "application/x-www-form-urlencoded") !=
      std::string::npos) { // param1=value1&param2=value2
    auto body_data = split(text, '&');
    for (int i = 0; i < body_data.size(); ++i) {
      size_t colonPos = body_data[i].find('=');
      if (colonPos != std::string::npos) {
        string key = body_data[i].substr(0, colonPos);
        string value = body_data[i].substr(colonPos + 1);
        req.body.content_json[key] = value;
      } else {
        throw HttpRequestParseException(
            HttpRequestParseException::ErrorCode::InvalidHeader,
            "An error was encountered while processing the  \
            application/x-www-form-urlencoded type");
      }
    }
    logger.debugf("json is :{}", req.body.content_json.dump());
    req.body.contentType = std::move(text);
  }
  if (req.header.headers["Content-Type"].find("multipart/form-data") !=
      std::string::npos) {
    auto boundary_start =
        req.header.headers["Content-Type"].find("boundary=") + 9;
    auto boundary = req.header.headers["Content-Type"].substr(boundary_start);
    multipart_form_data(text, boundary, req);
    logger.debugf("json is :{}", req.body.content_json.dump());
  }
  if (req.header.headers["Content-Type"].find("application/json") !=
      std::string::npos) {
    req.body.content_json = json::parse(text);
    logger.debugf("json is :{}", req.body.content_json.dump());
  }

  matching_state = END;
}

// parse will clear the buffer
HttpRequest HttpParser::parse(Buffer &metadata) {
  string inner = metadata.as_str();
  return parse(std::move(inner));
}
HttpRequest HttpParser::parse(string metadata) {
  HttpRequest request;
  matching_state = CHECK_STATE_REQUESTLINE;
  size_t bodyStart = metadata.find(CRLF + CRLF);
  std::string header;
  if (bodyStart != std::string::npos) {
    header = metadata.substr(0, bodyStart);
  } else {
    header = std::move(metadata);
  }

  std::string body = "";
  if (bodyStart != std::string::npos) {
    body = metadata.substr(bodyStart + 4); // 4 是空行的长度
  } else {
    body = ""; // 请求体为空
  }

  auto http_lines = split(header, CRLF);

  for (int i = 0; i < http_lines.size(); ++i) {
    switch (matching_state) {
    case CHECK_STATE_REQUESTLINE:
      parse_request_line(http_lines[i], request);
      break;
    case CHECK_STATE_HEADER:
      parse_request_headers(http_lines[i], request.header.headers);
      break;
    case CHECK_STATE_CONTENT:
      break;
    case END:
      break;
    }
  }

  parse_request_content(std::move(body), request);
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
