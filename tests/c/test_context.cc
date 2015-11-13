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
    context->draw_line(i, 25, i, 75, &style);
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
  char scratch[256];
  utf8_t font_file = TestHelpers::get_test_resource_path("resources/Minecraft.ttf", scratch);
  FileStreams streams = FileSystem::native()->open(font_file, OPEN_FILE_MODE_READ);
  Typeface minecraft = Typeface::read(streams.in());
  streams.close();

  Bitmap bitmap;
  ASSERT_TRUE(bitmap.init_empty(150, 100));
  GraphicsContext *context = bitmap.new_context();
  context->clear(Color::white());
  TextStyle style;
  style.set_antialias(false);
  style.set_color(Color::black());
  style.set_typeface(minecraft);
  style.set_text_size(16);
  context->draw_text("Hello", 10, 40, &style);
  context->draw_text("Matisse", 10, 80, &style);
  ASSERT_IMGEQ_WITH_TAG(&bitmap, "test_context_ttf_font.png", 100);
  delete context;
}