#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using std::vector;
class Buffer {
public:
  Buffer(size_t initialSize) : buffer_(initialSize), cursor_(0) {}

  void write(const char *data, size_t size) {
    ensure_capacity(size);
    for (size_t i = 0; i < size; ++i) {
      buffer_[cursor_++] = data[i];
    }
  }
  std::string as_str() {
    std::string str(buffer_.data(), buffer_.size());
    return str;
  }
  void write(size_t size) {
    ensure_capacity(size);
    cursor_ += size;
  }
  void read(size_t size) {
    ensure_capacity(size);
    if (cursor_ - size >= 0) {
      cursor_ -= size;
    }
    cursor_ = 0;
  }

  char *get_raw() { return buffer_.data() + cursor_; }
  size_t size() const { return cursor_; }
  size_t unused_size() const { return buffer_.size() - cursor_; }
  void clear() { cursor_ = 0; }
  bool empty() { return cursor_ == 0; }

private:
  void ensure_capacity(size_t size) {
    if (cursor_ + size >= buffer_.size()) {
      buffer_.resize(cursor_ + size);
    }
  }
  vector<char> take() {
    vector<char> data(buffer_.begin(), buffer_.begin() + cursor_);
    clear();
    return data;
  }

  vector<char> buffer_;
  size_t cursor_;
};