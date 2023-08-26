#!/usr/bin/bash

sudo pacman -Syy git    \
                 python \
                 gcc    \
                 llvm   \
                 cmake  \
                 scons  \
                 ninja  \
                 dotnet-sdk

# needed for clang-format v16 so the binary path defined in .vscode works.
# arch core clang/llvm packages are still v15.0.
# install path should be something like this:
#     /opt/clang-format-static/clang-format-16
paru -Syy clang-format-static-bin
