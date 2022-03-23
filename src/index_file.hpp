#ifndef ADL_INDEX_FILE_H__
#define ADL_INDEX_FILE_H__

#include "file_util.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

class pack_file;

std::string sha1_digit_to_hex(const unsigned char hash[]);

class index_file {
public:
  index_file(const std::string_view &idx_file_name_prefix) {
    idx_file_name_ = idx_file_name_prefix;
    idx_file_name_ += ".idx";
  }

  int parse();

private:
  int open_file();
  int parse_header();
  int parse_contents();

  int idx_fd_ = -1;
  size_t idx_size_;
  std::vector<char> buf_;
  // std::vector<std::string> objectname_list;
  std::string idx_file_name_;
  const char *end = nullptr;
  const char *cur = nullptr;
  unsigned int nr = 0;

  uint32_t idx_version;
  // std::vector<uint32_t> crc;
  off_t objectname_list_offset;
  off_t crc_table_offset;
  off_t offset_table_offset;

  friend class pack_file;
};

#endif // ADL_INDEX_FILE_H__