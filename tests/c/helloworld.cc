//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "context.hh"

using namespace renderer;

class HelloWorldRenderer : public Renderer {
public:
  virtual void configure(MainWindow *main_window) override;
  virtual void draw(GraphicsContext *context) override;
};

void HelloWorldRenderer::configure(MainWindow *main_window) {
  main_window->set_title("Hello, Chorus!");
}

void HelloWorldRenderer::draw(GraphicsContext *context) {
  context->clear();
  int32_size_t size = context->size();
  context->draw_text("Hello, Chorus!", size.width() / 2, size.height() / 2);
}

Renderer *EntryPoint::create_renderer() {
  return new HelloWorldRenderer();
}
