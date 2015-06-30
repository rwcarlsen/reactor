
ifeq ($(CXX),)
	CXX = g++
endif

source = $(filter-out main.cc,$(wildcard *.cc))
headers += $(wildcard *.h)
headers += $(wildcard draw/*.h)
headers += $(wildcard phys/*.h)
headers += $(wildcard sdl/*.h)

includes += $(shell export PKG_CONFIG_PATH="$(HOME)/.local/lib/pkgconfig"; pkg-config --cflags sdl2)
includes += $(shell export PKG_CONFIG_PATH="$(HOME)/.local/lib/pkgconfig"; pkg-config --cflags SDL2_ttf)

link_flags += $(LDFLAGS) -lSDL2 -lSDL2_ttf -std=c++11
cpp_flags += $(CXXFLAGS) -O3 -std=c++11 -I. $(includes)

reactor : main.cc $(headers)
	$(CXX) $< $(link_flags) $(cpp_flags) -o $@


debug : cpp_flags += -g
debug : reactor

clean :
	rm -f reactor *.o
