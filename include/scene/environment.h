/***
 *
 *
 * Scene Script Program Environment
 *
 *
 */
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include <sstream>
#include "error.h"
#include "tokens.h"
#include "../raytracer/vec.h"
#include "../raytracer/material.h"
#include "../raytracer/texture.h"

using std::string;
using std::unordered_map;
using std::ostringstream;

class Environment {
  private:
    unordered_map<string, float>     numbers;
    unordered_map<string, string>    strings;
    unordered_map<string, Vec3>      vec3s;
    unordered_map<string, Texture*>  textures;
    unordered_map<string, Material*> materials;

  public:
    ErrorMessage*  setNumber(Token ident, float f);
    ErrorMessage*  setString(Token ident, string s);
    ErrorMessage*  setVec3(Token ident, Vec3 v);
    ErrorMessage*  setTexture(Token ident, Texture* t);
    ErrorMessage*  setMaterial(Token ident, Material* m);

    float      getNumber(Token ident, ErrorMessage *err);
    string     getString(Token ident, ErrorMessage *err);
    Vec3       getVec3(Token ident, ErrorMessage *err);
    Texture*   getTexture(Token ident, ErrorMessage *err);
    Material*  getMaterial(Token ident, ErrorMessage *err);
};

#endif
