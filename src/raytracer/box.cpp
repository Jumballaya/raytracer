#include "raytracer/box.h"

Box::Box(const Vec3& p0, const Vec3& p1, Material *material) {
  pmin = p0;
  pmax = p1;

  Hitable **lst = new Hitable*[6];
  lst[0] = new Rect_xy(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material);
  lst[1] = new Flip_Normals(new Rect_xy(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material));
  lst[2] = new Rect_xz(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material);
  lst[3] = new Flip_Normals(new Rect_xz(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material));
  lst[4] = new Rect_yz(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material);
  lst[5] = new Flip_Normals(new Rect_yz(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material));

  list = new Hitable_List(lst,6);
};

bool Box::hit(const Ray& r, float t0, float t1, hit_record& rec) const {
  return list->hit(r, t0, t1, rec);
}
