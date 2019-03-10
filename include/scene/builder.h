/**
 * Scene Builder
 */
#ifndef BUILDER_H
#define BUILDER_H

#include <iostream>
#include "parser.h"
#include "program.h"
#include "environment.h"

#include "raytracer/sphere.h"
#include "raytracer/vec.h"
#include "raytracer/material.h"
#include "raytracer/texture.h"

class Builder {
  public:
    Builder(std::string fp);

    void run();

  private:
    Parser      parser;
    Program     program;

    std::vector<Token> tokens;
};

Builder::Builder(std::string fp) {
  std::string code = read_file(fp);
  Lexer lexer = Lexer(code, code.length());
  tokens = lexer.tokens();
  parser.setTokens(tokens);
}

void Builder::run() {
  program = parser.parse();

  if (parser.hasErrors()) {
    parser.printErrors();
    return;
  }

  program.run();
}

#endif
