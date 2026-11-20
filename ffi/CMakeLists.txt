# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(mobi_ffi_library VERSION 0.0.1 LANGUAGES C)

add_library(mobi_ffi SHARED
  "mobi_ffi.c"
  "../src/buffer.c"
	"../src/buffer.h"
	"../src/compression.c"
	"../src/compression.h"
	"../src/config.h"
	"../src/debug.c"
	"../src/debug.h"
	"../src/index.c"
	"../src/index.h"
	"../src/memory.c"
	"../src/memory.h"
	"../src/meta.c"
	"../src/meta.h"
	"../src/mobi.h"
	"../src/parse_rawml.c"
	"../src/parse_rawml.h"
	"../src/read.c"
	"../src/read.h"
	"../src/structure.c"
	"../src/structure.h"
	"../src/util.c"
	"../src/util.h"
	"../src/write.c"
	"../src/write.h"
	"../src/opf.h"
	"../src/opf.c"
	"../src/xmlwriter.h"
	"../src/xmlwriter.c"
)

set_target_properties(mobi_ffi PROPERTIES
  PUBLIC_HEADER mobi_ffi.h
  OUTPUT_NAME "mobi_ffi"
)

target_include_directories(mobi_ffi PRIVATE ../src)
target_compile_definitions(mobi_ffi PUBLIC DART_SHARED_LIB)

# Without USE_MINIZ, util.h takes uncompress and crc32 from the system zlib
# (the NDK ships it; iOS links it already).
target_link_libraries(mobi_ffi z)
