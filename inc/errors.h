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