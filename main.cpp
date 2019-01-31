#include <iostream>
#include "include/scene/program.h"
#include "include/scene/parser.h"
#include "include/cli.h"

int main(int argc, char **argv) {
  CLI cli(argc, argv);
  Parser p(cli.scene_file);
  Program prog = p.parseProgram();

  if (p.error_list.length() > 0) {
    p.errors();
    return 1;
  }

  prog.run();

  return 0;
}
