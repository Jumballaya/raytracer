#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class CLI {
  public:
    CLI(int ac, char** r);

    int    argc;
    char** raw;
    const char*  scene_file;
    const char*  out_file;
    const char*  ext;

    void usage();
    void version();
};

#endif
