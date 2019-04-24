#ifndef BVH_H
#define BVH_H

#include "raytracer/hitable.h"

class BVH_Node: public Hitable {
  BVH_Node() {}
  BVH_Node(Hitable **l, int n, float time0, float time1);

  Hitable *left;
  Hitable *right;
  AABB box;

  virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
  virtual bool bounding_box(float t0, float t1, AABB& box) const;
};

#endif
