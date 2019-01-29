#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include "file.h"

enum precedences {
  P_LOWEST,
  P_BLOCK,
  P_PAREN
};

int lookupPrecedence(TokenType tt) {
  switch (tt) {
    case TOK_LPAREN:
      return P_PAREN;
    case TOK_LBRACE:
      return P_BLOCK;
    default:
      return P_LOWEST;
  }
}

class Parser {
  public:
    Parser(std::string fp) : file(fp) {
      std::string code = read_file(fp);
      lexer = Lexer(code, code.length());
      nextToken();
      nextToken();
    }

    Lexer lexer;
    std::string file;
    Token       lastToken;
    Token       curToken;
    Token       peekToken;
    std::string error_list;

    void nextToken();
    void advanceToken();

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

    // Precedence
    int peekPrecedence() { return lookupPrecedence(peekToken.type); }
    int curPrecedence() { return lookupPrecedence(curToken.type); }

    // Parse
    Program parseProgram();

    bool getFloat(float& f, Program prog);
    bool getIdent(std::string& ident);
    bool getVec3(Vec3& v, Program prog);
    bool getMaterial(std::string ident, Material& m, Program prog);
    bool getTexture(std::string ident, Texture& t, Program prog);
    bool getSphere(std::string ident, Sphere& s, Program prog);

    void parseMacro(Program prog);
    void parseValue(Program prog);
    void parseFloat(Program prog);
    void parseObject(Program prog);
    void parseCamera(Program prog);
    void parseTexture(Program prog);
    void parseMaterial(Program prog);
    void parseSphere(Program prog);
    void parseVec3(Program prog);

    void parseConstant(Program prog);
    void parseLambertian(Program prog);
};

void Parser::advanceToken() {
  peekToken = lexer.nextToken();
}

void Parser::nextToken() {
  lastToken = curToken;
  curToken = peekToken;
  advanceToken();

  if (curTokenIs(TOK_ILLEGAL)) {
    std::string msg = "got illegal token: ";
    msg += curToken.literal;
    msg += "\n";
    registerError(msg);
    nextToken();
  }
}

void Parser::peekError(TokenType tt) {
  std::string msg = "expected next token to be: ";
  msg += tt;
  msg += ", got: ";
  msg += peekToken.type;
  msg += " instead.\n";
  registerError(msg);
}

Program Parser::parseProgram() {
  Program prog;
  while (curToken.type != TOK_EOF) {
    if (curToken.type == TOK_MACRO) parseMacro(prog);
    if (curToken.type == TOK_VALUE) parseValue(prog);
    if (curToken.type == TOK_OBJECT) parseObject(prog);
    if (curToken.type == TOK_CAMERA) parseCamera(prog);
    if (curToken.type == TOK_TEXTURE) parseTexture(prog);
    if (curToken.type == TOK_MATERIAL) parseMaterial(prog);
    if (curToken.type == TOK_SPHERE) parseSphere(prog);
    if (curToken.type == TOK_VEC3) parseVec3(prog);
    if (curToken.type == TOK_FLOAT) parseFloat(prog);

    if (curToken.type == TOK_CONSTANT) parseConstant(prog);
    if (curToken.type == TOK_LAMBERTIAN) parseLambertian(prog);
  }

  return prog;
}

void Parser::parseMacro(Program prog) {
  std::string macro = curToken.literal;
  if (expectPeek(TOK_NUMBER)) {
    float value = std::stof(curToken.literal);
    prog.setMacro(macro, value);
  }
}

void Parser::parseValue(Program prog) {
  if (expectPeek(TOK_COLON)) {
    bool t = peekTokenIs(TOK_TEXTURE);
    bool m = peekTokenIs(TOK_MATERIAL);
    bool v = peekTokenIs(TOK_VEC3);
    bool f = peekTokenIs(TOK_FLOAT);
    if (t || m || v || f) {
      nextToken();
    }
  }
  // register error: wrong value type
}

void Parser::parseObject(Program prog) {
  if (expectPeek(TOK_COLON)) {
    bool s = peekTokenIs(TOK_SPHERE);
    if (s) {
      nextToken();
    }
  }
  // register error: wrong value type
}

void Parser::parseTexture(Program prog) {
  if (expectPeek(TOK_COLON)) {
    bool c = peekTokenIs(TOK_CONSTANT);
    if (c) {
      nextToken();
    }
  }
  // register error: wrong texture type
}

void Parser::parseMaterial(Program prog) {
  if (expectPeek(TOK_COLON)) {
    bool l = peekTokenIs(TOK_LAMBERTIAN);
    if (l) {
      nextToken();
    }
  }
  // register error: wrong material type
}

bool Parser::getFloat(float& f, Program prog) {
  if (peekTokenIs(TOK_IDENT) || peekTokenIs(TOK_NUMBER)) {
    if (peekTokenIs(TOK_IDENT)) {
      nextToken();
      prog.env.getFloat(curToken.literal, f);
      return true;
    } else if (peekTokenIs(TOK_NUMBER)) {
      nextToken();
      f = std::stof(curToken.literal);
      return true;
    }
  }
  return false;
}

bool Parser::getIdent(std::string& ident) {
  if (expectPeek(TOK_LPAREN) && peekTokenIs(TOK_IDENT)) {
    nextToken();
    ident = curToken.literal;
    return true;
  }
  return false;
}

//Vec3 { 0; 0; 0; }
bool Parser::getVec3(Vec3& v, Program prog) {
  if (peekTokenIs(TOK_IDENT) || peekTokenIs(TOK_NUMBER)) {
    if (peekTokenIs(TOK_IDENT)) {
      nextToken();
      prog.env.getVec3(curToken.literal, v);
      return true;
    } else if (peekTokenIs(TOK_VEC3)) {
      nextToken();
      if (expectPeek(TOK_LBRACE)) {
        float e0, e1, e2;
        if (getFloat(e0, prog)) {
          expectPeek(TOK_SEMICOLON);
          if (getFloat(e1, prog)) {
            expectPeek(TOK_SEMICOLON);
            if (getFloat(e2, prog)) {
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
  return false;
}

//void Parser::parseVec3Literal(Vec3& v) {};
//void Parser::parseFloatLiteral(float& f) {};
//void Parser::parseTextureLiteral(Texture& t) {};
//void Parser::parseMaterialLiteral(Material& t) {};

void Parser::parseFloat(Program prog) {
  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_RPAREN) && expectPeek(TOK_LBRACE)) {
      float val;
      if (getFloat(val, prog)) {
        prog.env.addFloat(ident, val);
        expectPeek(TOK_SEMICOLON);
        expectPeek(TOK_RBRACE);
      }
    }
  }
}

void Parser::parseVec3(Program prog) {
  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_RPAREN) && expectPeek(TOK_LBRACE)) {
      if (peekTokenIs(TOK_IDENT) || peekTokenIs(TOK_NUMBER)) {
        nextToken();
        float e0, e1, e2;
        if (getFloat(e0, prog) && getFloat(e1, prog) && getFloat(e2, prog)) {
          prog.env.addVec3(ident, Vec3(e0, e1, e2));
          peekTokenIs(TOK_SEMICOLON);
          peekTokenIs(TOK_RBRACE);
        }
      }
    }
  }
}

void Parser::parseConstant(Program prog) {
  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_RPAREN) && expectPeek(TOK_LBRACE)) {
      if (peekTokenIs(TOK_IDENT)) {
        nextToken();
        if (curToken.literal.compare("Color") == 0) {
          nextToken();
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

void Parser::parseLambertian(Program prog) {
  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_RPAREN) && expectPeek(TOK_LBRACE)) {
      if (peekTokenIs(TOK_IDENT)) {
        nextToken();
        if (curToken.literal.compare("Albedo") == 0) {
          nextToken();
          if (expectPeek(TOK_COLON)) {
            Texture *albedo;
            if (peekTokenIs(TOK_IDENT)) {
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

bool Parser::parseFloatField(std::string ident, float& f, Program prog) {
  if (curTokenIs(TOK_IDENT) && curToken.literal.compare(ident) == 0) {
    if (expectPeek(TOK_COLON)) {
      nextToken();
      getFloat(f, prog);
      expectPeek(TOK_SEMICOLON);
    }
  }
}

bool Parser::parseVec3Field(std::string ident, Vec3& v) {
  if (curTokenIs(TOK_IDENT) && curToken.literal.compare(ident) == 0) {
    if (expectPeek(TOK_COLON)) {
      nextToken();
      getVec3(v, prog);
      expectPeek(TOK_SEMICOLON);
    }
  }
}

bool Parser::parseMaterialField(std::string ident, Material& m) {}

bool Parser::parseTextureField(std::string ident, Texture& t) {}

void Parser::parseSphere(Program prog) {
  float radius;
  Vec3 center;
  Material *m;

  std::string ident;
  if (getIdent(ident)) {
    if (expectPeek(TOK_RPAREN) && expectPeek(TOK_LBRACE)) {
      while (curToken.type != TOK_RBRACE) {
        parseFloatField("Radius", radius);
        parseVec3Field("Center", center);
        parseMaterialField("Material", m);
      }
    }
  }
}

void Parser::parseCamera(Program prog) {
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
    parseVec3Field("Lookfrom", lookfrom);
    parseVec3Field("Lookat", lookat);
    parseVec3Field("Vup", vup);
    parseFloatField("Vfov", vfov);
    parseFloatField("Aperature", aperature);
    parseFloatField("Focus", focus);
  }
  nextToken();

  prog.setCamera(lookfrom, lookat, vup, vfov, aperature, focus);
}

#endif
