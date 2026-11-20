// swift-tools-version: 5.9
// The swift-tools-version declares the minimum version of Swift required to
// build this package.

import PackageDescription

// The Swift Package Manager counterpart of ../mobi_ffi.podspec. Both must
// keep working: CocoaPods stays the fallback for this plugin, and the Android
// build still compiles ../src through CMake.
//
// How the podspec maps onto this manifest:
//
//   s.source_files = 'Classes/**/*'
//       Sources/mobi_ffi/mobi_ffi.c, which includes ../../../Classes/
//       mobi_ffi.c. A Swift package may not reach outside its own
//       directory, so it cannot simply point at Classes/; forwarding to the
//       CocoaPods forwarder keeps the list of shared C sources in one file.
//
//   s.dependency 'Flutter'
//       Dropped. It is boilerplate from the FFI plugin template: nothing under
//       ../src includes a Flutter header. Swift Package Manager plugins that do
//       need Flutter get it from the generated FlutterGeneratedPluginSwiftPackage.
//
//   s.platform = :ios, '12.0'
//       Mirrored as-is. A package floor only has to sit at or below its
//       dependent's; the generated FlutterGeneratedPluginSwiftPackage declares
//       15.0 and the app builds at 16.0, so 12.0 is satisfied by both. Raising
//       it to match the app would make this the one package in the graph
//       asking for more than the umbrella that consumes it, for no gain.
//
//   'DEFINES_MODULE' => 'YES'
//       Not needed. Every Swift package target is a module already.
//
//   'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'i386'
//       Not needed. Xcode 15 and later cannot target i386 at all, and a Swift
//       package manifest has no way to express excluded architectures.
//
//   s.swift_version = '5.0'
//       Covered by the swift-tools-version above. This target has no Swift in
//       it; the setting only ever described the toolchain.
let package = Package(
    name: "mobi_ffi",
    platforms: [
        .iOS("12.0")
    ],
    products: [
        // Dynamic, and not by default: with no `type:` Swift Package Manager
        // picks `.automatic`, which resolves to static linkage and fuses this
        // library into the Runner binary. ../../src vendors libmobi, which is
        // LGPL, and LGPLv3 section 4(d)(1) contemplates the user substituting
        // an interface-compatible build of the library for ours — an
        // arrangement a dynamic library allows and static linkage forecloses.
        // CocoaPods gave us this for nothing under `use_frameworks!`; Swift
        // Package Manager has to be asked.
        .library(name: "mobi-ffi", type: .dynamic, targets: ["mobi_ffi"])
    ],
    dependencies: [],
    targets: [
        .target(
            name: "mobi_ffi",
            dependencies: [],
            cSettings: [
                // The plugin exposes no headers to Swift or Objective-C: Dart
                // reaches the symbols through FFI. The directory exists because
                // Swift Package Manager insists a C target have one.
                .headerSearchPath("include/mobi_ffi")
            ]
        )
    ]
)
