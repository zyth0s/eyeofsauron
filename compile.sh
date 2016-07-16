#!/bin/sh

###################################################
# Manual
###################################################

#rm out.gzmat
#rm xyz2int
#g++ -I/usr/include/openbabel-2.0 -rdynamic /usr/lib/libopenbabel.so -Wl,-rpath,/usr/lib xyz2int.cpp -o xyz2int
##g++ -I/usr/include/openbabel-2.0 -rdynamic /usr/lib/libopenbabel.so  xyz2int.cpp -o xyz2int
#./xyz2int input.xyz out.gzmat

###################################################
# With CMake
###################################################
rm bin/main
rm lib/libeeyeos.so
cmake -DVTK_DIR:PATH=/home/daniel/pkg/vis/VTK-7.0.0/build .
make -j 4 #VERBOSE=1
rm -Rf CMakeCache.txt cmake_install.cmake CMakeFiles
echo "---------------------------------------------------------------------------"
echo "---------------------------------------------------------------------------"
#./bin/main 

