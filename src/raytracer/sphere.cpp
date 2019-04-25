#include "../../include/raytracer/sphere.h"

// Sphere
bool Sphere::bounding_box(float t0, float t1, AABB& box) const {
  box = AABB(center - Vec3(radius, radius, radius), center + Vec3(radius, radius, radius));
  return true;
}

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
  Vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius*radius;
  float discriminant = b*b - a*c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(b*b-a*c))/a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
      rec.normal = (rec.p - center) / radius;
      rec.mat = material;
      return true;
    }
    temp = (-b + sqrt(b*b-a*c))/a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      get_sphere_uv((rec.p-center)/radius, rec.u, rec.v);
      rec.normal = (rec.p - center) / radius;
      rec.mat = material;
      return true;
    }
  }
  return false;
}

// Moving Sphere
Vec3 Moving_Sphere::center(float time) const {
  return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

bool Moving_Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
  Vec3 oc = r.origin() - center(r.time());
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius*radius;
  float discriminant = b*b - a*c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(discriminant))/a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      rec.normal = (rec.p - center(r.time())) / radius;
      rec.mat = material;
      return true;
    }
    temp = (-b + sqrt(discriminant)) / a;
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.at(rec.t);
      rec.normal = (rec.p - center(r.time())) / radius;
      rec.mat = material;
      return true;
    }
  }
  return false;
}
