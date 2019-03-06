/***
 *
 * Scene script parser
 *
 *
 *
 */
#ifndef PARSER_H
#define PARSER_H

#include <cstdarg>
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

class Parser {
  public:
    Parser(std::vector<Token> tkns) : tokens(tkns) { current = 0; }
    Parser() { current = 0; }

    void setTokens(std::vector<Token> tkns) { tokens = tkns; current = 0; }

  private:
    std::vector<Token>        tokens;
    int                       current;
    std::vector<ErrorMessage> errors;
    Environment               env;

    // Errors
    void registerError(std::string err);
    bool hasErrors();
    void printErrors();

    // Boolean functions
    bool match(TokenType types...);
    bool check(TokenType type);
    bool isAtEnd();

    // Token producing functions
    Token consume(TokenType type, std::string message);
    Token advance();
    Token peek();
    Token previous;

    // Others
    void synchronize();

    //Parse
    std::vector<Statement> parse(); // return a vector of Statements
    Expression        parseExpression();
    Statement         parseStatement();

    Expression        parseLiteral();
    Expression        parseObject();
    Expression        parseVec3();
    Expression        parseTexture();
    Expression        parseMaterial();
    Expression        parseCamera();

    Expression        parseTConstant();
    Expression        parseTChecker();
    Expression        parseTNoise();
    Expression        parseTImage();

    Expression        parseMLambertian();
    Expression        parseMMetal();
    Expression        parseMDielectric();
    Expression        parseMDiffuseLight();
    Expression        parseMIsotropic();

    Expression        parseSphere();
    Expression        parseBox();
    Expression        parseRect();

    Statement         parseObjStatement();       // Add the object to the env
    Statement         parseValStatement();       // Add the value to the env
    Statement         parseMacroStatement();     // Add the macro to the env
    Statement         parseExpressionStatement();
};

void Parser::registerError(std::string err) {
  ErrorMessage e = ErrorMessage(err, row, col);
  errors.push_back(e);
};

bool Parser::hasErrors() {
  return errors.size() > 0;
};

void Parser::printErrors() {
  for (auto e = errors.begin(); e != errors.end(); ++e) {
    std::cout << e.format() << "\n";
  }
};

bool Parser::match(TokenType types...) {
  va_list args;
  va_start(args, types);

  while (*types != '\0') {
    if (check(*types)) {
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

Token Parser::consume(TokenType type, std::string message) {
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

#endif
