//- Copyright 2015 the Neutrino authors (see AUTHORS).
//- Licensed under the Apache License, Version 2.0 (see LICENSE).

#ifndef _SKIA_BRIDGE_HH
#define _SKIA_BRIDGE_HH

#include "c/stdc.h"

#ifdef IS_MSVC
#  pragma warning(push, 0)
#  define DWORD unsigned long
#  define SK_BUILD_FOR_WIN 1
#  define SK_BUILD_FOR_WIN32 1
#endif

#ifdef IS_GCC
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wconversion"
#endif

// If skia gets to define this itself stuff goes wrong because of the way tclib
// defines always_inline. In fairness it's probably a bad idea for tclib to do
// that but it's just so much easier to work around it like this.
#define SK_ALWAYS_INLINE always_inline

#include "core/SkCanvas.h"
#include "core/SkColorFilter.h"
#include "core/SkData.h"
#include "core/SkDevice.h"
#include "core/SkDrawLooper.h"
#include "core/SkFlattenable.h"
#include "core/SkGraphics.h"
#include "core/SkImageDecoder.h"
#include "core/SkImageEncoder.h"
#include "core/SkPaint.h"
#include "core/SkPath.h"
#include "core/SkRRect.h"
#include "core/SkTypeface.h"
#include "core/SkTypes.h"
#include "effects/SkDashPathEffect.h"
#include "views/SkApplication.h"
#include "views/SkWindow.h"

#ifdef IS_MSVC
#  pragma warning(pop)
#  undef DWORD
#  undef SK_BUILD_FOR_WIN
#  undef SK_BUILD_FOR_WIN32
#endif

#ifdef IS_GCC
#  pragma GCC diagnostic pop
#endif

#endif // _SKIA_BRIDGE_HH
