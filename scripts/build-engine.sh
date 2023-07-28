#!bash

# https://docs.godotengine.org/en/stable/contributing/development/compiling/introduction_to_the_buildsystem.html

(
	cd ./engine
	scons --clean platform=windows arch=x86_64 target=editor use_static_cpp=no dev_build=yes debug_symbols=yes verbose=yes optimize=none use_lto=no
	# --tree=linedraw 
	scons platform=windows arch=x86_64 target=editor use_static_cpp=no dev_build=yes debug_symbols=yes verbose=yes optimize=none use_lto=no
)
