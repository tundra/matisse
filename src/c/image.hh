//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#ifndef _IMAGE_HH
#define _IMAGE_HH

#include "c/stdc.h"

#include "io/stream.hh"

namespace matisse {

class GraphicsContext;

class int32_size_t {
public:
  int32_size_t(int32_t width, int32_t height)
    : width_(width)
    , height_(height) { }
  int32_t width() { return width_; }
  int32_t height() { return height_; }
private:
  int32_t width_;
  int32_t height_;
};

// A "dumb" bitmap image.
class Bitmap {
public:
  class Data;
  Bitmap();
  ~Bitmap();

  // Reads the contents of a png image from the given stream.
  bool read_from_png(tclib::InStream *in);

  bool compare(Bitmap *that);

  // Returns a new context for drawing on this bitmap.
  GraphicsContext *new_context();

  // Returns the size of this image.
  int32_size_t size();

private:
  Data *data_;
};

} // namespace renderer

#endif // _IMAGE_HH
