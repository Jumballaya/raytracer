/***
 *
 * Scene expression types
 *
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <iostream>
#include "tokens.h"

class Expression {};

namespace expression {
  class StringLiteral : public Expression {
    public:
      StringLiteral(std::string val) : value(val) {};
      std::string value;

      virtual TokenType getType() {
        return TOK_STRING;
      }

      std::string getValue() {
        return value;
      }
  };

  class NumberLiteral : public Expression {
    public:
      NumberLiteral(float val) : value(val) {};
      float value;

      virtual TokenType getType() {
        return TOK_NUMBER;
      }

      float getValue() {
        return value;
      }
  };

  class Object : Expression {};

  class Vec3 : Expression {};

  class Texture : Expression {};

  class Material : Expression {};

  class Camera : Expression {};
}

#endif
