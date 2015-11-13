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
  TextGlyph(code_point_t code_point) : code_point_(code_point) { }
  TextGlyph(char chr) : code_point_(chr) { }
  TextGlyph() : code_point_(0) { }
  char to_char() { return code_point_ ? static_cast<char>(code_point_) : ' '; }

private:
  code_point_t code_point_;
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
  void set_margin(uint32_t margin) { margin_ = margin; }

private:
  // The total number of glyphs.
  size_t glyph_count() { return width_ * height_; }

  // The size of an individual glyph.
  int32_size_t calc_glyph_size();

  uint32_t width_;
  uint32_t height_;
  uint32_t margin_;
  TextStyle *style_;
  TextGlyph *glyphs_;
};

} // namespace matisse

#endif // _TEXT_CANVAS_HH
