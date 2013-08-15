
#ifndef MATH_H_
#define MATH_H_

namespace rwc {

class Position {
 public:
  Position() : x(0), y(0) { };
  Position(int x, int y) : x(x), y(y) { };
  int x;
  int y;
};

class Size {
 public:
  Size() : w(0), h(0) { };
  Size(int w, int h) : w(w), h(h) { };
  int w;
  int h;
};

} // namespace rwc

#endif

