/**
 * Parser Errors
 */

#ifndef ERROR_H
#define ERROR_H

#include <sstream>

class ErrorMessage {
  public:
    std::string message;
    int         row;
    int         col;

    ErrorMessage(std::string msg, int r, int c) : message(msg), row(r), col(c) {};

    std::string format() {
      std::ostringstream oss;
      oss << "Error: " << message << " on line " << row << " column " << col;
      return oss.str();
    }
};


#endif
