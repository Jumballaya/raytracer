#ifndef PERLIN_H
#define PERLIN_H

#include "vec.h"

class perlin {
  public:
    Vec3 *ranvec;
    float *ranfloat;

    int *perm_x;
    int *perm_y;
    int *perm_z;

    float hash_noise(const Vec3& p) const;
    float noise(const Vec3& p) const;
    float turb(const Vec3& p, int depth) const;
};

#endif
