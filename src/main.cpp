#include "verify_pack.hpp"

#define HEAD_SIZE 4
// .git/objects/pack/pack-2324a3590d8ad3ce175765a63a273aef5e25c3d7.idx
int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "./a.out idx-file-path" << std::endl;
    return 1;
  }

  size_t file_name_size = strlen(argv[1]);
  std::string_view file_name{argv[1]};
  auto last_pos = file_name.find_last_of('.');
  std::string_view subfix =
      file_name.substr(last_pos, file_name_size - last_pos);
  if (last_pos == std::string::npos ||  (subfix != ".pack" && subfix != ".idx")) {
    std::cerr << "bad pack/idx filename:" << file_name << std::endl;
    return 1;
  }

  verify_pack vp(file_name.substr(0, last_pos));
  if (vp.parse()) {
    std::cerr << "bad pack/idx filename:" << file_name << std::endl;
  }
  /*  else {
      std::cout << "ok" << std::endl;
    } */
}