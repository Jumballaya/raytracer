#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "raytracer/hitable.h"

class Hitable_List: public Hitable {
  public:
    Hitable_List();
    Hitable_List(Hitable **l, int n);

    Hitable **list;
    int     list_size;

    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& b) const;
    void append(Hitable* object);
};

#endif
