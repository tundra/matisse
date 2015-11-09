# Copyright 2015 the Neutrino authors (see AUTHORS).
# Licensed under the Apache License, Version 2.0 (see LICENSE).
#
# Main gyp to use when building skia for matisse. You can't just build this
# directly, you have to symlink the directory into the skia checkout and then
# build it from there.
{
  "variables": {
    "skia_gyp_path": "gyp",
    "skia_src_path": "src",
    "skia_include_path": "include",
    "skia_gpu": 0,
    "skia_pdf": 0,
    # Link-time code generation makes linking against the library dog slow on
    # windows so disable it.
    "skia_win_ltcg": 0,
    "includes": ["gyp/opts.gypi"],
  },
  "targets": [
    {
      "target_name": "matisse",
      "product_name": "skia_matisse",
      "type": "static_library",
      "standalone_static_library": 1,
      "msvs_disabled_warnings": [4244, 4267, 4800],
      "includes": [
        "gyp/core.gypi",
        "gyp/utils.gypi",
      ],
      "dependencies": [
        "<(skia_gyp_path)/tools.gyp:tools",
        "<(skia_gyp_path)/xml.gyp:xml",
        "fontconfig",
      ],
      "defines": [
        # There doesn't seem to be a variable for this so set the define
        # directly.
        "SK_IGNORE_ETC1_SUPPORT=1",
      ],
      "include_dirs": [
        "<(skia_include_path)/c",
        "<(skia_include_path)/config",
        "<(skia_include_path)/core",
        "<(skia_include_path)/effects",
        "<(skia_include_path)/gpu",
        "<(skia_include_path)/images",
        "<(skia_include_path)/lazy",
        "<(skia_include_path)/pathops",
        "<(skia_include_path)/pipe",
        "<(skia_include_path)/ports",
        "<(skia_include_path)/private",
        "<(skia_include_path)/utils",
        "<(skia_include_path)/views",
        "<(skia_include_path)/xml",
        "<(skia_src_path)/core",
        "<(skia_src_path)/image",
        "<(skia_src_path)/lazy",
        "<(skia_src_path)/opts",
        "<(skia_src_path)/ports",
        "<(skia_src_path)/sfnt",
        "<(skia_src_path)/utils",
      ],
      "sources": [
        # These are defined in opts.gypi.
        "<@(none_sources)",
        "<@(ssse3_sources)",
        "<@(sse41_sources)",

        # Cross-platform stuff from ports.
        "<(skia_src_path)/ports/SkDiscardableMemory_none.cpp",
        "<(skia_src_path)/ports/SkImageDecoder_empty.cpp",
        "<(skia_src_path)/ports/SkImageGenerator_none.cpp",
        "<(skia_src_path)/ports/SkMemory_malloc.cpp",
        "<(skia_src_path)/ports/SkOSFile_stdio.cpp",

        # Cross-platform fonts stuff.
        "<(skia_src_path)/sfnt/SkOTTable_name.cpp",
        "<(skia_src_path)/sfnt/SkOTUtils.cpp",

        # Inline most of the views.
        "<(skia_include_path)/views/SkApplication.h",
        "<(skia_include_path)/views/SkBGViewArtist.h",
        "<(skia_include_path)/views/SkEvent.h",
        "<(skia_include_path)/views/SkEventSink.h",
        "<(skia_include_path)/views/SkKey.h",
        "<(skia_include_path)/views/SkOSMenu.h",
        "<(skia_include_path)/views/SkOSWindow_Mac.h",
        "<(skia_include_path)/views/SkOSWindow_SDL.h",
        "<(skia_include_path)/views/SkOSWindow_Unix.h",
        "<(skia_include_path)/views/SkOSWindow_Win.h",
        "<(skia_include_path)/views/SkStackViewLayout.h",
        "<(skia_include_path)/views/SkSystemEventTypes.h",
        "<(skia_include_path)/views/SkTouchGesture.h",
        "<(skia_include_path)/views/SkView.h",
        "<(skia_include_path)/views/SkViewInflate.h",
        "<(skia_include_path)/views/SkWidget.h",
        "<(skia_include_path)/views/SkWindow.h",

        "<(skia_src_path)/views/SkBGViewArtist.cpp",
        "<(skia_src_path)/views/SkEvent.cpp",
        "<(skia_src_path)/views/SkEventSink.cpp",
        "<(skia_src_path)/views/SkOSMenu.cpp",
        "<(skia_src_path)/views/SkParsePaint.cpp",
        "<(skia_src_path)/views/SkProgressView.cpp",
        "<(skia_src_path)/views/SkStackViewLayout.cpp",
        "<(skia_src_path)/views/SkTagList.cpp",
        "<(skia_src_path)/views/SkTagList.h",
        "<(skia_src_path)/views/SkTouchGesture.cpp",
        "<(skia_src_path)/views/SkView.cpp",
        "<(skia_src_path)/views/SkViewInflate.cpp",
        "<(skia_src_path)/views/SkViewPriv.cpp",
        "<(skia_src_path)/views/SkViewPriv.h",
        "<(skia_src_path)/views/SkWidgets.cpp",
        "<(skia_src_path)/views/SkWindow.cpp",

        # Instead of using the xml parser as a dependency just build it
        # directly, that way the result is only one library and we control how
        # it's built.
        "<(skia_include_path)/xml/SkBML_WXMLParser.h",
        "<(skia_include_path)/xml/SkBML_XMLParser.h",
        "<(skia_include_path)/xml/SkDOM.h",
        "<(skia_include_path)/xml/SkXMLParser.h",
        "<(skia_include_path)/xml/SkXMLWriter.h",

        "<(skia_src_path)/xml/SkBML_Verbs.h",
        "<(skia_src_path)/xml/SkBML_XMLParser.cpp",
        "<(skia_src_path)/xml/SkDOM.cpp",
        "<(skia_src_path)/xml/SkXMLParser.cpp",
        "<(skia_src_path)/xml/SkXMLWriter.cpp",

      ],
      "conditions": [
        ["OS == 'linux'", {
          "sources": [
            "<(skia_src_path)/fonts/SkFontMgr_fontconfig.cpp",
            "<(skia_src_path)/ports/SkDebug_stdio.cpp",
            "<(skia_src_path)/ports/SkFontConfigInterface_direct.cpp",
            "<(skia_src_path)/ports/SkFontHost_fontconfig.cpp",
            "<(skia_src_path)/ports/SkFontHost_FreeType.cpp",
            "<(skia_src_path)/ports/SkFontHost_FreeType_common.cpp",
            "<(skia_src_path)/ports/SkOSFile_posix.cpp",
            "<(skia_src_path)/ports/SkTLS_pthread.cpp",
            "<(skia_src_path)/views/unix/keysym2ucs.c",
            "<(skia_src_path)/views/unix/SkOSWindow_Unix.cpp",
          ],
        }],
        ["OS == 'win'", {
          "include_dirs": [
            "<(skia_include_path)/utils/win",
            "<(skia_src_path)/utils/win",
          ],
          "sources": [
            "<(skia_src_path)/fonts/SkFontMgr_indirect.cpp",
            "<(skia_src_path)/ports/SkDebug_win.cpp",
            "<(skia_src_path)/ports/SkFontMgr_win_dw.cpp",
            "<(skia_src_path)/ports/SkFontMgr_win_dw_factory.cpp",
            "<(skia_src_path)/ports/SkOSFile_win.cpp",
            "<(skia_src_path)/ports/SkScalerContext_win_dw.cpp",
            "<(skia_src_path)/ports/SkTime_win.cpp",
            "<(skia_src_path)/ports/SkTLS_win.cpp",
            "<(skia_src_path)/ports/SkTypeface_win_dw.cpp",
            "<(skia_src_path)/views/win/SkOSWindow_Win.cpp",
          ],
          "sources!": [
            # This one gets included unconditionally by one of the dependencies.
            "<(skia_src_path)/utils/SkThreadUtils_pthread.cpp",
          ],
        }],
        ["OS != 'mac'", {
          "sources/": [
            ["exclude", "/mac/"]
          ],
        }],
      ],
    },
    {
      # Ninja insists that there's a "most" target even if it's not used.
      "target_name": "most",
      "type": "none",
    },
    {
      # Get fontconfig settings from the system on linux. If we're not on linux
      # this is empty which is harmless.
      "target_name": "fontconfig",
      "type": "none",
      "conditions": [
        ["OS == 'linux'", {
          "direct_dependent_settings": {
            "cflags": [
              "<!@(pkg-config --cflags fontconfig)",
            ],
          },
          "link_settings": {
            "ldflags": [
              "<!@(pkg-config --libs-only-L --libs-only-other fontconfig)",
            ],
            "libraries": [
              "<!@(pkg-config --libs-only-l fontconfig)",
            ],
          }
        }]
      ],
    }
  ],
}
