
#ifndef WINDOW_H_
#define WINDOW_H_

#include "SDL2/SDL.h"

#include "sdl/error.h"

namespace sdl {

class Window {
 public:
  Window(std::string title, int width, int height,
         uint32_t flags) : win_(nullptr) {
    win_ = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, width, height, flags);
    if (win_ == nullptr) {
      throw FatalErr();
    }
  };

  ~Window() {
    SDL_DestroyWindow(win_);
  };

  Window(const Window& that) = delete;

  SDL_Window* raw() const {
    return win_;
  }

  std::string title() {
    return SDL_GetWindowTitle(win_);
  };

  void set_title(std::string s) {
    SDL_SetWindowTitle(win_, s.c_str());
  };

  void pos(int* x, int* y) {
    SDL_GetWindowPosition(win_, x, y);
  };

  void set_pos(int x, int y) {
    SDL_SetWindowPosition(win_, x, y);
    SDL_UpdateWindowSurface(win_);
  };

  void size(int* w, int* h) {
    SDL_GetWindowPosition(win_, w, h);
  };

  void set_size(int w, int h) {
    SDL_SetWindowPosition(win_, w, h);
  };

  void Center() {
    set_pos(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
  };

  void Show() {
    return SDL_ShowWindow(win_);
  };

  void Hide() {
    return SDL_HideWindow(win_);
  };

  void Maximize() {
    return SDL_MaximizeWindow(win_);
  };

  void Minimize() {
    return SDL_MinimizeWindow(win_);
  };

  void Restore() {
    return SDL_RestoreWindow(win_);
  };

 private:
  SDL_Window* win_;
};

} // namespace sdl

#endif
