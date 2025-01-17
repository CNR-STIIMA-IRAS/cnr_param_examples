############################################
## It does takes inspiration from 
## Ton idea is to have a single file that can be included in the CMakeLists.txt of the package
## and in the >package>Config.cmake
## https://discourse.cmake.org/t/how-to-conditionally-call-either-find-pacakge-or-find-dependency/8175
include(CMakeFindDependencyMacro)

if(${PROJECT_NAME} STREQUAL "cnr_param_example_lib")
  macro(_find_package)
    find_package(${ARGN})
  endmacro()
else()
  include(CMakeFindDependencyMacro)

  macro(_find_package)
    find_dependency(${ARGN})
  endmacro()
endif()

# message(STATUS "The 'find_package' will look for modules under the following paths: 
#           CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH};
#           CMAKE_FRAMEWORK_PATH=${CMAKE_FRAMEWORK_PATH};
#           CMAKE_APPBUNDLE_PATH=${CMAKE_APPBUNDLE_PATH};
#           CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH};
#           CMAKE_SYSTEM_PREFIX_PATH=${CMAKE_SYSTEM_PREFIX_PATH};
#           CMAKE_SYSTEM_FRAMEWORK_PATH=${CMAKE_SYSTEM_FRAMEWORK_PATH};
#           CMAKE_SYSTEM_APPBUNDLE_PAT=${CMAKE_SYSTEM_APPBUNDLE_PATH}")


# ##########################################################################################
if(cnr_param_example_lib_BUILD_AS_ABUILD_AS_A_CATKIN_PACKAGE OR BUILD_AS_A_CATKIN_PACKAGE)
  _find_package(catkin REQUIRED COMPONENTS cnr_param)
else()
  find_package(cnr_param REQUIRED)
endif()

