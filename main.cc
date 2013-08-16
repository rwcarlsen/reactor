
#include <iostream>

#include "SDL2/SDL.h"

#include "sdl/error.h"
#include "sdl/window.h"
#include "sdl/renderer.h"
#include "sdl/texture.h"
#include "sdl/sdl_init.h"
#include "sdl/color.h"

#include "phys/system.h"

using sdl::Color;

int main(int argc, char** argv) {
  try {
    sdl::SDLinit init(SDL_INIT_EVERYTHING);

    int w = 640;
    int h = 480;
    sdl::Window win(w, h, SDL_WINDOW_SHOWN);
    win.set_title("Hello World");
    win.Center();

    sdl::Renderer ren = win.renderer();
    sdl::Texture tex(ren, "hello.bmp");

    ren.Clear();

    SDL_Rect dst = {100, 100, 200, 80};
    SDL_Rect dst2 = {200, 200, 200, 120};

    tex.ApplyFull(0, 0);
    tex.ApplyRects(NULL, &dst);
    ren.FillRect(&dst2, Color::maroon());
    ren.Render();

    SDL_Delay(4000);
    return 0;
  } catch (std::exception err) {
    std::cout << "ERROR: " << err.what() << "\n";
    return 1;
  }
}


