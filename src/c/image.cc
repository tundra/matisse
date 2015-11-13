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
using namespace tclib;

class Bitmap::Data {
private:
  friend class Bitmap;
  SkBitmap sk_bitmap_;
};

// A hack taken inspired by from SkForceLinking which ensures that the linker
// won't discard static initialization of the image decoders.
void matisse_force_decoder_linking(bool not_true);

Bitmap::Bitmap() : data_(new Data()) { }

Bitmap::~Bitmap() {
  delete data_;
}

bool Utils::read_in_stream(InStream *in, std::vector<byte_t> *out) {
  while (true) {
    static const size_t kBufSize = 1024;
    byte_t raw_buffer[kBufSize];
    ReadIop iop(in, raw_buffer, kBufSize);
    if (!iop.execute())
      return false;
    size_t was_read = iop.bytes_read();
    bool at_eof = iop.at_eof();
    out->insert(out->end(), raw_buffer, raw_buffer + was_read);
    if (at_eof)
      break;
  }
  return true;
}

bool Bitmap::read_from_png(InStream *in) {
  // This call doesn't actually do anything, it just has to be somewhere.
  matisse_force_decoder_linking(false);
  std::vector<byte_t> bytes;
  if (!Utils::read_in_stream(in, &bytes))
    return false;
  return SkImageDecoder::DecodeMemory(bytes.data(), bytes.size(),
      &data_->sk_bitmap_);
}

bool Bitmap::init_empty(int width, int height) {
  SkImageInfo info = SkImageInfo::Make(width, height, kRGB_565_SkColorType,
      kPremul_SkAlphaType);
  if (!data_->sk_bitmap_.setInfo(info))
    return false;
  data_->sk_bitmap_.allocPixels();
  return true;
}

bool Bitmap::write_to_png(tclib::OutStream *out) {
  SkData *data = SkImageEncoder::EncodeData(data_->sk_bitmap_,
      SkImageEncoder::kPNG_Type, 100);
  if (data == NULL)
    return false;
  WriteIop iop(out, data->data(), data->size());
  if (!iop.execute())
    return false;
  data->unref();
  return true;
}

int32_size_t Bitmap::size() {
  int width = data_->sk_bitmap_.width();
  int height = data_->sk_bitmap_.height();
  return int32_size_t(width, height);
}

GraphicsContext *Bitmap::new_context() {
  SkCanvas *canvas = new SkCanvas(data_->sk_bitmap_);
  GraphicsContext *result = new SkiaGraphicsContext(canvas);
  canvas->unref();
  return result;
}

bool Bitmap::equals(Bitmap *that) {
  int32_size_t this_size = this->size();
  int32_size_t that_size = that->size();
  if (this_size.width() != that_size.width() || this_size.height() != that_size.height())
    return false;
  for (int x = 0; x < this_size.width(); x++) {
    for (int y = 0; y < this_size.height(); y++) {
      SkColor this_pixel = data_->sk_bitmap_.getColor(x, y);
      SkColor that_pixel = that->data_->sk_bitmap_.getColor(x, y);
      if (this_pixel != that_pixel)
        return false;
    }
  }
  return true;
}

void matisse_force_decoder_linking(bool not_true) {
  CHECK_FALSE("passed true to force_decoder_linking", not_true);
  if (not_true) {
#ifndef IS_MSVC
    CreatePNGImageDecoder();
#endif
  }
}
