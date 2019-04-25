#ifndef HITABLE_H
#define HITABLE_H

#include <float.h>
#include "aabb.h"
#include "material.h"

struct hit_record {
  float    t;
  float    u;
  float    v;
  Vec3     p;
  Vec3     normal;
  Material *mat;
};

void get_sphere_uv(const Vec3& p, float& u, float& v);

class Hitable {
  public:
    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(float t0, float t1, AABB& b) const = 0;
};

class Flip_Normals : public Hitable {
  public:
    Flip_Normals(Hitable *p) : object(p) {}

    Hitable *object;
    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& b) const;
};

class Translate : public Hitable {
  public:
    Translate(Hitable *p, const Vec3& displacement) : object(p), offset(displacement) {}
    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const;
    Hitable *object;
    Vec3 offset;
};

class Rotate_Y : public Hitable {
  public:
    Rotate_Y(Hitable *p, float angle);

    Hitable *object;
    float sin_theta;
    float cos_theta;
    bool has_box;
    AABB bbox;

    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const;
};

#endif
