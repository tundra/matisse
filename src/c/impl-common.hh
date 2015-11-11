//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "c/stdc.h"

#include "image.hh"

BEGIN_C_INCLUDES
#include "utils/check.h"
#include "utils/log.h"
END_C_INCLUDES

#include "skia-includes.hh"

namespace matisse {

// A graphics context backed by a skia canvas.
class SkiaGraphicsContext : public GraphicsContext {
public:
  // Create a context for the given canvas. Refs the context.
  SkiaGraphicsContext(SkCanvas *canvas);
  ~SkiaGraphicsContext();
  virtual int32_size_t size();
  virtual void draw_text(const char *message, int32_t x, int32_t y);
  virtual void clear();
private:
  SkCanvas *canvas_;
};

}
