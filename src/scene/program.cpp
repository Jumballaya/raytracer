#include "scene/program.h"

Program::Program() {
  nx = 640;
  ny = 480;
  ns = 10;
  t0 = 0;
  t1 = 1;
  lights = true;
  len = 0;
  world = Hitable_List();
}

void Program::setScene() {
  scene = Scene(cam, &world, nx, ny, ns);
  if (!lights) scene.toggle_lights();
}

void Program::run() {
  alignCamera();
  setScene();
  std::cout << scene.render() << std::endl;
}

void Program::addSphere(Sphere* s) {
  world.append(s);
}

void Program::setCamera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aperature, float focus_dist) {
  cam_data.lookfrom = lookfrom;
  cam_data.lookat = lookat;
  cam_data.vup = vup;
  cam_data.vfov = vfov;
  cam_data.aperature = aperature;
  cam_data.focus_dist = focus_dist;
  cam = Camera(lookfrom, lookat, vup, vfov, float(nx)/float(ny), aperature, focus_dist, t0, t1);
}

void Program::alignCamera() {
  Camera cam(cam_data.lookfrom, cam_data.lookat, cam_data.vup, cam_data.vfov, float(nx)/float(ny), cam_data.aperature, cam_data.focus_dist, t0, t1);
}

void Program::setMacro(std::string macro, float value) {
  if (strcmp(macro.c_str(), "set_nx") == 0) {
    nx = value;
  }
  if (strcmp(macro.c_str(), "set_ny") == 0) {
    ny = value;
  }
  if (strcmp(macro.c_str(), "set_ns") == 0) {
    ns = value;
  }
  if (strcmp(macro.c_str(), "lights") == 0) {
    if (value > 0) lights = true;
    else lights = false;
  }
  if (strcmp(macro.c_str(), "time_start") == 0) {
    t0 = value;
  }
  if (strcmp(macro.c_str(), "time_end") == 0) {
    t1 = value;
  }
}

