/**
 * Scene Builder
 */
#ifndef BUILDER_H
#define BUILDER_H

#include <iostream>
#include "parser.h"
#include "program.h"
#include "environment.h"

#include "expression.h"

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

void print_val(Expression *lit) {
  if(dynamic_cast<expression::StringLiteral>(lit)) {
    expression::StringLiteral sl = dynamic_cast<expression::StringLiteral>(lit);
    std::cout << print_token(sl.getValue()) << "\n";
    return;
  }
  if(dynamic_cast<expression::NumberLiteral>(lit)) {
    expression::NumberLiteral nl = dynamic_cast<expression::NumberLiteral>(lit);
    std::cout << print_token(nl.getValue()) << "\n";
    return;
  }
}

void Builder::run() {
  // Run the parser and display errors or run the program to build the image

  Expression *sl = new expression::StringLiteral("StringLiteral");
  Expression *nl = new expression::NumberLiteral(10.5);

  print_val(sl);
  print_val(nl);

  delete sl;
  delete nl;
}

#endif
