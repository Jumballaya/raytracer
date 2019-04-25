#include "scene/build.h"

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
