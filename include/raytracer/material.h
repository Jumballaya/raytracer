#ifndef MATERIAL_H
#define MATERIAL_H

#include "hitable.h"
#include "texture.h"

// Material base class
class Material {
  public:
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0;
    virtual Vec3 emitted(float u, float v, const Vec3& p) const {
      return Vec3(0,0,0);
    }
};

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

  // Lambertian diffuse material
  class lambertian : public Material {
    public:
      lambertian(Texture *a) : albedo(a) {}

      Texture *albedo;

      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const  {
        Vec3 target = rec.p + rec.normal + rand_vec_sphere();
        scattered = Ray(rec.p, target-rec.p, r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
      }
  };

  // Metal shiny (reflective) material
  class metal : public Material {
    public:
      metal(Texture *a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }

      Texture  *albedo;
      float    fuzz;

      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz*rand_vec_sphere(), r_in.time());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return (dot(scattered.direction(), rec.normal) > 0);
      }
  };


  // Dielectric transparent material
  class dielectric : public Material {
    public:
      dielectric(Texture *a, float ri) : ref_idx(ri), albedo(a) {}

      float ref_idx;
      Texture *albedo;

      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
        Vec3 outward_normal;
        Vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        Vec3 refracted;
        float reflect_prob;
        float cosine;

        if (dot(r_in.direction(), rec.normal) > 0) {
          outward_normal = -rec.normal;
          ni_over_nt = ref_idx;
          cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
          cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
        } else {
          outward_normal = rec.normal;
          ni_over_nt = 1.0 / ref_idx;
          cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted))
          reflect_prob = schlick(cosine, ref_idx);
        else
          reflect_prob = 1.0;

        if (drand48() < reflect_prob)
          scattered = Ray(rec.p, reflected);
        else
          scattered = Ray(rec.p, refracted);

        return true;
      }
  };

  // Diffuse Light
  class diffuse_light : public Material {
    public:
      diffuse_light(Texture *a) : emit(a) {}

      Texture *emit;

      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
        return false;
      };

      virtual Vec3 emitted(float u, float v, const Vec3& p) const {
        return emit->value(u, v, p);
      }
  };

  // Isotropic
  class isotropic : public Material {
    public:
      isotropic(Texture *a) : albedo(a) {}

      Texture *albedo;

      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
        scattered = Ray(rec.p, rand_vec_sphere());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
      }
  };

}

#endif
