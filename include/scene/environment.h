/***
 *
 *
 * Scene Script Program Environment
 *
 *
 */
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>
#include <stdlib.h>

#include "raytracer/vec.h"
#include "raytracer/hitable.h"
#include "raytracer/material.h"
#include "raytracer/texture.h"

struct float_pair {
  std::string ident;
  float       value;
};

struct float_list {
  float_pair *pairs;
  int        len;
};

struct vec3_pair {
  std::string ident;
  Vec3        value;
};

struct vec3_list {
  vec3_pair *pairs;
  int       len;
};

struct object_pair {
  std::string ident;
  Hitable     *value;
};

struct object_list {
  object_pair *pairs;
  int         len;
};

struct material_pair {
  std::string ident;
  Material    *value;
};

struct material_list {
  material_pair *pairs;
  int           len;
};

struct texture_pair {
  std::string ident;
  Texture     *value;
};

struct texture_list {
  texture_pair *pairs;
  int          len;
};

class Environment {
  public:
    Environment() {
      vectors = (vec3_list*) malloc(sizeof(vec3_list));
      vectors->pairs = (vec3_pair*) malloc(sizeof(Vec3) * 20);
      vectors->len = 0;

      objects = (object_list*) malloc(sizeof(object_list));
      objects->pairs = (object_pair*) malloc(sizeof(Hitable) * 20);
      objects->len = 0;

      materials = (material_list*) malloc(sizeof(material_list));
      materials->pairs = (material_pair*) malloc(sizeof(Material) * 20);
      materials->len = 0;

      textures = (texture_list*) malloc(sizeof(texture_list));
      textures->pairs = (texture_pair*) malloc(sizeof(Texture) * 20);
      textures->len = 0;

      floats = (float_list*) malloc(sizeof(float_list));
      floats->pairs = (float_pair*) malloc(sizeof(float) * 20);
      floats->len = 0;
    }

    float_list    *floats;
    vec3_list     *vectors;
    object_list   *objects;
    material_list *materials;
    texture_list  *textures;

    // Add Values
    void addFloat(std::string ident, float object);
    void addVec3(std::string ident, Vec3 object);
    void addObject(std::string ident, Hitable *object);
    void addMaterial(std::string ident, Material *object);
    void addTexture(std::string ident, Texture *object);

    // Get Values
    bool getVec3(std::string ident, Vec3& v);
    bool getObject(std::string ident, Hitable& o);
    bool getMaterial(std::string ident, Material& m);
    bool getTexture(std::string ident, Texture& t);
    bool getFloat(std::string ident, float& f);
};

void Environment::addVec3(std::string ident, Vec3 object) {
  vec3_pair p;
  p.ident = ident;
  p.value = object;
  vectors->pairs[vectors->len] = p;
  vectors->len++;
}

void Environment::addObject(std::string ident, Hitable *object) {
  object_pair o;
  o.ident = ident;
  o.value = object;
  objects->pairs[objects->len] = o;
  objects->len++;
}

void Environment::addMaterial(std::string ident, Material *object) {
  material_pair p;
  p.ident = ident;
  p.value = object;
  materials->pairs[materials->len] = p;
  materials->len++;
}

void Environment::addTexture(std::string ident, Texture *object) {
  texture_pair p;
  p.ident = ident;
  p.value = object;
  textures->pairs[textures->len] = p;
  textures->len++;
}

void Environment::addFloat(std::string ident, float object) {
  float_pair p;
  p.ident = ident;
  p.value = object;
  floats->pairs[floats->len] = p;
  floats->len++;
}

bool Environment::getVec3(std::string ident, Vec3& v) {
  for (int i = 0; i < vectors->len; i++) {
    if (vectors->pairs[i].ident.compare(ident) == 0) {
      v = vectors->pairs[i].value;
      return true;
    }
  }
  return false;
}

bool Environment::getObject(std::string ident, Hitable& o) {
  for (int i = 0; i < objects->len; i++) {
    if (objects->pairs[i].ident.compare(ident) == 0) {
      o = *objects->pairs[i].value;
      return true;
    }
  }
  return false;
}

bool Environment::getMaterial(std::string ident, Material& m) {
  for (int i = 0; i < materials->len; i++) {
    if (materials->pairs[i].ident.compare(ident) == 0) {
      m = *materials->pairs[i].value;
      return true;
    }
  }
  return false;
}

bool Environment::getTexture(std::string ident, Texture& t) {
  for (int i = 0; i < textures->len; i++) {
    if (textures->pairs[i].ident.compare(ident) == 0) {
      t = *textures->pairs[i].value;
      return true;
    }
  }
  return false;
}

bool Environment::getFloat(std::string ident, float& f) {
  for (int i = 0; i < floats->len; i++) {
    if (floats->pairs[i].ident.compare(ident) == 0) {
      f = floats->pairs[i].value;
      return true;
    }
  }
  return false;
}

#endif
