#include "index_file.hpp"
#include "objects.hpp"
#include <iomanip>

int index_file::parse() {
  int ret = 0;
  if (ret = open_file(); ret) {
    std::cerr << "cannot open index file" << idx_file_name_ << std::endl;
    return ret;
  }

  if (ret = parse_header(); ret) {
    std::cerr << "parse header of " << idx_file_name_ << "failed" << std::endl;
    return ret;
  }

  if (ret = parse_contents(); ret) {
    std::cerr << "parse contents of " << idx_file_name_ << "failed"
              << std::endl;
    return ret;
  }
  return ret;
}

int index_file::open_file() {

  if (idx_fd_ = open(idx_file_name_.data(), O_RDONLY | O_CLOEXEC);
      idx_fd_ < 0) {
    std::cerr << "fatal: Cannot open " << idx_file_name_ << std::endl;
    return 1;
  }

  if (idx_size_ = get_size(idx_fd_); idx_size_ == -1) {
    std::cerr << "fatal: Cannot get the size of " << idx_file_name_
              << std::endl;
    return 1;
  }

  buf_.resize(idx_size_);
  if (ssize_t ret = read_n(idx_fd_, buf_.data(), idx_size_); ret != idx_size_) {
    std::cerr << "fatal: Cannot read full file " << idx_file_name_ << std::endl;
    return 1;
  }
  // std::cout << "all ok! idx_fd= " << idx_fd_ << " idx_size=" << idx_size_
  //           << std::endl;

  cur = buf_.data();
  end = buf_.data() + idx_size_;

  return 0;
}

int index_file::parse_header() {
  /* magic number */
  if (cur + 4 > end)
    return 1;
  if (int ret = memcmp(cur, "\377tOc", 4); ret)
    return 1;
  cur += 4;

  /* version */
  if (cur + sizeof(int) > end)
    return 1;
  if (idx_version = ntohl(*(unsigned int *)cur); idx_version != 2)
    return 1;
  cur += sizeof(uint32_t);
  return 0;
}

int index_file::parse_contents() {
  /* fanout */
  if (cur + 256 * sizeof(uint32_t) > end)
    return 1;
  for (int i = 0; i < 256; i++) {
    auto n = ntohl(*(uint32_t *)cur);
    if (n < nr)
      return 1;
    nr = n;
    cur += sizeof(uint32_t);
  }

  /* objectname list */
  objectname_list_offset = cur - buf_.data();
  // objectname_list.resize(nr);
  // for (int i = 0; i < nr; i++) {
  //   cur += 20;
  // }

  /* crc */
  cur += 20 * nr;
  crc_table_offset = cur - buf_.data();
  for (int i = 0; i < nr; i++) {
    // crc[i] =
  }

  /* offset */
  cur += 4 * nr;
  offset_table_offset = cur - buf_.data();
  for (int i = 0; i < nr; i++) {
    // offset[i]=xxx
  }

  // std::cout << "objectname_list_offset=" << objectname_list_offset <<
  // std::endl
  //           << "crc_offset=" << crc_table_offset << std::endl
  //           << "offsets_offset=" << offset_table_offset << std::endl;
  cur += 4 * nr;
  /* pack file checksum 20B */
  /* index file checksum 20B */
  return 0;
}

std::string sha1_digit_to_hex(const unsigned char hash[]) {
  std::stringstream ss;
  ss.setf(std::ios::hex, std::ios::basefield);

  for (int i = 0; i < GIT_SHA1_RAWSZ; i++) {
    ss.width(2);
    ss.fill('0');
    ss << static_cast<unsigned int>(hash[i]);
  }
  ss.setf(std::ios::dec, std::ios::basefield);
  return ss.str();
}
