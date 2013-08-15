
#include <iostream>

#include "SDL2/SDL.h"

#include "error.h"
#include "window.h"
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

    SDL_Texture* tex = nullptr;
    tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);

    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);

    SDL_Delay(4000);

    SDL_DestroyTexture(tex);
    return 0;
  } catch (std::exception err) {
    std::cout << "ERROR: " << err.what() << "\n";
    return 1;
  }
}


