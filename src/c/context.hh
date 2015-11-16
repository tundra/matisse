//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#ifndef _CONTEXT_HH
#define _CONTEXT_HH

#include "c/stdc.h"

#include "image.hh"

class SkTypeface;

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
  const T &operator*() const { return value(); }

  // Alias for *.
  const T &value() const { return value_; }

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

// A typeface. The underlying data is reasonably heavy-weight but is ref-counted
// so you can pass them around by value and the underlying data will be shared
// and disposed only after the last reference goes away.
class Typeface {
public:
  Typeface() : sk_typeface_(NULL) { }

  // Create a matisse typeface based on an sk one. The typeface is passed
  // ownership of the sk-typeface so if you want to retain ownership you need
  // to explicitly ref the sk-typeface.
  Typeface(SkTypeface *sk_typeface);

  // Implements the ref-counting behavior.
  Typeface(const Typeface &that);
  ~Typeface();
  const Typeface &operator=(const Typeface &that);

  // Reads and returns a typeface from the given input stream.
  static Typeface read(tclib::InStream *in);

  // Returns the underlying skia typeface.
  SkTypeface *sk_typeface() const { return sk_typeface_; }

private:
  SkTypeface *sk_typeface_;
};

// Additional styles relating to how to render text.
class TextStyle : public Style {
public:
  void set_text_size(float value) { text_size_ = value; }
  const Option<float> &text_size() { return text_size_; }

  void set_typeface(Typeface typeface) { typeface_ = typeface; }
  const Option<Typeface> &typeface() { return typeface_; }

private:
  Option<float> text_size_;
  Option<Typeface> typeface_;
};

// The scalar type used to specify coordinates when using a graphics context.
typedef float scalar_t;

class GraphicsContext {
public:
  virtual ~GraphicsContext() { }
  virtual int32_size_t size() = 0;
  virtual void draw_text(const char *message, scalar_t x, scalar_t y, TextStyle *style = NULL) = 0;
  virtual void draw_line(scalar_t x0, scalar_t y0, scalar_t x1, scalar_t y1, Style *style = NULL) = 0;
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
