
#include <iostream>

#include "SDL2/SDL.h"

#include "error.h"
#include "window.h"
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

    SDL_Renderer* ren = win.renderer();

    SDL_Surface* bmp = nullptr;
    bmp = SDL_LoadBMP("hello.bmp");
    if (bmp == nullptr) {
      std::cout << SDL_GetError() << "\n";
      return 1;
    }

    rwc::Texture tex = win.CreateTexture(bmp);
    SDL_FreeSurface(bmp);

    win.Clear();

    SDL_Rect dst = {100, 100, 200, 80};
    tex.ApplyFull(0, 0);
    tex.ApplyRects(NULL, &dst);

    win.Render();

    SDL_Delay(4000);
    return 0;
  } catch (std::exception err) {
    std::cout << "ERROR: " << err.what() << "\n";
    return 1;
  }
}


