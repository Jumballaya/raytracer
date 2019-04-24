#include "raytracer/material.h"

Vec3 Material::emitted(float u, float v, const Vec3& p) const {
  return Vec3(0,0,0);
}

namespace material {
  // Reflect returns a vector from a 2-vector 'collision'
  Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v,n) * n;
  }

  // Refract returns a bool if the vectors refract on 'collision'
  bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) {
    Vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
    if (discriminant > 0) {
      refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
      return true;
    }
    return false;
  }

  // Schlick equation to approximate reflectiveness of glass
  float schlick(float cosine, float ref_idx) {
    float r0 = (1-ref_idx) / (1 + ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1-cosine), 5);
  }

  // Create a random vector in a sphere
  Vec3 rand_vec_sphere() {
    Vec3 p;
    do {
      p = 2.0 * Vec3(drand48(), drand48(), drand48()) - Vec3(1,1,1);
    } while (p.squared_length() >= 1.0);
    return p;
  }

  lambertian::lambertian(Texture *a) : albedo(a) {}

  bool lambertian::scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const  {
    Vec3 target = rec.p + rec.normal + rand_vec_sphere();
    scattered = Ray(rec.p, target-rec.p, r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
  }
}
