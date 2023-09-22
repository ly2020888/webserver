#include "config_factory.h"
#include "errors.h"
#include "http.h"
#include "logger.h"
#include "server.h"
#include "gtest/gtest.h"
#include <memory>
#include <vector>
struct HttpRequestLineTestParam {
  std::string requestLine;
  bool shouldThrowError;
};

struct HttpRequestHeader {
  std::string header;
  bool shouldThrowError;
};

class HttpRequestLineParameterizedTest
    : public ::testing::TestWithParam<HttpRequestLineTestParam> {};

class HttpRequestHeaderTestParam
    : public ::testing::TestWithParam<HttpRequestHeader> {};

vector<HttpRequestLineTestParam> testParams = {
    {"GET /example/index.html HTTP/1.1", false}, // 有效请求行
    {"POST /resource HTTP/1.1", false},

    {"POST /api/data HTTP/1.0", true}, // 无效请求行
    {"Invalid Request Line", true},
    {"PUT /file HTTP/2.0", true},
};

vector<HttpRequestHeader> headers = {
    {R"(User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/94.0.4606.81 Safari/537.36)", // 有效头部
     false},
    {R"(Content-Type: text/html; charset=utf-8)", false},
    {R"(Content-Length: 1234)", false},
    {R"(Accept: text/html, application/xhtml+xml, application/xml;q=0.9, */*;q=0.8)",
     false},
    {R"(Host: www.example.com)", false}};

// 使用INSTANTIATE_TEST_CASE_P宏来注册参数化测试用例并指定参数
INSTANTIATE_TEST_SUITE_P(HttpRequestLineTests, HttpRequestLineParameterizedTest,
                         ::testing::ValuesIn(testParams));

// 在参数化测试用例中测试
TEST_P(HttpRequestLineParameterizedTest, TestHttpRequestLine) {
  const HttpRequestLineTestParam &param = GetParam();
  HttpParser parser = HttpParser();
  HttpRequest req;
  if (param.shouldThrowError) {
    EXPECT_THROW(parser.parse_request_line(param.requestLine, req),
                 HttpRequestParseException);
  } else {
    EXPECT_NO_THROW(parser.parse_request_line(param.requestLine, req));
  }
}
