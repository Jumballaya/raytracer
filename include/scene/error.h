/**
 * Parser Errors
 */

#ifndef ERROR_H
#define ERROR_H

#include <sstream>

using std::string;
using std::ostringstream;

class ErrorMessage {
  public:
    std::string message;
    int         row;
    int         col;

    ErrorMessage() { message = ""; row = -1; col = -1; }
    ErrorMessage(string msg, int r, int c) : message(msg), row(r), col(c) {}

    void setMessage(string msg) { message = msg; }
    void setPos(int r, int c) { row = r; col = c; }

    string format() {
      ostringstream oss;
      oss << "Error: " << message << " on line " << row << " column " << col;
      return oss.str();
    }
};


#endif
