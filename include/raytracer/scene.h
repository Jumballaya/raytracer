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

    bool toggle_lights() {
      lights_on = !lights_on;
      return lights_on;
    }

    Vec3 draw_background(const Ray& r) {
      if (lights_on) {
        return Vec3(0,0,0);
      }
      Vec3 unit_dir = unit_vector(r.direction());
      float t = 0.5 * (unit_dir.y() + 1.0);
      return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
    }

    std::string render();

  private:
    Vec3 color(const Ray& r, int depth) {
      hit_record rec;
      if (world->hit(r, 0.001, MAXFLOAT, rec)) {
        Ray scattered;
        Vec3 attenuation;
        Vec3 emitted = rec.mat->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered))
          return emitted + attenuation*color(scattered, depth+1);
        else return emitted;
      }
      return draw_background(r);
    }
};


// @TODO: Figure out how C++ readers/writers work to pass in a buffer to write to so I can
//        convert to png/jpeg/gif else where
std::string Scene::render() {
  std::string output;
  // Header
  output += "P3\n";
  output +=  std::to_string(nx);
  output +=  " ";
  output +=  std::to_string(ny);
  output +=  "\n255\n";


  // Body
  for (int j = ny-1; j >= 0; j--) {
    for (int i = 0; i < nx; i++) {
      Vec3 col(0, 0, 0);
      for (int s=0; s < ns; s++) {
        float u = float(i+drand48())/ float(nx);
        float v = float(j+drand48())/ float(ny);
        Ray r = cam.get_ray(u, v);
        col += color(r, 0);
      }
      col /= float(ns);
      col = Vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
      int ir = int(255.99*col[0]);
      int ig = int(255.99*col[1]);
      int ib = int(255.99*col[2]);

      // Write line
      output += std::to_string(ir);
      output += " ";
      output += std::to_string(ig);
      output += " ";
      output += std::to_string(ib);
      output += "\n";
    }
  }

  return output;
}
#endif
