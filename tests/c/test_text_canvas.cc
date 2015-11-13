//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "text_canvas.hh"
#include "helpers.hh"
#include "test/unittest.hh"
#include "io/file.hh"

using namespace matisse;
using namespace tclib;

TEST(text_canvas, bounds) {
  TextCanvas canvas(80, 24);
  canvas.set_margin(1);
  TextStyle style;
  style.set_typeface(TestHelpers::read_minecraft_typeface());
  canvas.set_style(&style);
  int32_size_t size = canvas.calc_canvas_size();
  ASSERT_EQ(962, size.width());
  ASSERT_EQ(242, size.height());
}

TEST(text_canvas, really_simple) {
  TextCanvas canvas(12, 8);
  TextStyle style;
  style.set_typeface(TestHelpers::read_minecraft_typeface());
  style.set_color(Color::black());
  style.set_text_size(16);
  canvas.set_style(&style);
  canvas.set_margin(1);
  Bitmap bitmap;
  int32_size_t size = canvas.calc_canvas_size();
  bitmap.init_empty(size.width(), size.height());
  GraphicsContext *context = bitmap.new_context();

  for (int x = 0; x < 12; x++) {
    for (int y = 0; y < 8; y++)
      canvas.at(x, y) = 'X';
  }
  canvas.render(context);
  ASSERT_IMGEQ(&bitmap, "test_text_canvas_xes.png");

  for (int x = 0; x < 12; x++) {
    for (int y = 0; y < 8; y++)
      canvas.at(x, y) = (char) ('a' + (12 * y + x) % 26);
  }
  canvas.render(context);
  ASSERT_IMGEQ(&bitmap, "test_text_canvas_alphabet.png");


  delete context;
}
