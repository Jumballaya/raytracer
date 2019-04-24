#ifndef MATERIAL_H
#define MATERIAL_H

#include "raytracer/hitable.h"
#include "raytracer/texture.h"

// Material base class
class Material {
  public:
    Material() {}
    ~Material() {}
    virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const = 0;
    virtual Vec3 emitted(float u, float v, const Vec3& p) const;
};

namespace material {

  // Lambertian diffuse material
  class lambertian : public Material {
    public:
      lambertian(Texture *a);
      ~lambertian() {}

      Texture *albedo;
      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const;
  };

  // Metal shiny (reflective) material
  class metal : public Material {
    public:
      metal(Texture *a, float f) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
      ~metal() {}

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
      ~dielectric() {}

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
      ~diffuse_light() {}

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
      ~isotropic() {}

      Texture *albedo;

      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const {
        scattered = Ray(rec.p, rand_vec_sphere());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
      }
  };

}

#endif
