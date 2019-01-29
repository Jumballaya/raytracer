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

  TOK_IDENT,       // 11
  TOK_NUMBER,      // 12

  TOK_VALUE,       // 13
  TOK_OBJECT,      // 14
  TOK_CAMERA,      // 15

  TOK_TEXTURE,     // 16
  TOK_MATERIAL,    // 17
  TOK_VEC3,        // 18
  TOK_FLOAT,       // 19
  TOK_SPHERE,      // 20

  TOK_LAMBERTIAN,  // 21
  TOK_CONSTANT,    // 22
};

class Token {
  public:
    Token() {}
    Token(TokenType tt, std::string lit) : type(tt), literal(lit) {}

    TokenType   type;
    std::string literal;
};

static const char * const macros[] = {
  "start_time",
  "end_time",
  "set_nx",
  "set_ny",
  "set_ns",
  "lights"
};

TokenType lookupIdent(std::string literal) {
  if (literal.compare("Value") == 0) return TOK_VALUE;
  if (literal.compare("Float") == 0) return TOK_FLOAT;
  if (literal.compare("Texture") == 0) return TOK_TEXTURE;
  if (literal.compare("Constant") == 0) return TOK_CONSTANT;
  if (literal.compare("Material") == 0) return TOK_MATERIAL;
  if (literal.compare("Lambertian") == 0) return TOK_LAMBERTIAN;
  if (literal.compare("Object") == 0) return TOK_OBJECT;
  if (literal.compare("Sphere") == 0) return TOK_SPHERE;
  if (literal.compare("Camera") == 0) return TOK_CAMERA;
  if (literal.compare("Vec3") == 0) return TOK_VEC3;

  return TOK_IDENT;
}

#endif
