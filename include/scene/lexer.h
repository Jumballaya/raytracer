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
      row = 0;
      col = 0;
      readChar();
    };

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

void Lexer::skipWhiteSpace() {
  while (isWhiteSpace(cur)) readChar();
}

void Lexer::readChar() {
  if (readPos >= len) cur = '\0';
  else cur = input[readPos];
  pos = readPos;
  readPos++;
  col++;
  if (cur == '\n') {
    col = 0;
    row++;
  }
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
  col = 0;
  row++;
  return input.substr(position, pos - position);
}

std::vector<Token> Lexer::tokens() {
  std::vector<Token> tokens;
  Token t = nextToken();
  while (t.type != TOK_EOF) {
    tokens.push_back(t);
    t = nextToken();
  }
  tokens.push_back(t);
  return tokens;
}

Token Lexer::nextToken() {
  Token tok;

  skipWhiteSpace();

  if (cur == '}') tok = Token(TOK_RBRACE, "}", row, col);
  else if (cur == '{') tok = Token(TOK_LBRACE, "{", row, col);
  else if (cur == '(') tok = Token(TOK_LPAREN, "(", row, col);
  else if (cur == ')') tok = Token(TOK_RPAREN, ")", row, col);
  else if (cur == ':') tok = Token(TOK_COLON, ":", row, col);
  else if (cur == ';') tok = Token(TOK_SEMICOLON, ";", row, col);
  else if (cur == '.') tok = Token(TOK_DOT, ".", row, col);
  else if (cur == '=') tok = Token(TOK_EQUAL, "=", row, col);
  else if (cur == '%') {
    readChar();
    skipWhiteSpace();
    tok.type = TOK_MACRO;
    tok.literal = "%";
    tok.row = row;
    tok.column = col;
  }
  else if (cur == '#') {
    readChar();
    tok.type = TOK_COMMENT;
    tok.literal = readLine();
    tok.row = row;
    tok.column = col;
  }
  else if (cur == '\0') {
    tok.literal = "";
    tok.type = TOK_EOF;
    tok.row = row;
    tok.column = col;
  }
  else if (isLetter(cur)) {
    if (isWhiteSpace(cur)) skipWhiteSpace();
    tok.literal = readIdent();
    tok.type = lookupIdent(tok.literal);
    tok.row = row;
    tok.column = col;
    return tok;
  }
  else if (isDigit(cur)) {
    tok.literal = readNumber();
    tok.type = TOK_NUMBER;
    tok.row = row;
    tok.column = col;
    return tok;
  } else {
    tok = Token(TOK_ILLEGAL, &cur, row, col);
  }

  readChar();
  return tok;
}

#endif
