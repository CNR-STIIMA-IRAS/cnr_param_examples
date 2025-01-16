############################################
## It does takes inspiration from 
## Ton idea is to have a single file that can be included in the CMakeLists.txt of the package
## and in the >package>Config.cmake
## https://discourse.cmake.org/t/how-to-conditionally-call-either-find-pacakge-or-find-dependency/8175
include(CMakeFindDependencyMacro)

if(${PROJECT_NAME} STREQUAL "cnr_param_example_lib")
  message(STATUS "Loading ''cnr_param_example_lib-dependencies.cmake'' for the project ''${PROJECT_NAME}''")
  macro(_find_package)
    find_package(${ARGN})
  endmacro()
else()
  message(STATUS "Running Project: ${PROJECT_NAME}. Loading cnr_param_example_lib-dependencies.cmake from cnr_param_example_libConfig.cmake")
  include(CMakeFindDependencyMacro)

  macro(_find_package)
    find_dependency(${ARGN})
  endmacro()
endif()

# ##########################################################################################
if(cnr_param_example_lib_BUILD_AS_ABUILD_AS_A_CATKIN_PACKAGE OR BUILD_AS_A_CATKIN_PACKAGE)
  _find_package(catkin REQUIRED COMPONENTS cnr_param)
else()
  _find_package(cnr_param REQUIRED)
endif()

