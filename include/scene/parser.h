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

    Program   parse();

    bool hasErrors();
    void printErrors();

  private:
    vector<Token>        tokens;
    int                  current;
    vector<ErrorMessage> errors;
    Environment          env;
    Program              program;

    // Errors
    void registerError(string err);

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
    void      parseStatement();

    void      parseObjStatement();
    void      parseValStatement();
    void      parseMacroStatement();

    void        parseObject(Token ident);
    void        parseValue(Token ident);
    void        parseMacro(Token ident);
    void        parseCamera();
    float       parseNumber();
    Vec3        parseVec3();
    Texture*    parseTexture();
    Material*   parseMaterial();

    Texture*    parseTConstant();
    //Texture*    parseTChecker();
    //Texture*    parseTNoise();
    //Texture*    parseTImage();

    Material*     parseMLambertian();
    //Material*     parseMMetal();
    //Material*     parseMDielectric();
    //Material*     parseMDiffuseLight();
    //Material*     parseMIsotropic();

    void      parseSphere(Hitable* h);
    //Box       parseBox();

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
    Hitable *h;
    parseSphere(h);
    program.addObject(h);
    return;
  }
  if (check(TOK_CAMERA)) {
    // Parse camera field and edit the program there
    parseCamera();
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
    if (err) errors.push_back(*err);
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
  if (check(TOK_NUMBER)) {
    float f = parseNumber();
    ErrorMessage* err = env.setNumber(ident, f);
    if (err) errors.push_back(*err);
    return;
  }
  if (check(TOK_STRING)) {
    ErrorMessage* err = env.setString(ident, peek().literal);
    if (err) errors.push_back(*err);
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
      if (check(TOK_NUMBER)) vec.push_back(parseNumber());
      if (check(TOK_IDENT)) {
        // get the value from the environment
        ErrorMessage *err;
        float lit = env.getNumber(peek(), err);
        if (err) {
          errors.push_back(*err);
        } else {
          vec.push_back(lit);
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
      consume(TOK_RBRACE, "expected right brace after texture declaration");
      return t;
    }
  }

  registerError("improper Texture formation");
  synchronize();
  return NULL;
}

// Texture Constant -> "Color" ":" ( Vec3 | Identifier ) ";"
Texture* Parser::parseTConstant() {
  if (peek().type == TOK_IDENT && peek().literal == "Color") {
    advance();
    consume(TOK_COLON, "expected colon");
    Vec3 c = parseVec3();
    consume(TOK_SEMICOLON, "expected semicolon");
    Texture *t;
    *t = texture::constant(c);
    return t;
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
      consume(TOK_RBRACE, "expected right brace after material declaration");
      return m;
    }
  }

  registerError("improper Material formation");
  synchronize();
  return NULL;
}

// Material:Lambertian -> "Albedo" ":" ( Texture | Identifier ) ";"
Material* Parser::parseMLambertian() {
  if (peek().type == TOK_IDENT && peek().literal == "Albedo") {
    advance();
    consume(TOK_COLON, "expected colon");
    Texture *t = parseTexture();
    consume(TOK_SEMICOLON, "expected semicolon");
    Material *m;
    *m = material::lambertian(t);
    return m;
  }
  registerError("improper Material:Lambertian formation");
  synchronize();
  return NULL;
}

// NumberLiteral -> NUMBER
float Parser::parseNumber() {
  if (peek().type == TOK_NUMBER) {
    string::size_type sz;
    return stof(peek().literal, &sz);
  }
  ErrorMessage* err;
  float ret = env.getNumber(peek(), err);
  if (err) errors.push_back(*err);
  return ret;
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

// Camera ->  "Camera" "{"
//              "LookFrom" ":" ( Vec3 | Identifier ) ";"
//              "LookAt" ":" ( Vec3 | Identifier ) ";"
//              "Vfov" ":" ( NUMBER | Identifier ) ";"
//              "Aperature" ":" ( NUMBER | Identifier ) ";"
//              "Focus" ":" ( NUMBER | Identifier ) ";"
//            "}"
void Parser::parseCamera() {
  if (match(vector<TokenType>{TOK_CAMERA, TOK_LBRACE})) {
    Vec3 lf, la;
    float v, a, f;
    while (peek().type != TOK_RBRACE && !isAtEnd()) {
      if (check(TOK_IDENT)) {
        if (peek().literal == "LookFrom") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          lf = parseVec3();
          consume(TOK_SEMICOLON, "expected token ';'");
        } else if (peek().literal == "LookAt") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          la = parseVec3();
          consume(TOK_SEMICOLON, "expected token ';'");
        } else if (peek().literal == "Vfov") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          v = parseNumber();
          consume(TOK_SEMICOLON, "expected token ';'");
        } else if (peek().literal == "Aperature") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          a = parseNumber();
          consume(TOK_SEMICOLON, "expected token ';'");
        } else if (peek().literal == "Focus") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          f = parseNumber();
          consume(TOK_SEMICOLON, "expected token ';'");
        }
      } else {
        synchronize();
      }
    }
    program.setCamera(lf, la, Vec3(0, 1, 0), v, a, f);
    return;
  }
  // error message
  registerError("improper Camera formation");
  synchronize();
}

// Sphere ->  "Sphere" "{"
//              "Radius" ":" ( NUMBER | Identifier ) ";"
//              "Center" ":" ( Vec3 | Identifier ) ";"
//              "Material" ":" ( Material | Identifier ) ";"
//            "}"
void Parser::parseSphere(Hitable* h) {
  if (match(vector<TokenType>{TOK_SPHERE, TOK_LBRACE})) {
    float r;
    Vec3 c;
    Material* m;
    while (peek().type != TOK_RBRACE && !isAtEnd()) {
      if (check(TOK_IDENT)) {
        if (peek().literal == "Radius") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          r = parseNumber();
          consume(TOK_SEMICOLON, "expected token ';'");
        } else if (peek().literal == "Center") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          c = parseVec3();
          consume(TOK_SEMICOLON, "expected token ';'");
        } else if (peek().literal == "Material") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          m = parseMaterial();
          consume(TOK_SEMICOLON, "expected token ';'");
        }
      } else {
        synchronize();
      }
    }
    h = new Sphere(c, r, m);
  }
  // error message
  registerError("improper Sphere formation");
  synchronize();
}

#endif
