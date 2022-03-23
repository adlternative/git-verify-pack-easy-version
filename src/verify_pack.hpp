#ifndef ADL_VERIFY_PACK_H__
#define ADL_VERIFY_PACK_H__

#include "index_file.hpp"
#include "pack_file.hpp"
#include <string>

class verify_pack {
public:
  verify_pack(const std::string_view &pack_file_prefix);
  int parse();
  ~verify_pack() {
    delete idx_file_;
    delete pack_file_;
  }

private:
  index_file *idx_file_;
  pack_file *pack_file_;
};

#endif // ADL_VERIFY_PACK_H__