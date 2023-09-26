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

struct HttpTestData {
  std::string httpRequest;
};

class HttpRequestLineParameterizedTest
    : public ::testing::TestWithParam<HttpRequestLineTestParam> {};

class HttpRequestHeaderTestParam
    : public ::testing::TestWithParam<HttpRequestHeader> {};

class HttpContentTypeTest : public testing::TestWithParam<HttpTestData> {};

vector<HttpTestData> full_data = {
    {"POST /api/submit-form HTTP/1.1\r\n"
     "Host: example.com\r\n"
     "User-Agent: MyHttpClient\r\n"
     "Content-Type: application/x-www-form-urlencoded\r\n"
     "Content-Length: 29\r\n"
     "\r\n"
     "name=John+Doe&age=30&city=Anytown"},
    {"GET /api/update HTTP/1.1\r\n"
     "Host: example.com\r\n"
     "User-Agent: MyHttpClient\r\n"
     "Authorization: Bearer YourAccessToken\r\n"
     "Content-Type: application/json\r\n"
     "Content-Length: 68\r\n"
     "\r\n" // 空行分隔请求头和请求体
     "{\n"
     "    \"id\": 123,\n"
     "    \"name\": \"Updated Name\",\n"
     "    \"description\": \"Updated Description\"\n"
     "}"},
    {"POST /upload HTTP/1.1\r\n"
     "Host: example.com\r\n"
     "User-Agent: MyHttpClient\r\n"
     "Content-Type: multipart/form-data; boundary=Boundary-123456789\r\n"
     "Content-Length: (计算内容长度)\r\n"
     "\r\n"
     "--Boundary-123456789\r\n"
     "Content-Disposition: form-data; name=\"username\"\r\n"
     "\r\n"
     "JohnDoe\r\n"
     "--Boundary-123456789\r\n"
     "Content-Disposition: form-data; name=\"profile_picture\"; "
     "filename=\"profile.jpg\"\r\n"
     "Content-Type: image/jpeg\r\n"
     "\r\n"
     "(Binary file data here)\r\n"
     "--Boundary-123456789--"}
    // 添加其他测试数据
};
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
INSTANTIATE_TEST_SUITE_P(HttpRequestTests, HttpRequestLineParameterizedTest,
                         ::testing::ValuesIn(testParams));
INSTANTIATE_TEST_SUITE_P(HttpRequestTests, HttpRequestHeaderTestParam,
                         ::testing::ValuesIn(headers));
// 注册参数化测试用例
INSTANTIATE_TEST_SUITE_P(ContentTypeTests, HttpContentTypeTest,
                         testing::ValuesIn(full_data));
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

TEST_P(HttpRequestHeaderTestParam, TestHttpRequestHeader) {
  const HttpRequestHeader &param = GetParam();
  HttpParser parser = HttpParser();
  HttpRequest req;
  if (param.shouldThrowError) {
    EXPECT_THROW(parser.parse_request_headers(param.header, req.header.headers),
                 HttpRequestParseException);
  } else {
    EXPECT_NO_THROW(
        parser.parse_request_headers(param.header, req.header.headers));
  }
}

TEST_P(HttpContentTypeTest, TestHttpRequest) {
  const HttpTestData &data = GetParam();

  auto parser = HttpParser();

  auto req = parser.parse(data.httpRequest);
}
