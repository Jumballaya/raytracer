#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "hitable_list.h"
#include "material.h"
#include <math.h>

class Scene {
  public:
    Scene() {
      lights_on = true;
    }
    Scene(Camera c, Hitable *w, int x, int y, int s) : cam(c), world(w), nx(x), ny(y), ns(s) {
      lights_on = true;
    }

    Camera cam;
    Hitable *world;
    int nx, ny, ns;
    bool lights_on;

    bool toggle_lights();
    Vec3 draw_background(const Ray& r);
    std::string render();

  private:
    Vec3 color(const Ray& r, int depth);
};

#endif
