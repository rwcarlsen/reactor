
Reactor Game
=============

![Alt text](/screenshot1.png?raw=true "Screenshot")

Installation
-------------

To install:

Install dependencies:

``` bash
# Ubuntu
sudo apt-get install libsdl2-dev libsdl2-ttf-dev

# Arch
sudo pacman -S sdl2 sdl2_ttf

# Mac (requires macports: https://www.macports.org)
sudo port install libsdl2 libsdl2_ttf
```

And then run ``make``.  You will get a binary named ``reactor`` that you can
run to play the game!

How to Play
-----------

Create geometries you want and introduce some neutrons.  Physics is all done
real-time, and responds instantly as materials are being moved around.

* Add/create materials/objects: drag from toolbar onto main screen.

* Remove/delete materials/objects: drag onto toolbar.

* Create neutron burst: right-click anywhere on the screen.

* Toggle streaming source: right-click on the quadrant of the desired
  streaming direction.

* Resize materials/objects: Ctrl-click on any material/object and drag.

The top left displays some useful information.  The number of neutrons on the
screen is displayed. In order to accomodate exploring super-critical
configurations, when too many neutrons are on screen (computationally), some
are randomly removed.  The Power indicator tells you how many neutrons there
would have been if none had been artificially removed.  The period is a crude
indicator of "reactor period" where the entire screen is the reactor.
Positive periods indicate growing power, negative for decreasing power.  Small
periods indicate highly super/sub critical configurations while large periods
indicate near-critical (steady state) configurations.

