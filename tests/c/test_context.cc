//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "context.hh"
#include "helpers.hh"
#include "test/unittest.hh"

using namespace matisse;
using namespace tclib;

TEST(context, paint) {
  Bitmap bitmap;
  ASSERT_TRUE(bitmap.init_empty(150, 100));
  GraphicsContext *context = bitmap.new_context();
  context->clear();
  context->draw_line(25, 25, 125, 75);
  delete context;
  ASSERT_IMGEQ(&bitmap, "test_context_simple_line.png");
}
