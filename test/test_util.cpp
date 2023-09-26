#include "http.h"
#include "logger.h"
#include "util.h"
#include "gtest/gtest.h"

TEST(testUtil, testSplit) {
  HttpParser parser = HttpParser();
  auto &logger = Logger::get_instance();
  std::string delimiter = ",,,";
  std::string inputString = "apple,,,banana,,,cherry,,,date";

  logger.debugf("The original string : {}, delimiter is {}", inputString,
                delimiter);
  vector<string> result = {"apple", "banana", "cherry", "date"};
  auto res = split(inputString, delimiter);
  for (int i = 0; i < res.size(); i++) {
    ASSERT_EQ(res[i], result[i]);
    logger.debugf("{}", res[i]);
  }
}