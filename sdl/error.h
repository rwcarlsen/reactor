
#ifndef ERROR_H_
#define ERROR_H_

#include <exception>
#include <iostream>
#include <string>

namespace sdl {

class FatalErr : std::exception {
 public:
  FatalErr() : msg_(SDL_GetError()) { };
  FatalErr(std::string msg) : msg_(msg) { };
  virtual ~FatalErr() throw() { };
  virtual const char* what() const throw() {
    return msg_.c_str();
  };

 private:
  std::string msg_;
};

} // namespace sdl

#endif
