#ifndef ADL_PACK_FILE_H__
#define ADL_PACK_FILE_H__
#include "file_util.hpp"
#include "index_file.hpp"
#include "objects.hpp"
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>

class pack_file {
public:
  pack_file(const std::string_view &pack_file_prefix, index_file *idx_file)
      : idx_file_(idx_file) {
    pack_file_name_ = pack_file_prefix;
    pack_file_name_ += ".pack";
  }

  ~pack_file() {
    if (pack_fd_ != -1)
      close(pack_fd_);
  }

  int parse();

private:
  int open_file();
  int parse_header();
  int parse_contents();

  int set_object_with_offset_and_oid(object &obj, const unsigned char *oid,
                                     off_t offset);

  index_file *idx_file_;
  int pack_fd_ = -1;
  size_t pack_size_ = -1;
  const char *end = nullptr;
  const char *cur = nullptr;
  uint32_t verison_;
  uint32_t nr;

  // std::vector<object> objs;

  std::vector<char> buf_;
  std::string pack_file_name_;

  std::vector<object> objects;
};

#endif // ADL_PACK_FILE_H__