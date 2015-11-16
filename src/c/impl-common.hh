//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

// Private definitions that are shared between implementation files in the
// skie bridge layer.

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
  // Create a context for the given canvas. Refs the context so it's safe to
  // unref for the caller afterwards.
  SkiaGraphicsContext(SkCanvas *canvas);
  ~SkiaGraphicsContext();

  // Converts a matisse color to a skia one.
  static SkColor to_sk_color(Color color);

  // Converts a matisse style to a skia paint.
  static void style_to_sk_paint(Style *style, SkPaint *out);

  // Converts a matisse text style to a skia paint.
  static void text_style_to_sk_paint(TextStyle *style, SkPaint *out);

  virtual int32_size_t size();
  virtual void draw_text(const char *message, scalar_t x, scalar_t y, TextStyle *style);
  virtual void draw_line(scalar_t x0, scalar_t y0, scalar_t x1, scalar_t y1, Style *style);
  virtual void clear(Color color);
  virtual void set_default_style(Style *style);

private:
  SkAutoTUnref<SkCanvas> canvas_;
  SkPaint sk_default_paint_;
};

// Misc convenient utilities.
class Utils {
public:
  // Reads the full contents of the given stream into the given byte vector.
  static bool read_in_stream(tclib::InStream *in, std::vector<byte_t> *out);
};

}
