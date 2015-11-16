//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#ifndef _HELPERS
#define _HELPERS

#include "test/asserts.hh"
#include "image.hh"
#include "context.hh"

namespace matisse {

// Asserts that the contents of the given bitmap are equals, pixel-for-pixel, to
// the test image (under images/) with the given name. If not the input bitmap
// is stored in a file in the current directory whose name depends on the file
// and line where the assertion appears, such that different assertions should
// lead to different output files.
#define ASSERT_IMGEQ(BITMAP, IMGFILE) ASSERT_IMGEQ_WITH_TAG(BITMAP, IMGFILE, __LINE__)

// Same as ASSERT_IMGEQ but instead of basing the output name on the line where
// the assertion occurs uses an explicit tag int. This can be convenient during
// development so you can change the code without getting new files all the
// time. Generally you don't want to commit code that explicitly uses this.
#define ASSERT_IMGEQ_WITH_TAG(BITMAP, IMGFILE, TAG) do {                       \
  matisse::TestHelpers::assert_imgeq(__FILE__, __LINE__, TAG, (BITMAP), IMGFILE, \
      "Image comparison failed: " #BITMAP " equals " #IMGFILE ".\n"            \
      "  Contents of bitmap saved to %s.\n");                                  \
} while (false)

// Various helper functions for working with skia.
class TestHelpers {
public:
  // Reads a test image under images/ into the given bitmap. Returns true iff
  // successful.
  static bool read_test_image(const char *filename, Bitmap *out);

  // Performs an ASSERT_IMGEQ.
  static void assert_imgeq(const char *file, int line, int tag, Bitmap *bitmap,
      const char *imgfile, const char *fail_fmt);

  // Returns the path to a test resource relative to the current working
  // directory. The resource name must be relative to tests/c.
  static utf8_t get_test_resource_path(const char *relpath, char *scratch);

  // Reads and returns the monospace typeface.
  static Typeface read_monospace_typeface();

  // The monospace typeface is a bitmap font and this is the "natural" size for
  // the bitmaps.
  static const int32_t kMonospaceTypefacePreferredSize = 14;
};

} // namespace matisse

#endif // _HELPERS

