#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "parser.h"
#include "raytracer/scene.h"
#include "raytracer/sphere.h"

Scene build_scene() {
  Vec3 lookfrom(10, 2, 10);
  Vec3 lookat(0,0,0);
  float dist_to_focus = 10.0;
  float aperture = 0.0;
  float vfov = 40.0;

  int nx = 800;
  int ny = 400;

  Camera c(lookfrom, lookat, Vec3(0,1,0), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);
  Material *ground = new material::metal(new texture::constant(Vec3(0.4, 0.4, 0.4)), 0.1);

  Hitable *world = new Sphere(Vec3(0,0,0), 2, ground);

  return Scene(c, world, 400, 400, 5);
}

#endif
