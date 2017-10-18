cmake .

del CMakeCache.txt
del cmake_install.cmake
rmdir /s/q CMakeFiles
:del ALL_BUILD.vcproj
del ZERO_CHECK.vcproj
del *.user