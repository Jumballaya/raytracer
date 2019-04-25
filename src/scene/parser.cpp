#include "scene/parser.h"

Parser::Parser(vector<Token> tkns) : tokens(tkns) { current = 0; }
Parser::Parser() { current = 0; }
void Parser::setTokens(vector<Token> tkns) { tokens = tkns; current = 0; }

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
    if (tokens.at(current + 1).type == TOK_SEMICOLON) return;
    advance();
  }
};

Program Parser::parse() {
  while (!isAtEnd()) {
    parseStatement();
  }
  return program;
}

// Statement -> ObjStmt | ValStmt | MacroStmt | ExprStmt
void Parser::parseStatement() {
  if (check(TOK_OBJ)) return parseObjStatement();
  if (check(TOK_VAL)) return parseValStatement();
  if (check(TOK_MACRO)) return parseMacroStatement();
  advance();
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
    parseSphere();
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
    ErrorMessage* err = env.setTexture(ident, t);
    if (err) errors.push_back(*err);
    return;
  }
  if (check(TOK_MATERIAL)) {
    Material *m = parseMaterial();
    ErrorMessage* err = env.setMaterial(ident, m);
    if (err) errors.push_back(*err);
    return;
  }
  if (check(TOK_NUMBER)) {
    float f = parseNumber();
    advance();
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
  if (check(TOK_VEC3)) {
    if (match(vector<TokenType>{TOK_VEC3, TOK_LBRACE})) {
      advance();
      // parse the 3 fields ( NUMBER | Identifier ) ";"
      int i;
      for (i = 0; i < 3; i++) {
        if (check(TOK_NUMBER)) {
          vec.push_back(parseNumber());
          advance();
        }
        if (check(TOK_IDENT)) {
          // get the value from the environment
          ErrorMessage *err;
          float lit = env.getNumber(peek(), err);
          if (err) {
            errors.push_back(*err);
            synchronize();
          } else {
            vec.push_back(lit);
            advance();
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
  if (check(TOK_IDENT)) {
    ErrorMessage err;
    Vec3 v = env.getVec3(peek(), &err);
    advance();
    if (err.row != -1) {
      errors.push_back(err);
      synchronize();
      return Vec3(-1, -1, -1);
    } else {
      return v;
    }
  }
  registerError("improper Vec3 formation");
  synchronize();
  return Vec3(-1, -1, -1);
}

// Texture -> "Texture" ":" [type] "{" [fields] "}"
Texture* Parser::parseTexture() {
  if (check(TOK_TEXTURE)) {
    if (match(vector<TokenType>{TOK_TEXTURE, TOK_COLON})) {
      advance();
      if (peek().type == TOK_CONSTANT) {
        advance();
        consume(TOK_LBRACE, "expected left brace after texture type");
        Texture* t = parseTConstant();
        consume(TOK_RBRACE, "expected right brace after texture declaration");
        return t;
      }
    }
  }
  if (check(TOK_IDENT)) {
    ErrorMessage err;
    Texture* t = env.getTexture(peek(), &err);
    advance();
    if (err.row != -1) {
      errors.push_back(err);
      synchronize();
      return NULL;
    } else {
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
  if (check(TOK_MATERIAL)) {
    if (match(vector<TokenType>{TOK_MATERIAL, TOK_COLON})) {
      advance();
      if (peek().type == TOK_LAMBERTIAN) {
        advance();
        consume(TOK_LBRACE, "expected left brace after material type");
        Material* m = parseMLambertian();
        consume(TOK_RBRACE, "expected right brace after material declaration");
        return m;
      }
    }
  }
  if (check(TOK_IDENT)) {
    ErrorMessage err;
    Material* m = env.getMaterial(peek(), &err);
    advance();
    if (err.row != -1) {
      errors.push_back(err);
      synchronize();
      return NULL;
    } else {
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
  if (peek().type == TOK_IDENT) {
    ErrorMessage* err;
    float ret = env.getNumber(peek(), err);
    if (err) errors.push_back(*err);
    return 1.0;
  }
  return -0.1;
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
    advance();
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
          advance();
          consume(TOK_SEMICOLON, "expected token ';'");
        } else if (peek().literal == "Aperature") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          a = parseNumber();
          advance();
          consume(TOK_SEMICOLON, "expected token ';'");
        } else if (peek().literal == "Focus") {
          advance();
          consume(TOK_COLON, "expected token ':'");
          f = parseNumber();
          advance();
          consume(TOK_SEMICOLON, "expected token ';'");
        } else {
          advance();
        }
      } else {
        synchronize();
      }
    }
    program.setCamera(lf, la, Vec3(0, 1, 0), v, a, f);
    advance();
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
void Parser::parseSphere() {
  if (check(TOK_SPHERE)) {
    if (match(vector<TokenType>{TOK_SPHERE, TOK_LBRACE})) {
      advance();
      float r;
      Vec3 c;
      Material* m;
      while (peek().type != TOK_RBRACE && !isAtEnd()) {
        if (check(TOK_IDENT) || check(TOK_MATERIAL)) {
          if (peek().literal == "Radius") {
            advance();
            consume(TOK_COLON, "expected token ':'");
            r = parseNumber();
            advance();
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
          } else {
            advance();
          }
        } else {
          synchronize();
        }
      }
      Sphere *s = new Sphere(c, r, m);
      program.addSphere(s);
      consume(TOK_RBRACE, "expected right brace");
      return;
    }
  }
  // error message
  registerError("improper Sphere formation");
  synchronize();
}
