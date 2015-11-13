//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "c/stdc.h"

#include "text_canvas.hh"
#include "impl-common.hh"

using namespace matisse;

TextCanvas::TextCanvas(uint32_t width, uint32_t height)
  : width_(width)
  , height_(height)
  , margin_(0)
  , style_(NULL)
  , glyphs_(NULL) {
  CHECK_REL("zero width", 0, <, width);
  CHECK_REL("zero height", 0, <, height);
  glyphs_ = new TextGlyph[glyph_count()];
}

TextCanvas::~TextCanvas() {
  delete[] glyphs_;
}

TextGlyph &TextCanvas::at(uint32_t x, uint32_t y) {
  CHECK_REL("x bound", x, <, width_);
  CHECK_REL("y bound", y, <, height_);
  return glyphs_[width_ * y + x];
}

void TextCanvas::render(GraphicsContext *context) {
  context->clear(Color::white());
  int32_size_t glyph = calc_glyph_size();
  for (uint32_t y = 0; y < height_; y++) {
    for (uint32_t x = 0; x < width_; x++) {
      char c[2] = {at(x, y).to_char(), '\0'};
      context->draw_text(c, margin_ + x * glyph.width(), margin_ + (y + 1) * glyph.height(), style_);
    }
  }
}

int32_size_t TextCanvas::calc_canvas_size() {
  int32_size_t glyph = calc_glyph_size();
  return int32_size_t(width_ * glyph.width() + 2 * margin_, height_ * glyph.height() + 2 * margin_);
}

int32_size_t TextCanvas::calc_glyph_size() {
  SkPaint paint;
  SkiaGraphicsContext::text_style_to_sk_paint(style_, &paint);
  SkRect bounds = SkRect::MakeEmpty();
  // This is a little silly but okay, it works.
  static const char *kChars = "Xmky";
  for (const char *p = kChars; *p != '\0'; p++) {
    SkRect char_bounds;
    paint.measureText(p, 1, &char_bounds);
    bounds.growToInclude(char_bounds.width(), char_bounds.height());
  }
  static const int32_t kGlyphMargin = 1;
  return int32_size_t((int32_t) bounds.width() + kGlyphMargin,
      (int32_t) bounds.height() + kGlyphMargin);
}
