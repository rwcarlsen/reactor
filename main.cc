
#include <iostream>

#include "SDL2/SDL.h"

#include "error.h"
#include "window.h"
#include "renderer.h"
#include "texture.h"
#include "sdl_init.h"

int main(int argc, char** argv) {
  try {
    rwc::SDLinit init(SDL_INIT_EVERYTHING);

    int w = 640;
    int h = 480;
    rwc::Window win(w, h, SDL_WINDOW_SHOWN);
    win.set_title("Hello World");
    win.Center();

    rwc::Renderer ren = win.renderer();
    rwc::Texture tex(ren, "hello.bmp");

    ren.Clear();
    SDL_Rect dst = {100, 100, 200, 80};
    tex.ApplyFull(0, 0);
    tex.ApplyRects(NULL, &dst);

    ren.Render();

    SDL_Delay(4000);
    return 0;
  } catch (std::exception err) {
    std::cout << "ERROR: " << err.what() << "\n";
    return 1;
  }
}


