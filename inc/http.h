#pragma once
#include "logger.h"
#include <nlohmann/json.hpp>
#include <ranges>
#include <string>
#include <unordered_map>
#include <vector>
using std::string;
using std::unordered_map;
using std::vector;
using json = nlohmann::json;
// HTTP方法名
enum METHOD { GET = 0, POST, HEAD, PUT, DELETE, TRACE, OPTIONS, CONNECT, PATH };
// 主状态机状态，检查请求报文中元素
enum CHECK_STATE {
  CHECK_STATE_REQUESTLINE = 0,
  CHECK_STATE_HEADER,
  CHECK_STATE_CONTENT,
  END
};
// HTTP状态码
enum HTTP_CODE {
  NO_REQUEST,
  GET_REQUEST,
  BAD_REQUEST,
  NO_RESOURCE,
  FORBIDDEN_REQUEST,
  FILE_REQUEST,
  INTERNAL_ERROR,
  CLOSED_CONNECTION
};
// 从状态机的状态，文本解析是否成功
enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN };

struct HttpHeader {
  std::string method;      // 请求方法（例如：GET、POST）
  std::string requestUri;  // 请求URI（例如：/index.html）
  std::string httpVersion; // HTTP协议版本（例如：HTTP/1.1）
  std::unordered_map<std::string, std::string> headers; // HTTP头部字段映射
};

struct HttpBody {
  std::string contentType; // 内容类型（例如：text/html）
  std::string content;     // 实际内容
  json content_json;
};

struct HttpRequest {

  HttpHeader header;
  HttpBody body;
};

struct HttpResponse {
  HttpHeader header;         // HTTP响应头部
  HttpBody body;             // HTTP响应正文
  int statusCode;            // HTTP状态码（例如：200表示成功）
  std::string statusMessage; // HTTP状态消息（例如："OK"表示成功）
};

class HttpParser {
public:
  HttpRequest parse(string metadata);
  // 主状态机解析报文中的请求行数据
  void parse_request_line(const string &text, HttpRequest &req);
  // 主状态机解析报文中的请求头数据
  void
  parse_request_headers(const string &text,
                        std::unordered_map<std::string, std::string> &headers);
  // 主状态机解析报文中的请求内容
  void parse_request_content(string &&text, HttpRequest &req);

private:
  void multipart_form_data(const string &text, const string &boundary,
                           HttpRequest &req);
  CHECK_STATE matching_state;
  Logger &logger = Logger::get_instance();
};