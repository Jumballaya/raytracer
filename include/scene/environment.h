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
#include "raytracer/vec.h"

using std::string;
using std::unordered_map;
using std::ostringstream;

class Environment {
  private:
    unordered_map<string, float>    floats;
    unordered_map<string, string>   strings;
    unordered_map<string, Vec3>     vec3s;

  public:
    ErrorMessage* setFloat(Token ident, float f);
    ErrorMessage* setString(Token ident, string s);
    ErrorMessage* setVec3(Token ident, Vec3 v);

    float   getFloat(Token ident, ErrorMessage *err);
    string  getString(Token ident, ErrorMessage *err);
    Vec3    getVec3(Token ident, ErrorMessage *err);
};

ErrorMessage* Environment::setFloat(Token ident, float f) {
  ErrorMessage* err;
  auto found = floats.find(ident.literal);
  if (found != floats.end()) {
    ostringstream oss;
    oss << "identifier " << ident.literal << " has already been used";
    err->setMessage(oss.str());
    err->setPos(ident.row, ident.column);
    return err;
  } else {
    bool succeed = floats.insert(std::make_pair(ident.literal, f)).second;
    if (!succeed) {
      err->setMessage("error setting float value");
      err->setPos(ident.row, ident.column);
      return err;
    }
  }
  return NULL;
}

float Environment::getFloat(Token ident, ErrorMessage *err) {
  auto found = floats.find(ident.literal);
  if (found != floats.end()) {
    err = NULL;
    return found->second;
  }
  ostringstream oss;
  oss << "identifier " << ident.literal << " can't be found";
  err->setMessage(oss.str());
  err->setPos(ident.row, ident.column);
  return -1;
}


#endif
