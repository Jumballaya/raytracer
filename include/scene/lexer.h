/***
 *
 *
 * Scene Script Lexer
 *
 *
 */
#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <cctype>
#include <algorithm>

#include "tokens.h"

class Lexer {
  public:
    Lexer() {}
    Lexer(std::string i, int l);

    std::vector<Token> tokens();
    Token nextToken();

  private:
    std::string input;
    int    len;
    int    pos;
    int    readPos;
    char  cur;

    int row;
    int col;

    void skipWhiteSpace();

    void         readChar();
    std::string  readIdent();
    std::string  readNumber();
    std::string  readLine();
};

#endif
