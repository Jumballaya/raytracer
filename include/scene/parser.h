/***
 *
 * Scene script parser
 *
 * @TODO - Refactor this parser into better functions:
 *
 *      - Top-level parser (Value, Object, Camera, etc.)
 *      - Identifier parser (using a variable)
 *      - Literals parser (creating a literal with/without a name)
 *      - Fields parser (e.g. lookfrom/lookat in Camera)
 *
 */
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "program.h"
#include "file.h"

#include "raytracer/sphere.h"

class Parser {
  public:
    Parser(std::string fp) : file(fp) {
      std::string code = read_file(fp);
      lexer = Lexer(code, code.length());
      nextToken();
      nextToken();
    }

    Program     prog;
    Lexer       lexer;
    std::string file;
    Token       lastToken;
    Token       curToken;
    Token       peekToken;
    std::string error_list;

    void nextToken();

    // Errors
    void registerError(std::string err) { error_list += err; }
    void errors() { std::cout << error_list << std::endl; }
    void peekError(TokenType tt);


    // Token Booleans
    bool curTokenIs(TokenType tt) { return curToken.type == tt; }
    bool peekTokenIs(TokenType tt) { return peekToken.type == tt; }
    bool expectPeek(TokenType tt) {
      if (peekTokenIs(tt)) {
        nextToken();
        return true;
      }
      peekError(tt);
      return false;
    };

    // Parse
    bool parseProgram();

    bool getFloat(float& f);
    bool getIdent(std::string& ident);
    bool getVec3(Vec3& v);
    bool getMaterial(Material& m);
    bool getTexture(Texture& t);
    bool getHitable(Hitable& h);

    // Parse top-level parsing
    void parseMacro();
    void parseValue();
    void parseFloat();
    void parseObject();
    void parseCamera();
    void parseTexture();
    void parseMaterial();
    void parseVec3();

    // Parse second-level
    void parseSphere();
    void parseConstant();
    void parseLambertian();

    // Parse Fields
    bool parseFloatField(std::string ident, float& f);
    bool parseVec3Field(std::string ident, Vec3& v);
    bool parseMaterialField(std::string ident, Material& m);
    bool parseTextureField(std::string ident, Texture& t);

    // Parse Literal
    void parseVec3Literal(Vec3& v);
    void parseFloatLiteral(float& f);
    void parseTextureLiteral(Texture& t);
    void parseMaterialLiteral(Material& m);
};

// NextToken moves the tokens forward
void Parser::nextToken() {
  lastToken = curToken;
  curToken = peekToken;
  peekToken = lexer.nextToken();

  if (curTokenIs(TOK_ILLEGAL)) {
    std::string msg = "got illegal token: ";
    msg += curToken.literal;
    msg += "\n";
    registerError(msg);
    nextToken();
  }
}

// PeekError takes a TokenType and registers an error for that type
void Parser::peekError(TokenType tt) {
  std::string msg = "expected next token to be: ";
  msg += print_token(tt);
  msg += ", got: ";
  msg += print_token(peekToken.type);
  msg += " instead.\n";
  msg += "Current token: ";
  msg += print_token(curToken.type);
  msg += ".\n";
  registerError(msg);
}

// Parse Program is the entry point for the entire parser
bool Parser::parseProgram() {
  while (curToken.type != TOK_EOF) {
    if (curToken.type == TOK_MACRO) parseMacro();
    if (curToken.type == TOK_VALUE) parseValue();
    if (curToken.type == TOK_OBJECT) parseObject();
    if (curToken.type == TOK_CAMERA) parseCamera();
    nextToken();
  }

  if (error_list.length() > 0) {
    return false;
  }

  return true;
}

// Parse Macro parses a macro starting with the character %
void Parser::parseMacro() {
  std::string macro = curToken.literal;
  if (expectPeek(TOK_NUMBER)) {
    float value = std::stof(curToken.literal);
    prog.setMacro(macro, value);
  }
}

// Parse Value is the main branch for Value types (Float, Vec3, Texture, Material, etc.)
void Parser::parseValue() {
  if (expectPeek(TOK_COLON)) {
    switch (peekToken.type) {
      case TOK_TEXTURE:
        nextToken();
        parseTexture();
        return;

      case TOK_MATERIAL:
        nextToken();
        parseMaterial();
        return;

      case TOK_VEC3:
        nextToken();
        parseVec3();
        return;

      case TOK_FLOAT:
        nextToken();
        parseFloat();
        return;

      default:
        return;
        // register error: wrong value type
    }
  }
}

// Parse Object Statement is the main branch for Object types (Sphere, Box, Rect, etc)
void Parser::parseObject() {
  if (expectPeek(TOK_COLON)) {
    switch (peekToken.type) {
      case TOK_SPHERE:
        nextToken();
        parseSphere();
        return;

      default:
        return;
        // register error: wrong value type
    }
  }
}

// Parse Texture Statement is the main branch for Texture types (Constant, Noise, Image, etc)
void Parser::parseTexture() {
  if (expectPeek(TOK_COLON)) {
    switch (peekToken.type) {
      case TOK_CONSTANT:
        nextToken();
        parseConstant();
        return;

      default:
        return;
        // register error: wrong value type
    }
  }
}

// Parse Material Statement is the main branch for Material types (Lambertian, Metal, etc)
void Parser::parseMaterial() {
  if (expectPeek(TOK_COLON)) {
    switch (peekToken.type) {
      case TOK_LAMBERTIAN:
        nextToken();
        parseConstant();
        return;

      default:
        return;
        // register error: wrong value type
    }
  }
}

// Parse Float Statement
void Parser::parseFloat() {
  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_LBRACE)) {
      nextToken();
      float val;
      if (getFloat(val)) {
        prog.env.addFloat(ident, val);
        expectPeek(TOK_SEMICOLON);
        expectPeek(TOK_RBRACE);
      }
    }
  }
}


// Parse Vec3 Statement
void Parser::parseVec3() {
  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_LBRACE)) {
      if (peekTokenIs(TOK_IDENT) || peekTokenIs(TOK_NUMBER)) {
        nextToken();
        float e0, e1, e2;
        if (getFloat(e0) && getFloat(e1) && getFloat(e2)) {
          prog.env.addVec3(ident, Vec3(e0, e1, e2));
          expectPeek(TOK_SEMICOLON);
          expectPeek(TOK_RBRACE);
        }
      }
    }
  }
}


// Parse Sphere Statement
void Parser::parseSphere() {
  float radius;
  Vec3 center;
  Material *m;

  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_LBRACE)) {
      nextToken();
      while (curToken.type != TOK_RBRACE) {
        parseFloatField(std::string("Radius"), radius);
        parseVec3Field(std::string("Center"), center);
        parseMaterialField(std::string("Material"), *m);
        nextToken();
      }

      Sphere *s = new Sphere(center, radius, m);
      prog.env.addObject(ident, s);
      prog.addObject(s);
    }
  }
}

// Parse Constant Statement
void Parser::parseConstant() {
  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_LBRACE)) {
      if (peekTokenIs(TOK_IDENT)) {
        if (curToken.literal.compare("Color") == 0) {
          if (expectPeek(TOK_COLON)) {
            if (peekTokenIs(TOK_IDENT)) {
              Vec3 color;
              prog.env.getVec3(curToken.literal, color);
              Texture *t = new texture::constant(color);
              prog.env.addTexture(ident, t);
            }
            // Otherwise check if there is a Vec3 literal
          }
        } else {
          // register error: no Color field
        }
      }
    }
  }
}

// Parse Lambertian Statement
void Parser::parseLambertian() {
  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_LBRACE)) {
      if (peekTokenIs(TOK_IDENT)) {
        if (curToken.literal.compare("Albedo") == 0) {
          if (expectPeek(TOK_COLON)) {
            if (expectPeek(TOK_IDENT)) {
              Texture *albedo;
              prog.env.getTexture(curToken.literal, *albedo);
              Material *l = new material::lambertian(albedo);
              prog.env.addMaterial(ident, l);
            }
            // Otherwise see if there is a Texture literal
          }
        } else {
          // register error: no Albedo field
        }
      }
    }
  }
}

// Parse float from literal or identifier
// e.g. 10.0    or   ten
bool Parser::getFloat(float& f) {
  if (curTokenIs(TOK_IDENT) || curTokenIs(TOK_NUMBER)) {
    if (curTokenIs(TOK_IDENT)) {
      prog.env.getFloat(curToken.literal, f);
      return true;
    } else if (curTokenIs(TOK_NUMBER)) {
      f = std::stof(curToken.literal);
      return true;
    }
  }
  return false;
}

// Parse identifier
// e.g. (ident)
bool Parser::getIdent(std::string& ident) {
  if (expectPeek(TOK_LPAREN) && peekTokenIs(TOK_IDENT)) {
    nextToken();
    ident = curToken.literal;
    if (expectPeek(TOK_RPAREN)) {
      return true;
    }
  }
  return false;
}

// Parse Vec3 from literal or identifier
// e.g. Vec3 { 10; 10; 10; }    or   sphere_center
bool Parser::getVec3(Vec3& v) {
  if (peekTokenIs(TOK_IDENT) || peekTokenIs(TOK_NUMBER)) {
    if (peekTokenIs(TOK_IDENT)) {
      nextToken();
      return prog.env.getVec3(curToken.literal, v);
    } else if (peekTokenIs(TOK_VEC3)) {
      nextToken();
      if (expectPeek(TOK_LBRACE)) {
        float e0, e1, e2;
        if (getFloat(e0)) {
          if (expectPeek(TOK_SEMICOLON)) {
            nextToken();
            if (getFloat(e1)) {
              if (expectPeek(TOK_SEMICOLON)) {
                nextToken();
                if (getFloat(e2)) {
                  expectPeek(TOK_SEMICOLON);
                  expectPeek(TOK_RBRACE);
                  v = Vec3(e0, e1, e2);
                  return true;
                }
              }
            }
          }
        }
      }
    }
  }
  return false;
}

// Parse Material from literal or identifier
// e.g. Constant : { Color: Vec3 { 0.5; 0.5; 0.5; }; }    or    Constant: sphere_mat
bool Parser::getMaterial(Material& m) {
  if (peekTokenIs(TOK_IDENT)) {
    nextToken();
    return prog.env.getMaterial(curToken.literal, m);
  }
  if (peekTokenIs(TOK_MATERIAL)) {
    parseMaterialLiteral(m);
    return true;
  }
  return false;
}

// Parse Float Literal
// e.g. 0.425632
//void Parser::parseFloatLiteral(float& f) {};

// Parse Vec3 Literal
// e.g. Vec3 { 10; 10; 10; }
//void Parser::parseVec3Literal(Vec3& v) {};

// Parse Texture Literal
//void Parser::parseTextureLiteral(Texture& t) {};

// Parse Material Literal
// e.g. Constant : { Color: Vec3 { 0.5; 0.5; 0.5; }; }
//void Parser::parseMaterialLiteral(Material& m) {};

// Parse a Float field
// e.g.  Radius: 10.0;    or    Radius: ten;
bool Parser::parseFloatField(std::string ident, float& f) {
  if (curTokenIs(TOK_IDENT) && curToken.literal.compare(ident) == 0) {
    if (expectPeek(TOK_COLON)) {
      nextToken();
      getFloat(f);
      if (expectPeek(TOK_SEMICOLON)) {
        nextToken();
        return true;
      };
    }
  }
  return false;
}

// Parse Vec3 Field
// e.g. Center: Vec3 { 0; 0; 0; };    or    Center: sphere_center;
bool Parser::parseVec3Field(std::string ident, Vec3& v) {
  if (curTokenIs(TOK_IDENT) && curToken.literal.compare(ident) == 0) {
    if (expectPeek(TOK_COLON)) {
      nextToken();
      getVec3(v);
      if (expectPeek(TOK_SEMICOLON)) {
        nextToken();
        return true;
      }
    }
  }
  return false;
}

// Parse Material Field
// e.g. Material: Constant { Color: Vec3 { 0.5; 0.5; 0.5; }; };    or   Color: sphere_mat;
bool Parser::parseMaterialField(std::string ident, Material& m) {
  if (curTokenIs(TOK_MATERIAL) && (curToken.literal.compare(ident) == 0)) {
    if (expectPeek(TOK_COLON)) {
      nextToken();
      getMaterial(m);
      if (expectPeek(TOK_SEMICOLON)) {
        nextToken();
        return true;
      }
    }
  }
  return false;
}

// Parse Texture Field
// e.g. Albedo: Constant { Color: Vec3 { 0.5; 0.5; 0.5; }; };    or   Albedo: sphere_texture;
bool Parser::parseTextureField(std::string ident, Texture& t) {
  t = texture::constant(Vec3(0.5, 0.5, 0.5));
}

// Parse Camera Object
void Parser::parseCamera() {
  Vec3 lookfrom(10,10,10);
  Vec3 lookat(0,0,0);
  Vec3 vup(0,1,0);
  float vfov = 40.0;
  float aperature = 0.0;
  float focus = 10.0;

  if (!expectPeek(TOK_LBRACE)) return;
  nextToken();

  // Parse
  while (curToken.type != TOK_RBRACE) {
    parseVec3Field(std::string("Lookfrom"), lookfrom);
    parseVec3Field(std::string("Lookat"), lookat);
    parseVec3Field(std::string("Vup"), vup);
    parseFloatField(std::string("Vfov"), vfov);
    parseFloatField(std::string("Aperature"), aperature);
    parseFloatField(std::string("Focus"), focus);
    nextToken();
  }

  prog.setCamera(lookfrom, lookat, vup, vfov, aperature, focus);
}

#endif
