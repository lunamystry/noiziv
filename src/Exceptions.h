/**
 *   @file Exceptions.h
 *   @authors Mandla Mbuli (0705871Y),
 *            Godfrey Mnikathi (542562)
 *   @version 03.08
 *   @date 03/08/2013
 *   @brief Exceptions used in the Det class
 **/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

using namespace std;

/**
 * @class FileNotFound
 * this is an exception for a file which is not found
 **/
class FileNotFound: public exception {
 public:

  FileNotFound(const char* reason) {
    this->reason = reason;
  }
  virtual const char* what() const throw()
  {
    return this->reason;
  }

 private:
  const char* reason;
};

/**
 * @class InvalidMatrix
 * this is an exception for an invalid matrix
 **/
class InvalidMatrix: public exception {
 public:

  InvalidMatrix(const char* reason) {
    this->reason = reason;
  }
  virtual const char* what() const throw()
  {
    return this->reason;
  }

 private:
  const char* reason;
};

#endif
