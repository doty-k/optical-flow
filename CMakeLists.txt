cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
project( project-3 )


find_package( OpenCV REQUIRED )
#find_program(MAGICK_CONFIG "Magick++-config")

#execute_process(COMMAND "${MAGICK_CONFIG}" "--cxxflags" OUTPUT_VARIABLE MAGICK_CXX_FLAGS)
#execute_process(COMMAND "${MAGICK_CONFIG}" "--libs" OUTPUT_VARIABLE MAGICK_LD_FLAGS)




add_executable( bin/main src/main.cpp )
#add_executable( bin/magik src/ImageMagikTest.cpp )

#add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
#add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )

#find_package(ImageMagick COMPONENTS Magick++)
include_directories( ${OpenCV_INCLUDE_DIRS} )
#include_directories(${ImageMagick_INCLUDE_DIRS})

target_link_libraries( bin/main ${OpenCV_LIBS} )
#target_link_libraries( bin/magik ${ImageMagick_LIBRARIES})
