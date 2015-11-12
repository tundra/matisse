//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#ifndef _CONTEXT_HH
#define _CONTEXT_HH

#include "c/stdc.h"

#include "image.hh"

namespace matisse {

// An RGBA color. These are lightweight so don't worry about passing them by
// value.
class Color {
public:
  // Creates the default empty (black) color.
  Color() : argb_(black().argb()) { }
  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

  // Returns the underlying argb value.
  int32_t argb() { return argb_; }

  // Convenience methods.
  static Color black() { return Color(0, 0, 0); }
  static Color gray(uint8_t amount) { return Color(amount, amount, amount); }
  static Color white() { return Color(255, 255, 255); }
  static Color red() { return Color(255, 0, 0); }
  static Color green() { return Color(0, 255, 0); }
  static Color blue() { return Color(0, 0, 255); }

private:
  int32_t argb_;
};

class Style {
public:
  Style();
  void set_color(Color value) { color_ = value; }
  Color color() { return color_; }

  void set_antialias(bool value) { antialias_ = value; }
  bool antialias() { return antialias_; }

private:
  Color color_;
  bool antialias_;
};

class TextStyle : public Style {
public:
  TextStyle();
  void set_text_size(float value) { text_size_ = value; }
  float text_size() { return text_size_; }

private:
  float text_size_;
};

class GraphicsContext {
public:
  virtual ~GraphicsContext() { }
  virtual int32_size_t size() = 0;
  virtual void draw_text(const char *message, int32_t x, int32_t y, TextStyle *style) = 0;
  virtual void draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, Style *style) = 0;
  virtual void clear(Color color) = 0;
};

class MainWindow {
public:
  virtual ~MainWindow() { }
  virtual void set_title(const char *title) = 0;
};

class Renderer {
public:
  virtual ~Renderer() { }
  virtual void configure(MainWindow *main_window) { }
  virtual void draw(GraphicsContext *context) { }
};

class EntryPoint {
public:
  static Renderer *create_renderer();
};

} // namespace renderer

#endif // _SKIA_MAIN_HH
