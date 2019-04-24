#ifndef CAMERA_H
#define CAMERA_H

#include "raytracer/ray.h"

Vec3 rand_vec_disk();

class Camera {
  public:
    Camera() {}
    Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist, float t0, float t1);

    Vec3 origin;
    Vec3 llc;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    float lens_radius;
    float time0, time1;

    Ray get_ray(float s, float t);
};

#endif
