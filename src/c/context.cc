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

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
  : argb_(SkColorSetARGB(a, r, g, b)) { }

SkiaGraphicsContext::SkiaGraphicsContext(SkCanvas *canvas)
  : canvas_(canvas) {
  canvas->ref();
}

SkiaGraphicsContext::~SkiaGraphicsContext() {
  canvas_->flush();
}

void SkiaGraphicsContext::clear(Color color) {
  canvas_->clear(to_sk_color(color));
}

void SkiaGraphicsContext::set_default_style(Style *style) {
  sk_default_paint_ = SkPaint();
  sk_default_paint_ = style_to_sk_paint(style);
}

void SkiaGraphicsContext::draw_text(const char *message, int32_t x, int32_t y,
    TextStyle *style) {
  SkPaint paint = text_style_to_sk_paint(style);
  canvas_->drawText(message, strlen(message), SkScalar(x), SkScalar(y), paint);
}

SkColor SkiaGraphicsContext::to_sk_color(Color color) {
  return color.argb();
}

SkPaint SkiaGraphicsContext::style_to_sk_paint(Style *style) {
  if (style == NULL)
    return sk_default_paint_;
  SkPaint result = sk_default_paint_;
  if (style->color().has_value())
    result.setColor(to_sk_color(*style->color()));
  if (style->antialias().has_value())
    result.setAntiAlias(*style->antialias());
  return result;
}

SkPaint SkiaGraphicsContext::text_style_to_sk_paint(TextStyle *style) {
  SkPaint result = style_to_sk_paint(style);
  if (style->text_size().has_value())
    result.setTextSize(*style->text_size());
  if (style->typeface().has_value())
    result.setTypeface(style->typeface().value().sk_typeface());
  return result;
}

Typeface Typeface::read(tclib::InStream *in) {
  std::vector<uint8_t> data;
  if (!Utils::read_in_stream(in, &data))
    return Typeface();
  // The stream will be oned by the typeface so we need to copy the data out
  // of the vector.
  SkMemoryStream *stream = new SkMemoryStream(data.data(), data.size(),
      /*copyData*/ true);
  // Ownership of the stream is transferred so no need to delete it.
  SkTypeface *sk_typeface = SkTypeface::CreateFromStream(stream);
  return Typeface(sk_typeface);
}

Typeface::Typeface(SkTypeface *sk_typeface)
  : sk_typeface_(sk_typeface) { }

Typeface::~Typeface() {
  SkSafeUnref(sk_typeface_);
}

Typeface::Typeface(const Typeface &that) : sk_typeface_(that.sk_typeface_) {
  SkSafeRef(sk_typeface_);
}

const Typeface &Typeface::operator=(const Typeface &that) {
  if (that.sk_typeface_ != this->sk_typeface_) {
    SkSafeUnref(sk_typeface_);
    sk_typeface_ = that.sk_typeface_;
    SkSafeRef(sk_typeface_);
  }
  return *this;
}

void SkiaGraphicsContext::draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1,
    Style *style) {
  SkPaint sk_paint = style_to_sk_paint(style);
  canvas_->drawLine(SkScalar(x0), SkScalar(y0), SkScalar(x1), SkScalar(y1), sk_paint);
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
