
#include <iostream>

#include "SDL2/SDL.h"
#include "error.h"

int main(int argc, char** argv) {
  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    return FatalSDL();
  }

  int w = 640;
  int h = 480;
  SDL_Window* win = nullptr;
  SDL_Renderer* ren = nullptr;
  if (SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_SHOWN, &win, &ren) == -1) {
    return FatalSDL();
  }
  SDL_SetWindowTitle(win, "Hello World");

  SDL_Surface* bmp = nullptr;
  bmp = SDL_LoadBMP("hello.bmp");
  if (bmp == nullptr) {
    return FatalSDL();
  }

  SDL_Texture* tex = nullptr;
  tex = SDL_CreateTextureFromSurface(ren, bmp);
  SDL_FreeSurface(bmp);

  SDL_RenderClear(ren);
  SDL_RenderCopy(ren, tex, NULL, NULL);
  SDL_RenderPresent(ren);

  SDL_Delay(4000);

  SDL_DestroyTexture(tex);
  SDL_DestroyRenderer(ren);
  SDL_DestroyWindow(win);

  SDL_Quit();
  return 0;
}


