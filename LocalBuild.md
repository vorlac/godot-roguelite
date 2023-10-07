# Commands to run locally

```shell
mkdir .out
rm -rf .out/\*
cmake -B ./.out \
 -G "Ninja" \
 -DCMAKE_C_COMPILER:STRING="clang" \
 -DCMAKE_CXX_COMPILER:STRING="clang++" \
 -DVCPKG_TARGET_TRIPLET:STRING="x64-osx" \
 -DCMAKE_BUILD_TYPE:STRING="Debug" \
 -DCMAKE_INSTALL_PREFIX:PATH="/Users/vickylance/Projects/c++/godot-roguelite/.out/build/msvc-debug" \
 -DCMAKE_BINARY_PREFIX:PATH="/Users/vickylance/Projects/c++/godot-roguelite/.out/build/msvc-debug" \
 -DCMAKE_TOOLCHAIN_FILE:FILEPATH="/Users/vickylance/Projects/c++/godot-roguelite/extern/vcpkg/scripts/buildsystems/vcpkg.cmake" \
 -DCMAKE_MAKE_PROGRAM="/opt/homebrew/bin/ninja" \
 -S ./
cmake --build ./.out --config Debug --target clean --parallel 10
cmake --build ./.out --config Debug --target roguelite --parallel 10
```
