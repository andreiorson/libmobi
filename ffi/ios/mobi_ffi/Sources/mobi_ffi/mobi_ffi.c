// Swift Package Manager, like CocoaPods, refuses to reach outside the package
// it is building: a target's `path` must stay under the directory that holds
// Package.swift. So this file plays the same role for Swift Package Manager
// that ../../../Classes/mobi_ffi.c plays for CocoaPods — it is the one
// file inside the package, and it pulls the shared C sources in by relative
// include.
//
// It forwards to the CocoaPods forwarder rather than repeating its list, so
// the sources that make up the iOS build are written down exactly once. The
// includes inside that file are resolved against its own directory, which is
// how they still find ../../src.
#include "../../../Classes/mobi_ffi.c"
