#include "pack_file.hpp"
#include "objects.hpp"

int pack_file::parse() {
  int ret = 0;
  if (ret = open_file(); ret) {
    std::cerr << "cannot open index file" << pack_file_name_ << std::endl;
    return ret;
  }

  if (ret = parse_header(); ret) {
    std::cerr << "parse header of " << pack_file_name_ << "failed" << std::endl;
    return ret;
  }

  if (ret = parse_contents(); ret) {
    std::cerr << "parse contents of " << pack_file_name_ << "failed"
              << std::endl;
    return ret;
  }

  return ret;
}

int pack_file::open_file() {
  if (pack_fd_ = open(pack_file_name_.data(), O_RDONLY | O_CLOEXEC);
      pack_fd_ < 0) {
    std::cerr << "fatal: Cannot open " << pack_file_name_ << std::endl;
    return 1;
  }

  if (pack_size_ = get_size(pack_fd_); pack_size_ == -1) {
    std::cerr << "fatal: Cannot get the size of " << pack_file_name_
              << std::endl;
    return 1;
  }
  buf_.resize(pack_size_);
  if (ssize_t ret = read_n(pack_fd_, buf_.data(), pack_size_);
      ret != pack_size_) {
    std::cerr << "fatal: Cannot read full file " << pack_file_name_
              << std::endl;
    return 1;
  }
  // std::cout << "all ok! pack_fd= " << pack_fd_ << " pack_size=" << pack_size_
  //           << std::endl;
  cur = buf_.data();
  end = buf_.data() + pack_size_;
  return 0;
}

int pack_file::parse_header() {
  /* PACK */
  if (cur + 4 > end)
    return 1;
  if (int ret = memcmp(cur, "PACK", 4); ret)
    return 1;
  cur += 4;

  /* version */
  if (cur + 4 > end)
    return 1;
  verison_ = ntohl(*(uint32_t *)cur);
  if (verison_ != 2)
    return 1;
  cur += 4;

  /* number_object */
  if (cur + 4 > end)
    return 1;
  nr = ntohl(*(uint32_t *)cur);
  if (nr != idx_file_->nr)
    return 1;
  cur += 4;

  objects.resize(nr);
  // std::cout << "verison=" << verison_ << std::endl;
  // std::cout << "nr=" << nr << std::endl;
  return 0;
}

int pack_file::set_object_with_offset_and_oid(object &obj,
                                              const unsigned char *oid,
                                              off_t offset) {
  auto object_block = buf_.data() + offset;
  const char *cur = object_block;
  int cur_entry = *(int *)cur;

  obj.set_oid(oid);
  obj.type = (object_type)((cur_entry >> 4) & 7);
  if (obj.type >= 5 || obj.type <= 0)
    return 1;
  // std::cout << "objecttype=" << get_object_type((object_type)type) <<
  // std::endl;
  obj.size = cur_entry & 0xf;
  unsigned int shift = 4;
  while (cur_entry & 0x80) {
    cur++;
    cur_entry = *(int *)cur;
    obj.size += (cur_entry & 0x7f) << shift;
    shift += 7;
  }
  // std::cout << "size=" << size << std::endl;
  cur++;
  /* 然后 从 cur -> cur + size 之间都是 data */
  // obj.buf

  return 0;
}

int pack_file::parse_contents() {
  auto cur_offset = idx_file_->buf_.data() + idx_file_->offset_table_offset;
  auto cur_objname = idx_file_->buf_.data() + idx_file_->objectname_list_offset;

  for (int i = 0; i < nr; i++) {
    uint32_t offset = ntohl(*(int *)cur_offset);
    // std::cout << "offset" << (offset & 0x7fffffff) << std::endl;
    set_object_with_offset_and_oid(
        objects[i], (const unsigned char *)cur_objname, offset & 0x7fffffff);

    /* check objectname */
    /* check crc */

    /* output objectname */
    std::cout << "objectname=" << sha1_digit_to_hex(objects[i].oid)
              << std::endl;
    /* output objecttype */
    std::cout << "objecttype=" << get_object_type(objects[i].type) << std::endl;

    // sha1_digit_to_hex();

    if (offset & 0x80000000)
      break;
    cur_objname += 20;
    cur_offset += sizeof(uint32_t);
  }
  return 0;
}
