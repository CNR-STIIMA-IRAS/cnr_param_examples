cmake_minimum_required(VERSION 3.16.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cnrMacros.cmake)

# ##############################################################################
# PRELIMINARIES           ##
# ##############################################################################
get_project_name("package.xml" extracted_name extracted_version)

project(
  ${extracted_name}
  VERSION ${extracted_version}
  LANGUAGES CXX)

cnr_set_flags()
# use, i.e. don't skip the full RPATH for the build tree
set(CMAKE_SKIP_BUILD_RPATH FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# ##############################################################################
# END - PRELIMINARIES     ##
# ##############################################################################

# ##############################################################################
# OPTIONS                                                                     ##
# ##############################################################################
option(CMAKE_EXPORT_COMPILE_COMMANDS "Export Compile Commands" ON)
option(BUILD_AS_A_CATKIN_PACKAGE "Build the library using CATKIN tools" OFF)
option(BUILD_AS_AN_AMENT_PACKAGE "Build the library using AMENT tools" OFF)

if(BUILD_AS_A_CATKIN_PACKAGE)
  if(BUILD_AS_AN_AMENT_PACKAGE)
    message(
      FATAL_ERROR
        "You selected both the options BUILD_AS_A_CATKIN_PACKAGE and BUILD_AS_AN_AMENT_PACKAGE.\
      Please check your environment and re-run configuration.")
  endif()
  if(NOT $ENV{ROS_VERSION} STREQUAL "1")
    message(FATAL_ERROR "You selected the option BUILD_AS_A_CATKIN_PACKAGE\
       but the env variable ROS_VERSION is '$ENV{ROS_VERSION}'")
  endif()
  find_package(catkin REQUIRED)
elseif(BUILD_AS_AN_AMENT_PACKAGE)
  if(NOT $ENV{ROS_VERSION} STREQUAL "2")
    message(FATAL_ERROR "You selected the option BUILD_AS_AN_AMENT_PACKAGE\
        but the env variable ROS_VERSION is '$ENV{ROS_VERSION}'")
  endif()
  find_package(ament_cmake REQUIRED)
endif()
# ##############################################################################
# END OPTIONS                                                                 ##
# ##############################################################################

# ##############################################################################
# DEPENDENCIES            ##
# ##############################################################################
if(BUILD_AS_A_CATKIN_PACKAGE)
  find_package(catkin REQUIRED COMPONENTS cnr_param_example_lib)
else()
  find_package(cnr_param_example_lib REQUIRED)
endif()

if(BUILD_AS_A_CATKIN_PACKAGE AND (NOT cnr_param_example_lib_BUILD_AS_A_CATKIN_PACKAGE))
  message(FATAL_ERROR
    "The package cnr_param_example_lib has been found, but it must be compiled as a CATKIN package as the package cnr_param_example_lib_test")
elseif((NOT BUILD_AS_A_CATKIN_PACKAGE) AND cnr_param_BUILD_AS_A_CATKIN_PACKAGE)
  message(FATAL_ERROR
    "The package cnr_param_example_lib has been found, but the package cnr_param_example_lib_test must be compiled as an CATKIN package as cnr_param_example_lib")
endif()

if(BUILD_AS_A_CATKIN_PACKAGE)
  list(APPEND DEPENDENCIES_INCLUDE_DIRS   "${catkin_INCLUDE_DIRS}")
  list(APPEND DEPENDENCIES_LINK_LIBRARIES "${catkin_LIBRARIES}")
else()
  list(APPEND DEPENDENCIES_TARGETS cnr_param_example_lib::cnr_param_example_lib)
endif()

if(BUILD_AS_A_CATKIN_PACKAGE)
  catkin_package(
    CATKIN_DEPENDS cnr_param_example_lib
  )
endif()# ##############################################################################
# END DEPENDANCIES        ##
# ##############################################################################

# ##############################################################################
# EXAMPLE
# ##############################################################################
add_executable(mapped_file_module_with_lib
               ${CMAKE_CURRENT_SOURCE_DIR}/src/mapped_file_module_with_lib.cpp)
target_compile_definitions(mapped_file_module_with_lib 
               PRIVATE TEST_DIR="${CMAKE_CURRENT_LIST_DIR}")
target_include_directories(mapped_file_module_with_lib 
               PUBLIC ${DEPENDENCIES_INCLUDE_DIRS})
target_link_libraries(mapped_file_module_with_lib 
  PUBLIC ${DEPENDENCIES_LINK_LIBRARIES}
  PUBLIC ${DEPENDENCIES_TARGETS}
)
