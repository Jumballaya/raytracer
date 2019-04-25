/**
 * Scene Builder
 */
#ifndef BUILDER_H
#define BUILDER_H

#include <iostream>
#include "parser.h"
#include "program.h"
#include "environment.h"

#include "../raytracer/sphere.h"
#include "../raytracer/vec.h"
#include "../raytracer/material.h"
#include "../raytracer/texture.h"

class Builder {
  public:
    Builder(std::string fp);

    void run();

  private:
    Parser      parser;
    Program     program;

    std::vector<Token> tokens;
};

#endif
