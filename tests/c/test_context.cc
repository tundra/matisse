//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "context.hh"
#include "helpers.hh"
#include "test/unittest.hh"
#include "io/file.hh"

using namespace matisse;
using namespace tclib;

TEST(context, lines) {
  Bitmap bitmap;
  ASSERT_TRUE(bitmap.init_empty(150, 100));
  GraphicsContext *context = bitmap.new_context();
  context->clear(Color::white());
  Style style;
  style.set_antialias(false);
  style.set_color(Color::red());
  context->draw_line(25, 25, 125, 75, &style);
  ASSERT_IMGEQ(&bitmap, "test_context_simple_line.png");

  context->clear(Color::gray(224));
  for (int i = 25; i <= 125; i += 5) {
    style.set_color(Color(0, 0, static_cast<uint8_t>(2 * i)));
    context->draw_line(scalar_t(i), scalar_t(25), scalar_t(i), scalar_t(75),
        &style);
  }
  delete context;
  ASSERT_IMGEQ(&bitmap, "test_context_gradient_lines.png");
}

TEST(context, default_style) {
  Bitmap bitmap;
  ASSERT_TRUE(bitmap.init_empty(150, 100));
  GraphicsContext *context = bitmap.new_context();
  context->clear(Color::white());
  Style style;
  style.set_antialias(false);
  style.set_color(Color::red());
  context->set_default_style(&style);

  context->draw_line(25, 25, 125, 75);
  ASSERT_IMGEQ(&bitmap, "test_context_simple_line.png");

  delete context;
}

TEST(context, ttf_font) {
  Bitmap bitmap;
  ASSERT_TRUE(bitmap.init_empty(150, 100));
  GraphicsContext *context = bitmap.new_context();
  TextStyle style;
  style.set_antialias(false);
  style.set_color(Color::black());
  style.set_typeface(TestHelpers::read_monospace_bitmap_typeface());

  style.set_text_size(TestHelpers::kMonospaceBitmapTypefacePreferredSize);
  context->clear(Color::white());
  context->draw_text("Hello", 10, 40, &style);
  context->draw_text("Matisse", 10, 80, &style);
  ASSERT_IMGEQ(&bitmap, "test_context_ttf_font.png");

  style.set_text_size(TestHelpers::kMonospaceBitmapTypefaceBigSize);
  context->clear(Color::white());
  context->draw_text("Hello", 10, 40, &style);
  context->draw_text("Matisse", 10, 80, &style);
  ASSERT_IMGEQ(&bitmap, "test_context_ttf_font_big.png");

  style.set_typeface(TestHelpers::read_sans_serif_typeface());
  style.set_text_size(24);
  context->clear(Color::white());
  context->draw_text("Hello", 10, 40, &style);
  context->draw_text("Matisse", 10, 80, &style);
  ASSERT_IMGEQ_WITH_TOLERANCE(&bitmap, "test_context_ttf_sans_serif.png", 1);

  delete context;
}

TEST(context, complex) {
  Typeface sans_serif = TestHelpers::read_sans_serif_typeface();
  Bitmap bitmap;
  ASSERT_TRUE(bitmap.init_empty(150, 100));
  GraphicsContext *context = bitmap.new_context();
  TextStyle style;
  style.set_text_size(18);
  style.set_antialias(false);
  style.set_color(Color::black());
  style.set_typeface(sans_serif);

  context->clear(Color::white());
  context->draw_text("Hello\xcc\x86\xcc\xa7\xcc\xb6?", 25, 50, &style);
  ASSERT_IMGEQ_WITH_TOLERANCE(&bitmap, "test_context_combining_diacritics.png", 1);

  delete context;
}
