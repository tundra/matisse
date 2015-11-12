//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#ifndef _HELPERS
#define _HELPERS

#include "test/asserts.hh"
#include "image.hh"

namespace matisse {

// Asserts that the contents of the given bitmap are equals, pixel-for-pixel, to
// the test image (under images/) with the given name. If not the input bitmap
// is stored in a file in the current directory.
#define ASSERT_IMGEQ(bitmap, filename) do {                                    \
  matisse::TestHelpers::assert_imgeq(__FILE__, __LINE__, bitmap, filename,     \
      "Image comparison failed: " #bitmap " equals " #filename ".\n"           \
      "  Contents of bitmap saved to %s.\n");                                  \
} while (false)

// Various helper functions for working with skia.
class TestHelpers {
public:
  // Reads a test image under images/ into the given bitmap. Returns true iff
  // successful.
  static bool read_test_image(const char *filename, Bitmap *out);

  // Performs an ASSERT_IMGEQ.
  static void assert_imgeq(const char *file, int line, Bitmap *bitmap,
      const char *filename, const char *fail_fmt);
};

} // namespace matisse

#endif // _HELPERS

