//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

/// Basic C assertions for testing. These should not be used in program code,
/// only tests.

#ifndef _HELPERS
#define _HELPERS

#include "test/asserts.hh"
#include "image.hh"

namespace matisse {

class TestHelpers {
public:
  // Reads a test image under images/ into the given bitmap.
  static bool read_test_image(const char *filename, Bitmap *out);
};

} // namespace matisse

#endif // _HELPERS

