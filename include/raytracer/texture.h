#ifndef TEXTURE_H
#define TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"

#include "perlin.h"

class Texture {
  public:
    virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};

namespace texture {
  class constant: public Texture {
    public:
      constant() {}
      constant(Vec3 c): color(c) {}

      Vec3 color;

      virtual Vec3 value(float u, float v, const Vec3& p) const {
        return color;
      }
  };

  class checker : public Texture {
    public:
      checker() {}
      checker(Texture *t0, Texture *t1): even(t0), odd(t1) { }

      Texture *even;
      Texture *odd;

      virtual Vec3 value(float u, float v, const Vec3& p) const {
        float sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
        if (sines < 0) return odd->value(u, v, p);
        else return even->value(u, v, p);
      }
  };

  class noise : public Texture {
    public:
      noise() {}
      noise(float sc, int t) : scale(sc), type(t) {}

      perlin n;
      float scale;
      int   type;

      virtual Vec3 value(float u, float v, const Vec3& p) const {
        switch (type) {
          case 1:
            return Vec3(1,1,1)*0.5*(1 + n.turb(scale * p));
          case 2:
            return Vec3(1,1,1)*n.turb(scale * p);
          case 3:
            return Vec3(1,1,1)*0.5*(1 + sin(scale*p.x() + 5*n.turb(scale*p)));
          default:
            return Vec3(1,1,1)*n.hash_noise(scale * p);
        }
      }
  };

  class image : public Texture {
    public:
      image() {}
      image(const char *p) : path(p) {
        data = stbi_load(path, &nx, &ny, &nn, 0);
      }

      const char *path;
      unsigned char *data;
      int nx, ny, nn;

      virtual Vec3 value(float u, float v, const Vec3& p) const;
  };

  Vec3 image::value(float u, float v, const Vec3& p) const {
     int i = (  u)*nx;
     int j = (1-v)*ny-0.001;
     if (i < 0) i = 0;
     if (j < 0) j = 0;
     if (i > nx-1) i = nx-1;
     if (j > ny-1) j = ny-1;
     float r = int(data[3*i + 3*nx*j]  ) / 255.99;
     float g = int(data[3*i + 3*nx*j+1]) / 255.99;
     float b = int(data[3*i + 3*nx*j+2]) / 255.99;
     return Vec3(r, g, b);
  }
}


#endif
