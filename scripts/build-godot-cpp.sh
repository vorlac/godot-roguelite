#!bin/bash

pip install scons

# (from repo root)
git submodule update --init

./tools/godot/Godot_v4.1-stable_win64.exe --dump-extension-api extension_api.json

(
  cd libs/godot-cpp
  scons platform=windows -j16 custom_api_file=../../extension_api.json bits=64
)
