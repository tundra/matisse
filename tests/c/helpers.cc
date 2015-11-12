//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "c/stdc.h"

#include "helpers.hh"
#include "io/file.hh"

BEGIN_C_INCLUDES
#include "utils/string-inl.h"
#include "utils/log.h"
END_C_INCLUDES

using namespace matisse;
using namespace tclib;

// Returns the path to the durian executable.
static utf8_t get_full_test_path(char *scratch, const char *filename) {
  const char *root = getenv("TEST_IMAGES");
  ASSERT_TRUE(root != NULL);
  int size = sprintf(scratch, "%s/%s", root, filename);
  return new_string(scratch, size);
}

bool TestHelpers::read_test_image(const char *filename, Bitmap *out) {
  char scratch[256];
  utf8_t full_path = get_full_test_path(scratch, "test_image_hello_chorus.png");
  FileStreams streams = FileSystem::native()->open(full_path, OPEN_FILE_MODE_READ);
  if (!out->read_from_png(streams.in()))
    return false;
  streams.close();
  return true;
}

// Gets the full path to use for a failure image for an assertion that appeared
// at the given source location.
static utf8_t get_failure_image_path(const char *file, int line, char *scratch) {
  const char *test_name = strstr(file, "test_");
  ASSERT_TRUE(test_name != NULL);
  sprintf(scratch, "%s-%i.png", test_name, line);
  return new_c_string(scratch);
}

void TestHelpers::assert_imgeq(const char *file, int line, Bitmap *bitmap,
    const char *filename, const char *fail_fmt) {
  Bitmap expected;
  ASSERT_TRUE(read_test_image(filename, &expected));
  if (expected.compare(bitmap))
    return;
  char scratch[256];
  utf8_t image_path = get_failure_image_path(file, line, scratch);
  FileStreams streams = FileSystem::native()->open(image_path,
      OPEN_FILE_MODE_WRITE | OPEN_FILE_FLAG_BINARY);
  ASSERT_TRUE(bitmap->write_to_png(streams.out()));
  streams.close();
  fail(file, line, fail_fmt, image_path.chars);
}
