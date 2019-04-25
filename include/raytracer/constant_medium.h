#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include <float.h>
#include "hitable.h"
#include "texture.h"
#include "material.h"

class Constant_Medium : public Hitable  {
  public:
    Constant_Medium(Hitable *b, float d, Texture *a);

    Hitable *boundary;
    float density;
    Material *phase_function;

    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const {
      return boundary->bounding_box(t0, t1, box);
    }
};

#endif
