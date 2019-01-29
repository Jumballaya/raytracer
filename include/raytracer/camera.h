#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

Vec3 rand_vec_disk() {
  Vec3 p;
  do {
      p = 2.0*Vec3(drand48(),drand48(),0) - Vec3(1,1,0);
  } while (dot(p,p) >= 1.0);
  return p;
}

class Camera {
  public:
    Camera() {}
    Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1) {
      time0 = t0;
      time1 = t1;
      lens_radius = aperture / 2;
      float theta = vfov*M_PI/180;
      float half_height = tan(theta/2);
      float half_width = aspect * half_height;
      origin = lookfrom;
      w = unit_vector(lookfrom - lookat);
      u = unit_vector(cross(vup, w));
      v = cross(w, u);
      llc = origin  - half_width*focus_dist*u -half_height*focus_dist*v - focus_dist*w;
      horizontal = 2*half_width*focus_dist*u;
      vertical = 2*half_height*focus_dist*v;
    }

    Vec3 origin;
    Vec3 llc;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    float lens_radius;
    float time0, time1;

    Ray get_ray(float s, float t) {
      Vec3 rd = lens_radius*rand_vec_disk();
      Vec3 offset = u * rd.x() + v * rd.y();
      float time = time0 + drand48()*(time1 - time0);
      return Ray(origin + offset, llc + s*horizontal + t*vertical - origin - offset, time);
    }
};

#endif
