#ifndef STREAM_SRC_H_
#define STREAM_SRC_H_

#include <map>
#include <set>

#include "phys/object.h"
#include "phys/system.h"

namespace phys {

static const double kNPS = 3e3;

enum Dir {N, S, E, W};

class StreamSource : public Object {
 public:
  StreamSource() {};

  StreamSource(const StreamSource* f) : Object(f) { }

  Dir dir(int x, int y) {
    Rect r = rect();
    bool down_left = y - r.y > (r.h / r.w) * (x - r.x);
    bool up_left = y - (r.y +r.h) < -(r.h / r.w) * (x - r.x);
    if (down_left && up_left)
      return W;
    if (down_left && !up_left)
      return S;
    if (!down_left && up_left)
      return N;
    // else (!down_left && !up_left)
    return E;
  }

  virtual StreamSource* clone() { return new StreamSource(&(*this)); };

  virtual bool OnClick(int x, int y) {
    Dir d = dir(x, y);
    if (streaming_.find(d) != streaming_.end()) {
      streaming_.erase(d);
    } else {
      streaming_.insert(d);
    }
    return true;
  }

  std::pair<int, int> Coords(Dir d, int step, int total) {
    Rect r = rect();
    double beamfrac = 0.25;
    double beamw = beamfrac * r.w;
    double beamh = beamfrac * r.h;
    double frac  = (double)step / total;
    int x, y;
    switch (d) {
      case N:
        x = (int)(r.x + 0.5 * (r.w - beamw) + frac * beamw);
        y = r.y;
        break;
        
      case S:
        x = (int)(r.x + 0.5 * (r.w - beamw) + frac * beamw);
        y = r.y + r.h;
        break;
        
      case E:
        x = r.x + r.w;
        y = (int)(r.y + 0.5 * (r.h - beamh) + frac * beamh);
        break;

      case W:
        x = r.x;
        y = (int)(r.y + 0.5 * (r.h - beamh) + frac * beamh);
        break;
    }
    return std::make_pair(x, y);
  }
  
  std::pair<int, int> Speed(Dir d) {
    int nom = phys::Neutron::kNomSpeed;
    int vx, vy;
    switch (d) {
      case N:
        vx = 0;
        vy = -nom;
        break;
        
      case S:
        vx = 0;
        vy = nom;
        break;
        
      case E:
        vx = nom;
        vy = 0;
        break;
        
      case W:
        vx = -nom;
        vy = 0;
        break;
    }
    return std::make_pair(vx, vy);
  }
  
  virtual void Tick(double deltat, System* sys, std::vector<Neutron*> neutrons) {
    int n = (int) kNPS * deltat;
    std::set<Dir>::iterator it;
    for (it = streaming_.begin(); it != streaming_.end(); ++it) {
      phys::Neutron::Pop ns;
      std::pair<int, int> speed = Speed(*it);
      for (int i = 0; i < n; ++i) {
        std::pair<int, int> coords = Coords(*it, i, n);
        ns.push_back(phys::Neutron(coords.first, coords.second,
                                   speed.first, speed.second));
      }
      sys->AddNeutrons(ns);
    }
  }

 private:
  std::set<Dir> streaming_;
};

}

#endif
