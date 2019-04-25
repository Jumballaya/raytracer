#include "scene/error.h"

ErrorMessage::ErrorMessage() { message = ""; row = -1; col = -1; }
ErrorMessage::ErrorMessage(std::string msg, int r, int c) : message(msg), row(r), col(c) {}

void ErrorMessage::setMessage(std::string msg) { message = msg; }
void ErrorMessage::setPos(int r, int c) { row = r; col = c; }

std::string ErrorMessage::format() {
  std::ostringstream oss;
  oss << "Error: " << message << " on line " << row << " column " << col;
  return oss.str();
}
