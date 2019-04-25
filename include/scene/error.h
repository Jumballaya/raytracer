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

    ErrorMessage();
    ErrorMessage(std::string msg, int r, int c);

    void setMessage(std::string msg);
    void setPos(int r, int c);

    std::string format();
};

#endif
