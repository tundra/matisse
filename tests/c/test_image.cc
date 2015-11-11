//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "context.hh"
#include "image.hh"
#include "test/unittest.hh"
#include "io/file.hh"
#include "utils/string-inl.h"

using namespace renderer;
using namespace tclib;

// Returns the path to the durian executable.
static utf8_t write_full_test_path(char *scratch, const char *filename) {
  const char *root = getenv("TEST_IMAGES");
  ASSERT_TRUE(root != NULL);
  int size = sprintf(scratch, "%s/%s", root, filename);
  return new_string(scratch, size);
}

TEST(image, read) {
  Bitmap bitmap;
  char scratch[256];
  utf8_t full_path = write_full_test_path(scratch, "test_image_hello_chorus.png");
  FileStreams streams = FileSystem::native()->open(full_path, OPEN_FILE_MODE_READ);
  ASSERT_TRUE(bitmap.read_from_png(streams.in()));
  streams.close();
  int32_size_t size = bitmap.size();
  ASSERT_EQ(150, size.width());
  ASSERT_EQ(100, size.height());
}
