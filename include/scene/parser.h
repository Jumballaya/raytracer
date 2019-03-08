/***
 *
 * Scene script parser
 *
 *
 *
 */
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "lexer.h"
#include "program.h"
#include "expression.h"
#include "environment.h"
#include "statement.h"
#include "file.h"
#include "error.h"

#include "raytracer/sphere.h"
#include "raytracer/rect.h"
#include "raytracer/box.h"

using std::vector;
using std::string;


class Parser {
  public:
    Parser(vector<Token> tkns) : tokens(tkns) { current = 0; }
    Parser() { current = 0; }

    void setTokens(vector<Token> tkns) { tokens = tkns; current = 0; }

  private:
    vector<Token>        tokens;
    int                  current;
    vector<ErrorMessage> errors;
    Environment          env;
    Program              program;

    // Errors
    void registerError(string err);
    bool hasErrors();
    void printErrors();

    // Boolean functions
    bool match(vector<TokenType>);
    bool check(TokenType type);
    bool isAtEnd();

    // Token producing functions
    Token consume(TokenType type, string message);
    Token advance();
    Token peek();
    Token previous();

    // Others
    void synchronize();

    //Parse
    Program   parse();
    void      parseStatement();

    void      parseObjStatement();
    void      parseValStatement();
    void      parseMacroStatement();

    void        parseObject(Token ident);
    void        parseValue(Token ident);
    void        parseMacro(Token ident);
    Camera      parseCamera();
    float       parseNumberLiteral();
    Vec3        parseVec3();
    Texture*    parseTexture();
    Material*   parseMaterial();

    texture::constant   parseTConstant();
    //texture::checker    parseTChecker();
    //texture::noise      parseTNoise();
    //texture::image      parseTImage();

    material::lambertian     parseMLambertian();
    //material::metal          parseMMetal();
    //material::dielectric     parseMDielectric();
    //material::diffuse_light  parseMDiffuseLight();
    //material::isotropic      parseMIsotropic();

    Sphere    parseSphere();
    Box       parseBox();

};

void Parser::registerError(string err) {
  ErrorMessage e = ErrorMessage(err, tokens.at(current).row, tokens.at(current).column);
  errors.push_back(e);
};

bool Parser::hasErrors() {
  return errors.size() > 0;
};

void Parser::printErrors() {
  for (auto e = errors.begin(); e != errors.end(); ++e) {
    std::cout << (*e).format() << "\n";
  }
};

bool Parser::match(vector<TokenType> types) {
  for (auto t = types.begin(); t != types.end(); ++t) {
    if (check(*t)) {
      advance();
      return true;
    }
  }

  return false;
};

bool Parser::check(TokenType type) {
  if (isAtEnd()) return false;
  return peek().type == type;
};

bool Parser::isAtEnd() {
  return peek().type == TOK_EOF;
};

Token Parser::consume(TokenType type, string message) {
  if (check(type)) return advance();
  registerError(message);
};

Token Parser::advance() {
  if (!isAtEnd()) current++;
  return previous();
};

Token Parser::peek() {
  return tokens.at(current);
};

Token Parser::previous() {
  return tokens.at(current - 1);
};

void Parser::synchronize() {
  advance();

  while (!isAtEnd()) {
    if (previous().type == TOK_SEMICOLON) return;
    advance();
  }
};

Program Parser::parse() {
  while (!isAtEnd()) {
    parseStatement();
    advance();
  }
  return program;
}

// Statement -> ObjStmt | ValStmt | MacroStmt | ExprStmt
void Parser::parseStatement() {
  if (check(TOK_OBJ)) return parseObjStatement();
  if (check(TOK_VAL)) return parseValStatement();
  if (check(TOK_MACRO)) return parseMacroStatement();
}

// ObjStmt -> "obj" Identifier "=" Expression ";"
void Parser::parseObjStatement() {
  consume(TOK_OBJ, "expected keyword 'obj'");
  Token ident = consume(TOK_IDENT, "expected identifier");
  consume(TOK_EQUAL, "expected token '='");
  parseObject(ident);
  consume(TOK_SEMICOLON, "expected token ';'");
}

// Object -> Sphere | Box
void Parser::parseObject(Token ident) {
  if (check(TOK_SPHERE)) {
    Sphere s = parseSphere();
    // Add obj to environment with the identifier name
    return;
  }
}

// ValStmt -> "val" Identifier "=" Expression ";"
void Parser::parseValStatement() {
  consume(TOK_VAL, "expected keyword 'val'");
  Token ident = consume(TOK_IDENT, "expected identifier");
  consume(TOK_EQUAL, "expected token '='");
  parseValue(ident);
  consume(TOK_SEMICOLON, "expected token ';'");
}

// Value -> Vec3 | Texture | Material | Camera | STRING | NUMBER
void Parser::parseValue(Token ident) {
  if (check(TOK_VEC3)) {
    Vec3 obj = parseVec3();
    ErrorMessage* err = env.setVec3(ident, obj);
    if (err) errors.push_back(err);
    return;
  }
  if (check(TOK_TEXTURE)) {
    Texture *t = parseTexture();
    // Add val to environment with the identifier name
    return;
  }
  if (check(TOK_MATERIAL)) {
    Material *m = parseMaterial();
    // Add val to environment with the identifier name
    return;
  }
  if (check(TOK_CAMERA)) {
    // Parse camera field and edit the program there
    parseCamera();
    return;
  }
  if (check(TOK_NUMBER)) {
    float f = parseFloatLiteral();
    ErrorMessage* err = env.setFloat(ident, f);
    if (err) errors.push_back(err);
    return;
  }
  if (check(TOK_STRING)) {
    ErrorMessage* err = env.setString(ident, peek().literal);
    if (err) errors.push_back(err);
    return;
  }
}

// Vec3 -> "Vec3" "{" ( (NUMBER | Identifier) ";" )*3 "}"
Vec3 Parser::parseVec3() {
  vector<float> vec;
  if (match(vector<TokenType>{TOK_VEC3, TOK_LBRACE})) {
    // parse the 3 fields ( NUMBER | Identifier ) ";"
    int i;
    for (i = 0; i < 3; i++) {
      if (check(TOK_NUMBER)) vector.push_back(parseFloatLiteral());
      if (check(TOK_IDENT)) {
        // get the value from the environment
        ErrorMessage *err;
        float lit = env.getFloat(peek(), err);
        if (err) {
          errors.push_back(err);
        } else {
          vector.push_back(lit);
        }
      }
      consume(TOK_SEMICOLON, "expected semicolon after expression");
    }
    consume(TOK_RBRACE, "expected right brace");
    Vec3 ret = Vec3(vec.at(0), vec.at(1), vec.at(2));
    return ret;
  }
  registerError("improper Vec3 formation");
  synchronize();
  return Vec3(-1, -1, -1);
}

// Texture -> "Texture" ":" [type] "{" [fields] "}"
Texture* Parser::parseTexture() {
  if (match(vector<TokenType>{TOK_TEXTURE, TOK_COLON})) {
    if (peek().type == TOK_CONSTANT) {
      advance();
      consume(TOK_LBRACE, "expected left brace after texture type");
      Texture* t = parseTConstant();
      consume(TOK_RBACE, "expected right brace after texture declaration");
      return t;
    }
  }

  registerError("improper Texture formation");
  synchronize();
  return NULL;
}

// Texture Constant -> "Color" ":" ( Vec3 | Identifier ) ";"
texture::constant Parser::parseTConstant() {
  if (peek().type == TOK_IDENT && peek().literal == "Color") {
    advance();
    consume(TOK_COLON, "expected colon");
    Vec3 c = parseVec3();
    consume(TOK_SEMICOLON, "expected semicolon");
    return texture::constant(c);
  }
  registerError("improper Texture:Constant formation");
  synchronize();
  return NULL;
}

// Material -> "Material" ":" [type] "{" [fields] "}"
Material* Parser::parseMaterial() {
  if (match(vector<TokenType>{TOK_MATERIAL, TOK_COLON})) {
    if (peek().type == TOK_LAMBERTIAN) {
      advance();
      consume(TOK_LBRACE, "expected left brace after material type");
      Material* m = parseMLambertian();
      consume(TOK_RBACE, "expected right brace after material declaration");
      return m;
    }
  }

  registerError("improper Material formation");
  synchronize();
  return NULL;
}

// Material:Lambertian -> "Albedo" ":" ( Texture | Identifier ) ";"
texture::constant Parser::parseMLambertian() {
  if (peek().type == TOK_IDENT && peek().literal == "Albedo") {
    advance();
    consume(TOK_COLON, "expected colon");
    Texture *t = parseTexture();
    consume(TOK_SEMICOLON, "expected semicolon");
    return material::lambertian(t);
  }
  registerError("improper Material:Lambertian formation");
  synchronize();
  return NULL;
}

// FloatLiteral -> NUMBER
float Parser::parseFloatLiteral() {
  string::size_type sz;
  return stof(peek().literal, &sz);
}

// MacroStmt -> "%" Identifier ( NUMBER | STRING )
void Parser::parseMacroStatement() {
  consume(TOK_MACRO, "expected keyword '%'");
  Token ident = consume(TOK_IDENT, "expected identifier");
  parseMacro(ident);
}

void Parser::parseMacro(Token ident) {
  // Parse the macro value and set it on the program
  if (check(TOK_STRING)) {
    // not implemented
    advance();
    return;
  }
  Token n = consume(TOK_NUMBER, "expected string or number literal for macro value");
  string::size_type sz;
  float lit = stof(n.literal, &sz);
  program.setMacro(ident.literal, lit);
}

void Parser::parseCamera() {}

#endif
