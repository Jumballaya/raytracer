/***
 *
 * Scene expression types
 *
 */

#ifndef EXPRESSION_H
#define EXPRESSION_H

class Expression {};

namespace expression {
  class Literal : Expression {};

  class Object : Expression {};

  class Vec3 : Expression {};

  class Texture : Expression {};

  class Material : Expression {};

  class Camera : Expression {};
}

#endif
