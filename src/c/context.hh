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

// A simple struct that may or may not hold a value.
template <typename T>
class Option {
public:
  // Initializes an empty option.
  Option() : has_value_(false) { }

  // Initializes an option with the given value.
  Option(const T &value) : value_(value), has_value_(true) { }

  // Returns this option's value. The option must have a value, otherwise the
  // result is undefined.
  const T &operator*() const { return value_; }

  // Does this option hold a value?
  bool has_value() const { return has_value_; }

private:
  T value_;
  bool has_value_;
};

// Configuration of how to paint when painting. The various settings start out
// "transparent" in the sense that if you don't set say the color, the color
// will not just be some default for this style but will be taken from some
// underlying default style.
class Style {
public:
  void set_color(Color value) { color_ = value; }
  const Option<Color> &color() { return color_; }

  void set_antialias(bool value) { antialias_ = value; }
  const Option<bool> &antialias() { return antialias_; }

private:
  Option<Color> color_;
  Option<bool> antialias_;
};

// Additional styles relating to how to render text.
class TextStyle : public Style {
public:
  void set_text_size(float value) { text_size_ = value; }
  const Option<float> &text_size() { return text_size_; }

  void set_typeface(utf8_t filename) { typeface_ = filename; }
  const Option<utf8_t> &typeface() { return typeface_; }

private:
  Option<float> text_size_;
  Option<utf8_t> typeface_;
};

class GraphicsContext {
public:
  virtual ~GraphicsContext() { }
  virtual int32_size_t size() = 0;
  virtual void draw_text(const char *message, int32_t x, int32_t y, TextStyle *style = NULL) = 0;
  virtual void draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, Style *style = NULL) = 0;
  virtual void clear(Color color) = 0;

  // Sets the default style to use for operations within this context. Any
  // values set in the given style will be used for a given operation if the
  // style passed explicitly to the operation doesn't define that option itself.
  virtual void set_default_style(Style *style) = 0;
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
