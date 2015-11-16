//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "c/stdc.h"

#include "text_canvas.hh"
#include "impl-common.hh"

BEGIN_C_INCLUDES
#include "utils/alloc.h"
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
  GlyphFontInfo font_info = calc_glyph_font_info();
  for (uint32_t cy = 0; cy < height_; cy++) {
    std::vector<char> line;
    for (uint32_t cx = 0; cx < width_; cx++) {
      TextGlyph &glyph = at(cx, cy);
      if (glyph.is_empty())
        continue;
      utf8_t utf8 = new_c_string(glyph.as_utf8());
      char *chars = const_cast<char*>(utf8.chars);
      line.insert(line.end(), chars, chars + utf8.size);
    }
    line.push_back('\0');
    SkScalar px = margin_;
    SkScalar py = margin_ + font_info.ascent + SkScalar(cy) * font_info.height_per_line;
    context->draw_text(line.data(), px, py, style_);
  }
}

int32_size_t TextCanvas::calc_canvas_size() {
  GlyphFontInfo glyph = calc_glyph_font_info();
  // The width of one row, just the characters.
  scalar_t row_width = scalar_t(width_) * glyph.width_per_glyph;
  // The canvas width which includes margins etc.
  scalar_t canvas_width = row_width + 2 * margin_;
  scalar_t column_height = scalar_t(height_ - 1) * glyph.height_per_line;
  scalar_t canvas_height = column_height + 2 * margin_ + glyph.ascent + glyph.descent;
  return int32_size_t(int32_t(canvas_width), int32_t(canvas_height));
}

GlyphFontInfo TextCanvas::calc_glyph_font_info() {
  SkPaint paint;
  SkiaGraphicsContext::text_style_to_sk_paint(style_, &paint);
  SkPaint::FontMetrics metrics;
  struct_zero_fill(metrics);
  SkScalar height_per_line = paint.getFontMetrics(&metrics);
  SkScalar width_per_glyph = 0;
  paint.getTextWidths("X", 1, &width_per_glyph);
  GlyphFontInfo info = {
      width_per_glyph,
      height_per_line,
      -metrics.fAscent,
      metrics.fDescent
  };
  return info;
}
