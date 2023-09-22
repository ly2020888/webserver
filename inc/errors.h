#include <exception>
#include <sstream>
#include <string>
using std::exception;
using std::ostringstream;
using std::string;

class ReadError : public exception {
public:
  ReadError(const std::string &message) {
    ostringstream ss;
    ss << "ReadError: " << errorMessage;
    errorMessage = ss.str();
  }
  virtual const char *what() const noexcept { return errorMessage.c_str(); }

private:
  string errorMessage;
};

class HttpRequestParseException : public std::runtime_error {
public:
  enum class ErrorCode {
    InvalidRequestLine,
    InvalidHeader,
    InvalidMethod,
    InvalidUri,
    InvalidVersion,
    MissingHostHeader,
    Other
  };

  HttpRequestParseException(ErrorCode code, const std::string &message)
      : std::runtime_error(message), error_code_(code) {}

  ErrorCode GetErrorCode() const { return error_code_; }

private:
  ErrorCode error_code_;
};