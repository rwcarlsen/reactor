
#include <iostream>

int FatalSDL() {
  std::cout << SDL_GetError() << "\n";
  return 1;
}
