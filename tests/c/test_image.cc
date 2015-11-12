//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "context.hh"
#include "helpers.hh"
#include "test/unittest.hh"

using namespace matisse;
using namespace tclib;

TEST(image, read) {
  Bitmap bitmap;
  ASSERT_TRUE(TestHelpers::read_test_image("test_image_hello_chorus.png", &bitmap));
  int32_size_t size = bitmap.size();
  ASSERT_EQ(150, size.width());
  ASSERT_EQ(100, size.height());
}

TEST(image, compare) {
  Bitmap a;
  ASSERT_TRUE(TestHelpers::read_test_image("test_image_hello_chorus.png", &a));
  Bitmap b;
  ASSERT_TRUE(TestHelpers::read_test_image("test_image_hello_chorus.png", &b));
  ASSERT_TRUE(a.equals(&b));
  ASSERT_IMGEQ(&a, "test_image_hello_chorus.png");
  GraphicsContext *context = a.new_context();
  context->draw_text(".", 10, 10);
  ASSERT_FALSE(a.equals(&b));
  delete context;
}
