#ifndef CLI_H
#define CLI_H

#define APP_NAME "raytracer"
#define APP_VERSION "0.0.1"

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

// Print Help Message
void CLI::usage() {
  printf("Usage: %s [OPTION] SCENE_FILE\n", APP_NAME);
  printf("SCENE_FILE is the path to the scene file to render\n");
  printf("OPTIONS include:\n");
  printf("   -h, --help           this help menu\n");
  printf("   -e, --ext            image format [png, ppm, bmp], default ppm\n");
  printf("   -o, --out            image format image path, default output.[ext]\n");
  printf("   -v, --version        print current version\n");
}

// Print version message
void CLI::version() {
  printf("%s version %s\n", APP_NAME, APP_VERSION);
}

// Parses the raw argv array into the cli options
CLI::CLI(int ac, char** av) {
  argc = ac;
  raw = av;
  // Set defaults
  ext = "ppm";
  out_file = (std::string("output.") + ext).c_str();
  scene_file = "";

  if (argc < 2) {
    usage();
    exit(0);
  }

  char* tok;
  for (int i = 0; i < argc; i++) {
    tok = raw[i];

    // File Extension
    if (strcmp(tok, "-e") == 0 || strcmp(tok, "--ext") == 0) {
      i++;
      ext = raw[i];
      continue;
    }

    // Output file
    if (strcmp(tok, "-o") == 0 || strcmp(tok, "--out") == 0) {
      i++;
      out_file = raw[i];
      continue;
    }

    // Help
    if (strcmp(tok, "-h") == 0 || strcmp(tok, "--help") == 0) {
      i++;
      usage();
      exit(0);
    }

    // Version
    if (strcmp(tok, "-v") == 0 || strcmp(tok, "--version") == 0) {
      i++;
      version();
      exit(0);
    }

    // The other option is the SCENE_FILE
    if (i != 0) {
      scene_file = raw[i];
    }
  }

  if (strcmp(scene_file, "") == 0) {
    std::cerr << "You must provide a path to your scene file" << std::endl;
    exit(0);
  }
}

#endif
