#ifndef BOX_H
#define BOX_H

#include "raytracer/hitable_list.h"
#include "raytracer/rect.h"

class Box : public Hitable {
  public:
    Box() {}
    Box(const Vec3& p0, const Vec3& p1, Material *material);

    Vec3 pmin, pmax;
    Hitable *list;

    virtual bool hit(const Ray& r, float t0, float t1, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& b) const {
      b = AABB(pmin, pmax);
      return true;
    }
};

#endif
