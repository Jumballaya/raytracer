#ifndef MATERIAL_H
#define MATERIAL_H

#include "hitable.h"
#include "texture.h"

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
      metal(Texture *a, float f);
      ~metal() {}

      Texture  *albedo;
      float    fuzz;

      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const;
  };


  // Dielectric transparent material
  class dielectric : public Material {
    public:
      dielectric(Texture *a, float ri);
      ~dielectric() {}

      float ref_idx;
      Texture *albedo;

      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const;
  };

  // Diffuse Light
  class diffuse_light : public Material {
    public:
      diffuse_light(Texture *a);
      ~diffuse_light() {}

      Texture *emit;

      virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const;
      virtual Vec3 emitted(float u, float v, const Vec3& p) const;
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
