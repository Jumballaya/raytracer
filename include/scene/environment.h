#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <iostream>

#include "raytracer/vec.h"
#include "raytracer/sphere.h"
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

struct sphere_pair {
  std::string ident;
  Sphere      *value;
};

struct sphere_list {
  sphere_pair *pairs;
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
      vectors->len = 0;
      spheres->len = 0;
      materials->len = 0;
      textures->len = 0;
    }

    float_list    *floats;
    vec3_list     *vectors;
    sphere_list   *spheres;
    material_list *materials;
    texture_list  *textures;

    // Add Values
    void addFloat(std::string ident, float object);
    void addVec3(std::string ident, Vec3 object);
    void addSphere(std::string ident, Sphere *object);
    void addMaterial(std::string ident, Material *object);
    void addTexture(std::string ident, Texture *object);

    // Get Values
    bool getVec3(std::string ident, Vec3& v);
    bool getSphere(std::string ident, Sphere& s);
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

void Environment::addSphere(std::string ident, Sphere *object) {
  sphere_pair p;
  p.ident = ident;
  p.value = object;
  spheres->pairs[spheres->len] = p;
  spheres->len++;
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

bool Environment::getSphere(std::string ident, Sphere& s) {
  for (int i = 0; i < spheres->len; i++) {
    if (spheres->pairs[i].ident.compare(ident) == 0) {
      s = *spheres->pairs[i].value;
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
