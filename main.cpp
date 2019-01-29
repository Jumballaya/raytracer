#include <iostream>
#include "include/scene/evaluator.h"

int parse_file(int argc, char **argv) {
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

int main(int argc, char **argv) {
  Scene s = build_scene();
  s.toggle_lights();
  std::cout << s.render() << std::endl;
}
