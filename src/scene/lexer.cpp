#include "scene/lexer.h"

static inline bool isLetter(char ch) {
  bool concat = (ch == '_') || (ch == '-');
  return isalpha(ch) || concat;
}

static inline bool isDigit(char ch) {
  return isdigit(ch);
}

static inline bool isWhiteSpace(char ch) {
  return isspace(ch) || (ch == '\n');
}

static inline bool isIdent(char ch) {
  return (isLetter(ch) || isDigit(ch)) && !isWhiteSpace(ch);
}

static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
    return !std::isspace(ch);
  }));
}

static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

Lexer::Lexer(std::string i, int l) : input(i), len(l) {
  pos = 0;
  readPos = 0;
  row = 0;
  col = 0;
  readChar();
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
    std::string ident = readIdent();
    trim(ident);
    tok.literal = ident;
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
