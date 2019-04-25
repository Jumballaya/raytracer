#include <iostream>
#include "scene/builder.h"
#include "cli.h"

int main(int argc, char **argv) {
  CLI cli(argc, argv);
  Builder b(cli.scene_file);
  b.run();

  return 0;
}
