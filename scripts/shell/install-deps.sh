#!/usr/bin/bash

sudo pacman -Syy git    \
                 python \
                 gcc    \
                 llvm   \
                 cmake  \
                 scons  \
                 ninja  \
                 dotnet-sdk

# needed for clang-format >= v15.0
paru -Syy lldb llvm-git
