#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.h"

class Hitable_List: public Hitable {
  public:
    Hitable_List() { list_size = 0; list = (Hitable**) malloc(sizeof(Hitable*) * 20); };
    Hitable_List(Hitable **l, int n) : list(l), list_size(n) { }

    Hitable **list;
    int     list_size;

    virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const;
    virtual bool bounding_box(float t0, float t1, AABB& b) const;
    void append(Hitable* object) { list[list_size] = object; list_size++; }
};

bool Hitable_List::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
  hit_record temp_rec;
  bool hit_anything = false;
  double closest_so_far = t_max;

  for (int i = 0; i < list_size; i++) {
    if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }
  return hit_anything;
}

bool Hitable_List::bounding_box(float t0, float t1, AABB& b) const {
  if (list_size < 1) return false;
  AABB temp_box;
  bool first_true = list[0]->bounding_box(t0, t1, temp_box);
  if (!first_true) return false;
  b = temp_box;
  for (int i = 1; i < list_size; i++) {
    if (!(list[0]->bounding_box(t0, t1, temp_box))) return false;
    b = surrounding_box(b, temp_box);
  }
  return true;
}

#endif
