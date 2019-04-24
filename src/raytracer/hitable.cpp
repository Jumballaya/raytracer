#include "raytracer/hitable.h"

void get_sphere_uv(const Vec3& p, float& u, float& v) {
  float phi = atan2(p.z(), p.x());
  float theta = asin(p.y());
  u = 1-(phi + M_PI) / (2*M_PI);
  v = (theta + M_PI/2) / M_PI;
}

bool Translate::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
  Ray moved_r(r.origin() - offset, r.direction(), r.time());
  if (object->hit(moved_r, t_min, t_max, rec)) {
    rec.p += offset;
    return true;
  }
  return false;
}

bool Translate::bounding_box(float t0, float t1, AABB& box) const {
  if (object->bounding_box(t0, t1, box)) {
    box = AABB(box.min() + offset, box.max()+offset);
    return true;
  }
  return false;
}

bool Flip_Normals::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
  if (object->hit(r, t_min, t_max, rec)) {
    rec.normal = -rec.normal;
    return true;
  }
  return false;
}

bool Flip_Normals::bounding_box(float t0, float t1, AABB& b) const {
  return object->bounding_box(t0, t1, b);
}

Rotate_Y::Rotate_Y(Hitable *p, float angle) : object(p) {
  float radians = (M_PI / 180.) * angle;
  sin_theta = sin(radians);
  cos_theta = cos(radians);
  has_box = object->bounding_box(0, 1, bbox);
  Vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
  Vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        float x = i*bbox.max().x() + (1-i)*bbox.min().x();
        float y = j*bbox.max().y() + (1-j)*bbox.min().y();
        float z = k*bbox.max().z() + (1-k)*bbox.min().z();
        float newx = cos_theta*x + sin_theta*z;
        float newz = -sin_theta*x + cos_theta*z;
        Vec3 tester(newx, y, newz);
        for (int c = 0; c < 3; c++) {
          if (tester[c] > max[c]) max[c] = tester[c];
          if (tester[c] < min[c]) min[c] = tester[c];
        }
      }
    }
  }
  bbox = AABB(min, max);
}

bool Rotate_Y::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
  Vec3 origin = r.origin();
  Vec3 direction = r.direction();
  origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
  origin[2] =  sin_theta*r.origin()[0] + cos_theta*r.origin()[2];
  direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
  direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];
  Ray rotated_r(origin, direction, r.time());
  if (object->hit(rotated_r, t_min, t_max, rec)) {
    Vec3 p = rec.p;
    Vec3 normal = rec.normal;
    p[0] = cos_theta*rec.p[0] + sin_theta*rec.p[2];
    p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];
    normal[0] = cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];
    rec.p = p;
    rec.normal = normal;
    return true;
  }
  else return false;
}

bool Rotate_Y::bounding_box(float t0, float t1, AABB& box) const {
  box = bbox;
  return has_box;
}
