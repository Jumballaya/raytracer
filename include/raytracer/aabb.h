#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "hitable.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class AABB {
  public:
    AABB() {}
    AABB(const Vec3& a, const Vec3& b) { _min = a; _max = b; }

    Vec3 _min, _max;

    Vec3 min() { return _min; }
    Vec3 max() { return _max; }

    bool hit(const Ray& r, float t_min, float t_max) const;
};

AABB surrounding_box(AABB box0, AABB box1);

#endif
