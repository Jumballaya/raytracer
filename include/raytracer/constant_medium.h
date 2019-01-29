#ifndef CONSTANT_MEDIUM_H
#define CONSTANT_MEDIUM_H

#include <float.h>
#include "hitable.h"
#include "texture.h"
#include "material.h"

class Constant_Medium : public Hitable  {
  public:
    Constant_Medium(Hitable *b, float d, Texture *a) : boundary(b), density(d) {
      phase_function = new material::isotropic(a);
    }

    Hitable *boundary;
    float density;
    Material *phase_function;

    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& box) const {
      return boundary->bounding_box(t0, t1, box);
    }
};

bool Constant_Medium::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    bool db = (drand48() < 0.00001);
    db = false;
    hit_record rec1, rec2;
    if (boundary->hit(r, -FLT_MAX, FLT_MAX, rec1)) {
        if (boundary->hit(r, rec1.t+0.0001, FLT_MAX, rec2)) {
    if (db) std::cerr << "\nt0 t1 " << rec1.t << " " << rec2.t << "\n";
            if (rec1.t < t_min)
                rec1.t = t_min;
            if (rec2.t > t_max)
                rec2.t = t_max;
            if (rec1.t >= rec2.t)
                return false;
            if (rec1.t < 0)
                rec1.t = 0;
            float distance_inside_boundary = (rec2.t - rec1.t)*r.direction().length();
            float hit_distance = -(1/density)*log(drand48());
            if (hit_distance < distance_inside_boundary) {
            if (db) std::cerr << "hit_distance = " <<  hit_distance << "\n";
                rec.t = rec1.t + hit_distance / r.direction().length();
            if (db) std::cerr << "rec.t = " <<  rec.t << "\n";
                rec.p = r.at(rec.t);
            if (db) std::cerr << "rec.p = " <<  rec.p << "\n";
                rec.normal = Vec3(1,0,0);  // arbitrary
                rec.mat = phase_function;
                return true;
            }
        }
    }
    return false;
}

#endif
