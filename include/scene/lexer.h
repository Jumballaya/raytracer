/***
 *
 *
 * Scene Script Lexer
 *
 *
 */
#ifndef LEXER_H
#define LEXER_H

#include <ctype.h>
#include "tokens.h"

bool isLetter(char ch) {
  bool concat = (ch == '_') || (ch == '-');
  return isalpha(ch) || concat;
}

bool isDigit(char ch) {
  return isdigit(ch);
}

bool isWhiteSpace(char ch) {
  return isspace(ch) || (ch == '\n');
}

bool isIdent(char ch) {
  return (isLetter(ch) || isDigit(ch)) && !isWhiteSpace(ch);
}

class Lexer {
  public:
    Lexer() {}
    Lexer(std::string i, int l) : input(i), len(l) {
      pos = 0;
      readPos = 0;
      readChar();
    };

    std::string input;
    int    len;
    int    pos;
    int    readPos;
    char  cur;

    Token nextToken();

  private:
    void skipWhiteSpace();

    void         readChar();
    std::string  readIdent();
    std::string  readNumber();
    std::string  readLine();
};

void Lexer::skipWhiteSpace() {
  while (isWhiteSpace(cur)) readChar();
}

void Lexer::readChar() {
  if (readPos >= len) cur = '\0';
  else cur = input[readPos];
  pos = readPos;
  readPos++;
}

std::string Lexer::readIdent() {
  int position = pos;
  while (isIdent(cur)) readChar();
  return input.substr(position, pos - position);
}

std::string Lexer::readNumber() {
  int position = pos;
  while (isDigit(cur) || cur == '.') readChar();
  return input.substr(position, pos - position);
}

std::string Lexer::readLine() {
  int position = pos;
  while (cur != '\n') readChar();
  return input.substr(position, pos - position);
}

Token Lexer::nextToken() {
  Token tok;

  skipWhiteSpace();

  if (cur == '}') tok = Token(TOK_RBRACE, "}");
  else if (cur == '{') tok = Token(TOK_LBRACE, "{");
  else if (cur == '(') tok = Token(TOK_RPAREN, "(");
  else if (cur == ')') tok = Token(TOK_LPAREN, ")");
  else if (cur == ':') tok = Token(TOK_COLON, ":");
  else if (cur == ';') tok = Token(TOK_SEMICOLON, ";");
  else if (cur == '.') tok = Token(TOK_DOT, ".");
  else if (cur == '%') {
    readChar();
    skipWhiteSpace();
    tok.type = TOK_MACRO;
    tok.literal = readIdent();
  }
  else if (cur == '#') {
    readChar();
    tok.type = TOK_COMMENT;
    tok.literal = readLine();
  }
  else if (cur == '\0') {
    tok.literal = "";
    tok.type = TOK_EOF;
  }
  else if (isLetter(cur)) {
    if (isWhiteSpace(cur)) skipWhiteSpace();
    tok.literal = readIdent();
    tok.type = lookupIdent(tok.literal);
    return tok;
  }
  else if (isDigit(cur)) {
    tok.literal = readNumber();
    tok.type = TOK_NUMBER;
    return tok;
  } else {
    tok = Token(TOK_ILLEGAL, &cur);
  }

  readChar();
  return tok;
}

#endif
