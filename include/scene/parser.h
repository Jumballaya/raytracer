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
#include "environment.h"
#include "file.h"
#include "error.h"

#include "../raytracer/sphere.h"
#include "../raytracer/rect.h"
#include "../raytracer/box.h"

using std::vector;
using std::string;


class Parser {
  public:
    Parser(vector<Token> tkns);
    Parser();

    void     setTokens(vector<Token> tkns);
    Program  parse();
    bool     hasErrors();
    void     printErrors();

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

    void      parseSphere();
    //Box       parseBox();

};

#endif
