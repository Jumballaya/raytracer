#ifndef RECT_H
#define RECT_H

#include "material.h"

// @TODO: Re-write Rect into a single Rect class that can create any
//        rect in space

class Rect_xy: public Hitable  {
    public:
      Rect_xy() {}
      Rect_xy(float _x0, float _x1, float _y0, float _y1, float _k, Material *mat) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), material(mat) {};

      float x0, x1, y0, y1, k;
      Material  *material;

      virtual bool hit(const Ray& r, float t0, float t1, hit_record& rec) const;
      virtual bool bounding_box(float t0, float t1, AABB& box) const {
         box =  AABB(Vec3(x0,y0, k-0.0001), Vec3(x1, y1, k+0.0001));
         return true;
      }
};

class Rect_xz: public Hitable  {
    public:
      Rect_xz() {}
      Rect_xz(float _x0, float _x1, float _z0, float _z1, float _k, Material *mat) : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), material(mat) {};

      float x0, x1, z0, z1, k;
      Material  *material;

      virtual bool hit(const Ray& r, float t0, float t1, hit_record& rec) const;
      virtual bool bounding_box(float t0, float t1, AABB& box) const {
         box =  AABB(Vec3(x0,k-0.0001,z0), Vec3(x1, k+0.0001, z1));
         return true;
      }
};

class Rect_yz: public Hitable {
    public:
      Rect_yz() {}
      Rect_yz(float _y0, float _y1, float _z0, float _z1, float _k, Material *mat) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), material(mat) {};

      float y0, y1, z0, z1, k;
      Material  *material;

      virtual bool hit(const Ray& r, float t0, float t1, hit_record& rec) const;
      virtual bool bounding_box(float t0, float t1, AABB& box) const {
         box =  AABB(Vec3(k-0.0001, y0, z0), Vec3(k+0.0001, y1, z1));
         return true;
      }
};


bool Rect_xy::hit(const Ray& r, float t0, float t1, hit_record& rec) const {
    float t = (k-r.origin().z()) / r.direction().z();
    if (t < t0 || t > t1)
        return false;
    float x = r.origin().x() + t*r.direction().x();
    float y = r.origin().y() + t*r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    rec.mat = material;
    rec.p = r.at(t);
    rec.normal = Vec3(0, 0, 1);
    return true;
}


bool Rect_xz::hit(const Ray& r, float t0, float t1, hit_record& rec) const {
    float t = (k-r.origin().y()) / r.direction().y();
    if (t < t0 || t > t1)
        return false;
    float x = r.origin().x() + t*r.direction().x();
    float z = r.origin().z() + t*r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.mat = material;
    rec.p = r.at(t);
    rec.normal = Vec3(0, 1, 0);
    return true;
}

bool Rect_yz::hit(const Ray& r, float t0, float t1, hit_record& rec) const {
    float t = (k-r.origin().x()) / r.direction().x();
    if (t < t0 || t > t1)
        return false;
    float y = r.origin().y() + t*r.direction().y();
    float z = r.origin().z() + t*r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1)
        return false;
    rec.u = (y-y0)/(y1-y0);
    rec.v = (z-z0)/(z1-z0);
    rec.t = t;
    rec.mat = material;
    rec.p = r.at(t);
    rec.normal = Vec3(1, 0, 0);
    return true;
}

#endif
