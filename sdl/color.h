
#ifndef COLOR_H_
#define COLOR_H_

#include "SDL2/SDL.h"

#include "error.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const Uint32 R_MASK = 0xff000000;
    const Uint32 G_MASK = 0x00ff0000;
    const Uint32 B_MASK = 0x0000ff00;
    const Uint32 A_MASK = 0x000000ff;
#else
    const Uint32 R_MASK = 0x000000ff;
    const Uint32 G_MASK = 0x0000ff00;
    const Uint32 B_MASK = 0x00ff0000;
    const Uint32 A_MASK = 0xff000000;
#endif

namespace sdl {

class Color {
 public:
  Color() : r(0), g(0), b(0), a(SDL_ALPHA_OPAQUE) { };

  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) { };

  SDL_Color sdl() {
    return SDL_Color{r, g, b, a};
  }

#define COLOR(name, r, g, b) static Color name(int alpha=SDL_ALPHA_OPAQUE) {return Color(r, g, b, alpha);};
COLOR(black, 0, 0, 0);
COLOR(white, 255, 255, 255);
COLOR(gray, 128, 128, 128);
COLOR(silver, 192, 192, 192);
COLOR(red, 255, 0, 0);
COLOR(lime, 0, 255, 0);
COLOR(blue, 0, 0, 255);
COLOR(yellow, 255, 255, 0);
COLOR(cyan, 0, 255, 255);
COLOR(magenta, 255, 0, 255);
COLOR(maroon, 128, 0, 0);
COLOR(green, 0, 128, 0);
COLOR(navy, 0, 0, 128);
COLOR(olive, 128, 128, 0);
COLOR(teal, 0, 128, 128);
COLOR(purple, 128, 0, 128);
#undef COLOR

static Color transparent() {return Color(0, 0, 0, SDL_ALPHA_TRANSPARENT);}

  uint8_t r, g, b, a;
};

} // namespace

#endif
