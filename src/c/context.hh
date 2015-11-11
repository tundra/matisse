//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#ifndef _CONTEXT_HH
#define _CONTEXT_HH

#include "c/stdc.h"

#include "image.hh"

namespace renderer {

class GraphicsContext {
public:
  virtual ~GraphicsContext() { }
  virtual int32_size_t size() = 0;
  virtual void draw_text(const char *message, int32_t x, int32_t y) = 0;
  virtual void clear() = 0;
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
