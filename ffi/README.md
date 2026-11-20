# ffi

The library as a Flutter app ships it: libmobi (`../src`, with this fork's
changes) and a small forwarder (`mobi_ffi.c`, `mobi_ffi.h`) built into one
shared library, `mobi_ffi`, that Dart reaches through FFI.

- Android and desktop: `CMakeLists.txt` builds `libmobi_ffi` from the
  forwarder and `../src`.
- iOS: `ios/` holds the CocoaPods spec and the Swift package that build the
  same sources as the dynamic framework `mobi_ffi` / `mobi-ffi`.

libmobi is LGPL-3.0-or-later; the forwarder and the build files here are
offered under the same licence.
