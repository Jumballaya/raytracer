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

bool AABB::hit(const Ray& r, float t_min, float t_max) const {
  for (int a = 0; a < 3; a++) {
    float i1 = (_min[a] - r.origin()[a]) / r.direction()[a];
    float i2 = (_max[a] - r.origin()[a]) / r.direction()[a];
    float t0 = ffmin(i1, i2);
    float t1 = ffmax(i1, i2);
    t_min = ffmax(t0, t_min);
    t_max = ffmin(t1, t_max);
    if (t_max <= t_min) return false;
  }
  return true;
}

AABB surrounding_box(AABB box0, AABB box1) {
  Vec3 small(
    fmin(box0.min().x(), box1.min().x()),
    fmin(box0.min().y(), box1.min().y()),
    fmin(box0.min().z(), box1.min().z())
  );
  Vec3 big(
    fmax(box0.max().x(), box1.max().x()),
    fmax(box0.max().y(), box1.max().y()),
    fmax(box0.max().z(), box1.max().z())
  );
  return AABB(small,big);
}


#endif
