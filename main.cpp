#include <iostream>
#include "include/scene/program.h"
#include "include/scene/parser.h"
#include "include/cli.h"

int main(int argc, char **argv) {
  CLI cli(argc, argv);
  Parser p(cli.scene_file);
  bool success = p.parseProgram();

  if (!success) {
    p.errors();
    return 1;
  }

  p.prog.run();

  return 0;
}
