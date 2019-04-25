#include "scene/file.h"

std::string read_file(std::string fp) {
  std::ifstream ifs(fp);
  std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
  return content;
}
