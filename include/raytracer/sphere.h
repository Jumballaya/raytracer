#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.h"

/**
 *
 * Sphere Object
 *
 */
class Sphere: public Hitable  {
  public:
    Sphere() {}
    Sphere(Vec3 cen, float r, Material *m) : center(cen), radius(r), material(m)  {};

    Vec3 center;
    float radius;
    Material *material;

    virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const;
};


/**
 *
 * Moving Sphere Object
 *
 */
class Moving_Sphere: public Hitable {
  public:
    Moving_Sphere() {}
    Moving_Sphere(Vec3 cen0, Vec3 cen1, float t0, float t1, float r, Material *m) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), material(m) {};

    Vec3 center0, center1;
    float time0, time1;
    float radius;
    Material *material;

    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
    Vec3 center(float time) const;
};

#endif
