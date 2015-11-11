//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#include "c/stdc.h"

#include "helpers.hh"
#include "io/file.hh"

BEGIN_C_INCLUDES
#include "utils/string-inl.h"
END_C_INCLUDES

using namespace matisse;
using namespace tclib;

// Returns the path to the durian executable.
static utf8_t write_full_test_path(char *scratch, const char *filename) {
  const char *root = getenv("TEST_IMAGES");
  ASSERT_TRUE(root != NULL);
  int size = sprintf(scratch, "%s/%s", root, filename);
  return new_string(scratch, size);
}

bool TestHelpers::read_test_image(const char *filename, Bitmap *out) {
  char scratch[256];
  utf8_t full_path = write_full_test_path(scratch, "test_image_hello_chorus.png");
  FileStreams streams = FileSystem::native()->open(full_path, OPEN_FILE_MODE_READ);
  if (!out->read_from_png(streams.in()))
    return false;
  streams.close();
  return true;
}
