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
    case TOK_VALUE: return "Value";
    case TOK_OBJECT: return "Object";
    case TOK_CAMERA: return "Camera";
    case TOK_TEXTURE: return "Texture";
    case TOK_MATERIAL: return "Material";
    case TOK_VEC3: return "Vec3";
    case TOK_FLOAT: return "Float";
    case TOK_SPHERE: return "Sphere";
    case TOK_LAMBERTIAN: return "Lambertian";
    case TOK_CONSTANT: return "Constant";
    default: return "Token Not Found";
  }
}

#endif
