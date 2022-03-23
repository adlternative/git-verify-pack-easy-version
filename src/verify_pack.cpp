#include "verify_pack.hpp"

verify_pack::verify_pack(const std::string_view &pack_file_prefix)
    : idx_file_(new index_file(pack_file_prefix)),
      pack_file_(new pack_file(pack_file_prefix, idx_file_)) {}

int verify_pack::parse() {
  int ret = 0;
  if (ret = idx_file_->parse(); ret) {
    std::cout << "index file parse failed" << std::endl;
    return ret;
  }
  if (ret = pack_file_->parse(); ret) {
    std::cout << "pack file parse failed" << std::endl;
    return ret;
  }
  return ret;
}
