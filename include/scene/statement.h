/**
 * Statements
 */
#ifndef STATEMENT_H
#define STATEMENT_H

class Statement {};

namespace statement {
  class Object : Statement {};

  class Value : Statement {};

  class Macro : Statement {};

  class Expression : Statement {};
}

#endif
