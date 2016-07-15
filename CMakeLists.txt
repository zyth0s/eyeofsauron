cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR) # dev:3.0.2
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

option(USE_CLANG "build application with clang" OFF) 
if(USE_CLANG)
    SET (CMAKE_C_COMPILER "/usr/bin/clang")
    SET (CMAKE_C_FLAGS_INIT                "-Wall -std=c99")
    SET (CMAKE_C_FLAGS_DEBUG_INIT          "-g")
    SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

    SET (CMAKE_CXX_COMPILER "/usr/bin/clang++")
    SET (CMAKE_CXX_FLAGS_INIT                "-Wall")
    SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
    SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")
    # Static
    #SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    #SET(BUILD_SHARED_LIBRARIES OFF)
    #SET(CMAKE_EXE_LINKER_FLAGS "-static")
endif(USE_CLANG)

###########################################################################
# OpenBabel
###########################################################################
set(OPENBABEL2_INCLUDE_DIR /home/daniel/pkg/openbabel-2.3.2-uovi/install/include/openbabel-2.0/)
set(OPENBABEL2_LIBRARIES /home/daniel/pkg/openbabel-2.3.2-uovi/install/lib/libopenbabel.so)
set(OPENBABEL2_VERSION_MET TRUE)
find_package(OpenBabel2 REQUIRED)
include_directories(${OPENBABEL2_INCLUDE_DIR})

###########################################################################
# VTK
###########################################################################
set(VTK_RENDERING_BACKEND "OpenGL")
option(USE_OpenGL2 "Use OpenGL2 instead of OpenGL" ON)
if(USE_OpenGL2)
    # Needs VTK compiled with OpenGL2 (faster)
    set(VTK_RENDERING_BACKEND "OpenGL2") 
endif(USE_OpenGL2)
if(NOT VTK_BINARY_DIR)
    find_package(VTK 7.0 REQUIRED COMPONENTS
      #  find_package(VTK 7.0 COMPONENTS
          vtkFiltersSources
          vtkInteractionStyle
          vtkRenderingCore
          vtkRendering${VTK_RENDERING_BACKEND}
          vtkInteractionStyle
          vtkDomainsChemistry
          vtkRenderingAnnotation
          vtkInteractionWidgets
          NO_MODULE
  )
  # Components are listed in
  # /home/daniel/pkg/vis/VTK-7.0.0/build/lib/cmake/vtk-7.0/Modules/
  include(${VTK_USE_FILE})
  #include("/home/daniel/pkg/vis/VTK-7.0.0/Domains/Chemistry")
endif()

###########################################################################
# Boost
###########################################################################
FIND_PACKAGE(Boost 1.49) # dev: 1.55
IF(Boost_FOUND)
  INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIRS}" "/usr/include/python2.7")
  SET(Boost_USE_STATIC_LIBS OFF)
  SET(Boost_USE_MULTITHREADED ON)
  SET(Boost_USE_STATIC_RUNTIME OFF)
  FIND_PACKAGE(Boost ${BOOST_VERSION} COMPONENTS python)


  #file(GLOB SOURCES
  #    include/*.hpp
  #    #./*.h
  #    ./*.cpp
  #)

  set(SOURCES 
      include/representation.hpp 
      include/escher.hpp 
      representation.cpp 
      escher.cpp
      promoldenformat.cpp
  )
  
  # A python module for scripting.
  add_library(escherpy SHARED ${SOURCES} pymodule.cpp)
  target_link_libraries(escherpy ${OPENBABEL2_LIBRARIES} ${VTK_LIBRARIES} ${Boost_LIBRARIES})

ENDIF()


# An executable for the C++ library --> QtEscher.
add_executable(main main.cpp ${SOURCES})
target_link_libraries(main ${OPENBABEL2_LIBRARIES} ${VTK_LIBRARIES})
