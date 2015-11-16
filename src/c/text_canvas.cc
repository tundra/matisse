//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "c/stdc.h"

#include "text_canvas.hh"
#include "impl-common.hh"

BEGIN_C_INCLUDES
#include "utils/string-inl.h"
END_C_INCLUDES

using namespace matisse;

TextGlyph::~TextGlyph() {
  if (!is_inline_)
    delete[] data_.ptr_;
}

TextGlyph::TextGlyph(utf8_t encoded) {
  set_contents(encoded);
}

void TextGlyph::set_contents(utf8_t encoded) {
  char *dest = NULL;
  if (encoded.size < kInlineCharCount) {
    // If there's room copy the encoded string into the glyph itself.
    is_inline_ = true;
    dest = data_.inline_;
  } else {
    // If the glyph is too large make a copy.
    is_inline_ = false;
    dest = data_.ptr_ = new char[encoded.size + 1];
  }
  memcpy(dest, encoded.chars, encoded.size);
  dest[encoded.size] = '\0';
}

TextGlyph::TextGlyph(const TextGlyph& that) {
  if (!is_inline_)
    delete[] data_.ptr_;
  set_contents(new_c_string(that.as_utf8()));
}

const TextGlyph &TextGlyph::operator=(const TextGlyph &that) {
  if (!is_inline_)
    delete[] data_.ptr_;
  set_contents(new_c_string(that.as_utf8()));
  return *this;
}

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
  int32_size_t glyph_size = calc_glyph_size();
  for (uint32_t cy = 0; cy < height_; cy++) {
    for (uint32_t cx = 0; cx < width_; cx++) {
      TextGlyph &glyph = at(cx, cy);
      if (glyph.is_empty())
        continue;
      const char *c = glyph.as_utf8();
      uint32_t px = margin_ + cx * glyph_size.width();
      uint32_t py = margin_ + (cy + 1) * glyph_size.height();
      context->draw_text(c, px, py, style_);
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
