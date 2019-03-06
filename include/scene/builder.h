/**
 * Scene Builder
 */
#ifndef BUILDER_H
#define BUILDER_H

#include <iostream>
#include "parser.h"
#include "program.h"
#include "environment.h"

class Builder {
  public:
    Builder(std::string fp);

    void run();

  private:
    Parser      parser;
    Program     program;
};

Builder::Builder(std::string fp) {
  std::string code = read_file(fp);
  Lexer lexer = Lexer(code, code.length());
  parser.setTokens(lexer.tokens());
}

void Builder::run() {
  std::cout << "Hello World\n";

  // Run the parser and display errors or run the program to build the image
}

#endif
