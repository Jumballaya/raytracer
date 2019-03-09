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
    unordered_map<string, float>    numbers;
    unordered_map<string, string>   strings;
    unordered_map<string, Vec3>     vec3s;

  public:
    ErrorMessage* setNumber(Token ident, float f);
    ErrorMessage* setString(Token ident, string s);
    ErrorMessage* setVec3(Token ident, Vec3 v);

    float   getNumber(Token ident, ErrorMessage *err);
    string  getString(Token ident, ErrorMessage *err);
    Vec3    getVec3(Token ident, ErrorMessage *err);
};

ErrorMessage* Environment::setNumber(Token ident, float f) {
  ErrorMessage* err;
  auto found = numbers.find(ident.literal);
  if (found != numbers.end()) {
    ostringstream oss;
    oss << "identifier " << ident.literal << " has already been used";
    err->setMessage(oss.str());
    err->setPos(ident.row, ident.column);
    return err;
  } else {
    bool succeed = numbers.insert(std::make_pair(ident.literal, f)).second;
    if (!succeed) {
      err->setMessage("error setting float value");
      err->setPos(ident.row, ident.column);
      return err;
    }
  }
  return NULL;
}

float Environment::getNumber(Token ident, ErrorMessage *err) {
  auto found = numbers.find(ident.literal);
  if (found != numbers.end()) {
    err = NULL;
    return found->second;
  }
  ostringstream oss;
  oss << "identifier " << ident.literal << " can't be found";
  err->setMessage(oss.str());
  err->setPos(ident.row, ident.column);
  return -1;
}

ErrorMessage* Environment::setVec3(Token ident, Vec3 v) {
  ErrorMessage* err;
  auto found = vec3s.find(ident.literal);
  if (found != vec3s.end()) {
    ostringstream oss;
    oss << "identifier " << ident.literal << " has already been used";
    err->setMessage(oss.str());
    err->setPos(ident.row, ident.column);
    return err;
  } else {
    bool succeed = vec3s.insert(std::make_pair(ident.literal, v)).second;
    if (!succeed) {
      err->setMessage("error setting float value");
      err->setPos(ident.row, ident.column);
      return err;
    }
  }
  return NULL;
}

Vec3 Environment::getVec3(Token ident, ErrorMessage *err) {
  auto found = vec3s.find(ident.literal);
  if (found != vec3s.end()) {
    return found->second;
  }
  ostringstream oss;
  oss << "identifier " << ident.literal << " can't be found" << std::endl;
  string msg = oss.str();
  err->setMessage(msg);
  err->setPos(ident.row, ident.column);
  return Vec3(-1, -1, -1);
}

ErrorMessage* Environment::setString(Token ident, string s) {
  ErrorMessage* err;
  auto found = strings.find(ident.literal);
  if (found != strings.end()) {
    ostringstream oss;
    oss << "identifier " << ident.literal << " has already been used";
    err->setMessage(oss.str());
    err->setPos(ident.row, ident.column);
    return err;
  } else {
    bool succeed = strings.insert(std::make_pair(ident.literal, s)).second;
    if (!succeed) {
      err->setMessage("error setting float value");
      err->setPos(ident.row, ident.column);
      return err;
    }
  }
  return NULL;
}

string Environment::getString(Token ident, ErrorMessage* err) {
  auto found = strings.find(ident.literal);
  if (found != strings.end()) {
    err = NULL;
    return found->second;
  }
  ostringstream oss;
  oss << "identifier " << ident.literal << " can't be found";
  err->setMessage(oss.str());
  err->setPos(ident.row, ident.column);
  return "";
}


#endif
