/***
 *
 *
 * Scene Script Tokens
 *
 *
 */
#ifndef TOKENS_H
#define TOKENS_H

#include <iostream>

enum TokenType {
  TOK_COMMENT,     // 0
  TOK_MACRO,       // 1

  TOK_EOF,         // 2
  TOK_ILLEGAL,     // 3

  TOK_LBRACE,      // 4
  TOK_RBRACE,      // 5

  TOK_LPAREN,      // 6
  TOK_RPAREN,      // 7

  TOK_SEMICOLON,   // 8
  TOK_COLON,       // 9
  TOK_DOT,         // 10
  TOK_EQUAL,       // 11

  TOK_IDENT,       // 12
  TOK_NUMBER,      // 13
  TOK_STRING,      // 14

  TOK_CAMERA,      // 15

  TOK_TEXTURE,     // 16
  TOK_MATERIAL,    // 17
  TOK_VEC3,        // 18
  TOK_FLOAT,       // 19
  TOK_SPHERE,      // 20

  TOK_LAMBERTIAN,  // 21
  TOK_CONSTANT,    // 22

  TOK_OBJ,         // 23
  TOK_VAL,         // 24
};

class Token {
  public:
    Token() {}
    Token(TokenType tt, std::string lit, int r, int c) : type(tt), literal(lit), row(r), column(c) {}

    TokenType   type;
    std::string literal;
    int         row;
    int         column;
};

#endif
