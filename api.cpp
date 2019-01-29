#include <iostream>

// Objects
#include "include/raytracer/sphere.h"
#include "include/raytracer/box.h"
#include "include/raytracer/rect.h"
#include "include/raytracer/constant_medium.h"
#include "include/raytracer/camera.h"
#include "include/raytracer/bvh.h"

// Hitable
#include "include/raytracer/hitable.h"
#include "include/raytracer/hitable_list.h"

// Others
#include "include/raytracer/material.h"
#include "include/raytracer/scene.h"

Hitable *cornell_box() {
  Hitable **list = new Hitable*[8];
  int i = 0;
  Material *red = new material::lambertian(new texture::constant(Vec3(0.65, 0.05, 0.05)));
  Material *white = new material::lambertian(new texture::constant(Vec3(0.73, 0.73, 0.73)));
  Material *green = new material::lambertian(new texture::constant(Vec3(0.12, 0.45, 0.15)));
  Material *light = new material::diffuse_light(new texture::constant(Vec3(15, 15, 15)));
  Material *metal = new material::metal(new texture::constant(Vec3(0.9, 0.9, 0.9)), 0.0);
  Material *glass = new material::dielectric(new texture::constant(Vec3(1,1,1)), 1.5);

  list[i++] = new Flip_Normals(new Rect_yz(0, 555, 0, 555, 555, green));
  list[i++] = new Rect_yz(0, 555, 0, 555, 0, red);
  list[i++] = new Rect_yz(213, 343, 227, 332, 554, light);
  list[i++] = new Flip_Normals(new Rect_xz(0, 555, 0, 555, 555, white));
  list[i++] = new Rect_xz(0, 555, 0, 555, 0, white);
  list[i++] = new Flip_Normals(new Rect_xy(0, 555, 0, 555, 555, white));
  list[i++] = new Translate(new Rotate_Y(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(130,0,65));
  list[i++] = new Translate(new Rotate_Y(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white),  15), Vec3(265,0,295));

  list[i++] = new Sphere(Vec3(275.5, 275.5, 275.5), 75, metal);
  return new Hitable_List(list,i);
}

Hitable *earth() {
  Hitable **list = new Hitable*[3];
  Material *m_earth =  new material::lambertian(new texture::image("assets/earthmap.jpg"));
  Material *m_moon =  new material::lambertian(new texture::image("assets/moonmap.png"));
  Material *m_ground = new material::metal(new texture::constant(Vec3(0.4, 0.4, 0.4)), 0.1);

  list[0] = new Sphere(Vec3(0,0,0), 2, m_earth);
  list[1] = new Sphere(Vec3(-1,2,0), 0.66, m_moon);
  list[2] = new Sphere(Vec3(0,-102.5,0), 100, m_ground);

  return new Hitable_List(list, 3);
}

Camera make_camera(int nx, int ny) {
  Vec3 lookfrom(10, 2, 10);
  Vec3 lookat(0,0,0);
  float dist_to_focus = 10.0;
  float aperture = 0.0;
  float vfov = 40.0;
  Camera cam(lookfrom, lookat, Vec3(0,1,0), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);
  return cam;
}

Camera make_cornell_camera(int nx, int ny) {
  Vec3 lookfrom(278, 278, -800);
  Vec3 lookat(278,278,0);
  float dist_to_focus = 10.0;
  float aperture = 0.0;
  float vfov = 40.0;
  Camera cam(lookfrom, lookat, Vec3(0,1,0), vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);
  return cam;
}

int main() {
  int nx = 800;
  int ny = 800;
  int ns = 1;

  Camera cam = make_camera(nx, ny);
  //Camera cam = make_cornell_camera(nx, ny);
  Hitable *world = earth();
  //Hitable *world = cornell_box();

  Scene s(cam, world, nx, ny, ns);
  s.toggle_lights();
  std::cout << s.render() << std::endl;

  return 0;
}
