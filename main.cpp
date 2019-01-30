#include <iostream>
#include "include/scene/program.h"
#include "include/scene/parser.h"

int main(int argc, char **argv) {
  if (argc == 2) {
    Parser p(argv[1]);
    Program prog = p.parseProgram();

    if (p.error_list.length() > 0) {
      p.errors();
      return 1;
    }

    prog.run();

    return 0;
  }

  std::cerr << "must provide a path to a .scene file" << std::endl;

  return 1;
}
