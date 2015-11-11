//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "c/stdc.h"

#include "context.hh"

BEGIN_C_INCLUDES
#include "utils/check.h"
#include "utils/log.h"
END_C_INCLUDES

#include "skia-includes.hh"

using namespace renderer;
using namespace tclib;

class Bitmap::Data {
private:
  friend class renderer::Bitmap;
  SkBitmap sk_bitmap_;
};

// A hack taken inspired by from SkForceLinking which ensures that the linker
// won't discard static initialization of the image decoders.
void matisse_force_decoder_linking(bool not_true);

Bitmap::Bitmap() : data_(new Data()) { }

Bitmap::~Bitmap() {
  delete data_;
}

// Reads the full contents of the given stream into the given byte vector.
static bool read_in_stream(InStream *in, std::vector<byte_t> *out) {
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
  if (!read_in_stream(in, &bytes))
    return false;
  return SkImageDecoder::DecodeMemory(bytes.data(), bytes.size(),
      &data_->sk_bitmap_);
}

int32_size_t Bitmap::size() {
  int width = data_->sk_bitmap_.width();
  int height = data_->sk_bitmap_.height();
  return int32_size_t(width, height);
}

void matisse_force_decoder_linking(bool not_true) {
  CHECK_FALSE("passed true to force_decoder_linking", not_true);
  if (not_true) {
#ifndef IS_MSVC
    CreatePNGImageDecoder();
#endif
  }
}
