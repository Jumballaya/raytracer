/***
 *
 *
 * Scene Script Program
 *
 *
 */
#ifndef PROGRAM_H
#define PROGRAM_H

#include <string.h>

#include "tokens.h"
#include "environment.h"

#include "../raytracer/scene.h"
#include "../raytracer/camera.h"
#include "../raytracer/hitable_list.h"
#include "../raytracer/sphere.h"

struct camera_data {
  Vec3 lookfrom;
  Vec3 lookat;
  Vec3 vup;
  float vfov;
  float aperature;
  float focus_dist;
};

class Program {
  public:
    Program();

    int len;
    int nx, ny, ns;
    bool lights;
    float t0, t1;
    camera_data  cam_data;
    Scene        scene;
    Camera       cam;
    Hitable_List world;

    // Interact with the raytracer API
    void addSphere(Sphere* s);
    void alignCamera();
    void setCamera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aperture, float focus_dist);
    void setMacro(std::string macro, float value);
    void setScene();
    void run();
};

#endif
