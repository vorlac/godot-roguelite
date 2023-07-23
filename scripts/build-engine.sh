#!bash

# https://docs.godotengine.org/en/stable/contributing/development/compiling/introduction_to_the_buildsystem.html

(
	cd ./engine
	scons --clean platform=windows target=editor arch=x86_64 debug_symbols=yes optimize=none
	scons --no-cache platform=windows target=editor arch=x86_64 debug_symbols=yes optimize=none
)
