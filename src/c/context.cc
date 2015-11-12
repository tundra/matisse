//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "c/stdc.h"

#include "context.hh"
#include "impl-common.hh"

BEGIN_C_INCLUDES
#include "utils/check.h"
#include "utils/log.h"
END_C_INCLUDES

#include "skia-includes.hh"

using namespace matisse;

SkiaGraphicsContext::SkiaGraphicsContext(SkCanvas *canvas)
  : canvas_(canvas) {
  canvas->ref();
}

SkiaGraphicsContext::~SkiaGraphicsContext() {
  canvas_->flush();
}

void SkiaGraphicsContext::clear() {
  canvas_->clear(SK_ColorWHITE);
}

void SkiaGraphicsContext::draw_text(const char *message, int32_t x, int32_t y) {
  SkPaint paint;
  paint.setColor(SK_ColorDKGRAY);
  paint.setTextSize(16);
  paint.setTextAlign(SkPaint::kCenter_Align);
  paint.setAntiAlias(false);
  canvas_->drawText(message, strlen(message), SkScalar(x), SkScalar(y), paint);
}

void SkiaGraphicsContext::draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1) {
  SkPaint paint;
  paint.setColor(SK_ColorRED);
  paint.setAntiAlias(false);
  canvas_->drawLine(SkScalar(x0), SkScalar(y0), SkScalar(x1), SkScalar(y1), paint);
}

int32_size_t SkiaGraphicsContext::size() {
  SkISize raw_size = canvas_->getBaseLayerSize();
  return int32_size_t(raw_size.width(), raw_size.height());
}

class SkiaMainWindow : public SkOSWindow, public MainWindow {
public:
  SkiaMainWindow(Renderer *renderer, void *hwnd);
  virtual ~SkiaMainWindow();
  virtual void draw(SkCanvas *canvas) override;
  virtual void set_title(const char *title);
private:
  SkiaMainWindow(const SkiaMainWindow&);
  SkiaMainWindow &operator=(const SkiaMainWindow&);
  Renderer *renderer_;
};

SkiaMainWindow::SkiaMainWindow(Renderer *renderer, void *hwnd)
  : SkOSWindow(hwnd)
  , renderer_(renderer) {
}

SkiaMainWindow::~SkiaMainWindow() {
  delete renderer_;
}

void SkiaMainWindow::draw(SkCanvas *canvas) {
  SkiaGraphicsContext context(canvas);
  renderer_->draw(&context);
}

void SkiaMainWindow::set_title(const char *title) {
  setTitle(title);
}

void SkFlattenable::InitializeFlattenablesIfNeeded() {
  // nothing to do
}

SkColorFilter* SkColorFilter::CreateModeFilter(SkColor color, SkXfermode::Mode mode) {
  return NULL;
}
