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
  style_to_sk_paint(style, &sk_default_paint_);
}

void SkiaGraphicsContext::draw_text(const char *message, scalar_t x, scalar_t y,
    TextStyle *style) {
  SkPaint paint = sk_default_paint_;
  text_style_to_sk_paint(style, &paint);
  canvas_->drawText(message, strlen(message), x, y, paint);
}

SkColor SkiaGraphicsContext::to_sk_color(Color color) {
  return color.argb();
}

void SkiaGraphicsContext::style_to_sk_paint(Style *style, SkPaint *out) {
  if (style == NULL)
    return;
  if (style->color().has_value())
    out->setColor(to_sk_color(*style->color()));
  if (style->antialias().has_value())
    out->setAntiAlias(*style->antialias());
}

void SkiaGraphicsContext::text_style_to_sk_paint(TextStyle *style, SkPaint *out) {
  if (style == NULL)
    return;
  style_to_sk_paint(style, out);
  if (style->text_size().has_value())
    out->setTextSize(*style->text_size());
  if (style->typeface().has_value())
    out->setTypeface(style->typeface().value().sk_typeface());
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

void SkiaGraphicsContext::draw_line(scalar_t x0, scalar_t y0, scalar_t x1, scalar_t y1,
    Style *style) {
  SkPaint paint = sk_default_paint_;
  style_to_sk_paint(style, &paint);
  canvas_->drawLine(x0, y0, x1, y1, paint);
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
