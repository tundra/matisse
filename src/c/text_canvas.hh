//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#ifndef _TEXT_CANVAS_HH
#define _TEXT_CANVAS_HH

#include "c/stdc.h"

#include "context.hh"

namespace matisse {

// A unicode code point.
typedef uint64_t code_point_t;

// An individual glyph on the canvas.
class TextGlyph {
public:
  inline TextGlyph(char chr);
  TextGlyph(utf8_t encoded);
  TextGlyph(const TextGlyph&);
  const TextGlyph &operator=(const TextGlyph &);
  inline TextGlyph();
  ~TextGlyph();

  // Returns the utf8-encoding of this glyph.
  inline const char *as_utf8() const;

  // Does this glyph not have contents?
  inline bool is_empty();

  // Does this glyph hold its character data inline?
  bool is_inline() { return is_inline_; }

private:
  // Up to how many characters (including null terminator) will be store within
  // the glyph itself?
  static const size_t kInlineCharCount = 4;

  // Is the character data stored inline within the glyph?
  bool is_inline_;

  // Sets the contents "properly", copying the string if it's too long to store
  // inline.
  void set_contents(utf8_t encoded);

  union {
    // Data stored inline.
    char inline_[kInlineCharCount];
    // Pointer to an external copy owned by this glyph.
    char *ptr_;
  } data_;
};

TextGlyph::TextGlyph(char chr)
  : is_inline_(true) {
  data_.inline_[0] = chr;
  data_.inline_[1] = '\0';
}

TextGlyph::TextGlyph()
  : is_inline_(true) {
  data_.inline_[0] = '\0';
}

const char *TextGlyph::as_utf8() const {
  return is_inline_ ? data_.inline_ : data_.ptr_;
}

bool TextGlyph::is_empty() {
  return as_utf8()[0] == '\0';
}

// Info about the font used to draw glyphs.
struct GlyphFontInfo {
  // With of a glyph, including spacing.
  scalar_t width_per_glyph;
  // Preferred line height.
  scalar_t height_per_line;
  // Distance from the baseline to the top of the text.
  scalar_t ascent;
  // Distance from the baseline to the bottom of the text.
  scalar_t descent;
};

// A text canvas is responsible for rendering a square block of text (that is,
// the contents of the text terminal) on a graphics context.
class TextCanvas {
public:
  TextCanvas(uint32_t width, uint32_t height);
  ~TextCanvas();

  // Returns the full size in pixels of the canvas to draw this on.
  int32_size_t calc_canvas_size();

  // Sets the text style to draw with.
  void set_style(TextStyle *style) { style_ = style; }

  // Returns the glyph at (x, y).
  TextGlyph &at(uint32_t x, uint32_t y);

  // Draws the contents of this canvas on the given context.
  void render(GraphicsContext *context);

  // Sets the empty margin around the text.
  void set_margin(scalar_t margin) { margin_ = margin; }

private:
  // The total number of glyphs.
  size_t glyph_count() { return width_ * height_; }

  // The size of an individual glyph.
  GlyphFontInfo calc_glyph_font_info();

  uint32_t width_;
  uint32_t height_;
  scalar_t margin_;
  TextStyle *style_;
  TextGlyph *glyphs_;
};

} // namespace matisse

#endif // _TEXT_CANVAS_HH
