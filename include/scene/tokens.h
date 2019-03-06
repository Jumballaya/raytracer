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

  TOK_CAMERA,      // 14

  TOK_TEXTURE,     // 15
  TOK_MATERIAL,    // 16
  TOK_VEC3,        // 17
  TOK_FLOAT,       // 18
  TOK_SPHERE,      // 19

  TOK_LAMBERTIAN,  // 20
  TOK_CONSTANT,    // 21

  TOK_OBJ,         // 22
  TOK_VAL,         // 23
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

TokenType lookupIdent(std::string literal) {
  if (literal.compare("Float") == 0) return TOK_FLOAT;
  if (literal.compare("Texture") == 0) return TOK_TEXTURE;
  if (literal.compare("Constant") == 0) return TOK_CONSTANT;
  if (literal.compare("Material") == 0) return TOK_MATERIAL;
  if (literal.compare("Lambertian") == 0) return TOK_LAMBERTIAN;
  if (literal.compare("Sphere") == 0) return TOK_SPHERE;
  if (literal.compare("Camera") == 0) return TOK_CAMERA;
  if (literal.compare("Vec3") == 0) return TOK_VEC3;
  if (literal.compare("obj") == 0) return TOK_OBJ;
  if (literal.compare("val") == 0) return TOK_VAL;

  return TOK_IDENT;
}

const char* print_token(TokenType tt) {
  switch (tt) {
    case TOK_COMMENT: return "Comment";
    case TOK_MACRO: return "Macro";
    case TOK_EOF: return "EOF";
    case TOK_ILLEGAL: return "Illegal";
    case TOK_LBRACE: return "Left Brace";
    case TOK_RBRACE: return "Right Brace";
    case TOK_LPAREN: return "Left Paren";
    case TOK_RPAREN: return "Right Paren";
    case TOK_SEMICOLON: return "Semicolon";
    case TOK_COLON: return "Colon";
    case TOK_DOT: return "Dot";
    case TOK_IDENT: return "Ident";
    case TOK_NUMBER: return "Number";
    case TOK_CAMERA: return "Camera";
    case TOK_TEXTURE: return "Texture";
    case TOK_MATERIAL: return "Material";
    case TOK_VEC3: return "Vec3";
    case TOK_FLOAT: return "Float";
    case TOK_SPHERE: return "Sphere";
    case TOK_LAMBERTIAN: return "Lambertian";
    case TOK_CONSTANT: return "Constant";
    case TOK_OBJ: return "Object Assignment";
    case TOK_VAL: return "Value Assignment";
    case TOK_EQUAL: return "Equals";
    default: return "Token Not Found";
  }
}

#endif
