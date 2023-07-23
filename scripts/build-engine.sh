#!bash

# https://docs.godotengine.org/en/stable/contributing/development/compiling/introduction_to_the_buildsystem.html

(
	cd ./engine
	scons --clean
	scons platform=windows arch=x86_64 target=editor dev_build=yes debug_symbols=yes verbose=yes warnings=extra werror=yes use_lto=yes
)
