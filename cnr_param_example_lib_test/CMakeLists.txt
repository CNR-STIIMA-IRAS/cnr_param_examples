cmake_minimum_required(VERSION 3.16.0)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/cnrConfigMacros.cmake)

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
# DEPENDENCIES            ##
# ##############################################################################
message(STATUS "ENV{ROS_VERSION}=$ENV{ROS_VERSION}")
if("$ENV{ROS_VERSION}" STREQUAL "1")
  find_package(catkin REQUIRED)
  find_package(cnr_param_example_lib REQUIRED PATHS "${CMAKE_INSTALL_PREFIX}/share/cnr_param/cmake_alternative" NO_DEFAULT_PATH)
elseif("$ENV{ROS_VERSION}" STREQUAL "2")
  find_package(ament_cmake REQUIRED)
  find_package(cnr_param_example_lib REQUIRED PATHS "${CMAKE_INSTALL_PREFIX}/.." NO_DEFAULT_PATH)
else()
  find_package(cnr_param_example_lib REQUIRED NO_MODULE)
endif()

# ##############################################################################
# END DEPENDANCIES        ##
# ##############################################################################

# ##############################################################################
# EXAMPLE
# ##############################################################################
add_executable(mapped_file_module_with_lib
               ${CMAKE_CURRENT_SOURCE_DIR}/src/mapped_file_module_with_lib.cpp)
target_compile_definitions(mapped_file_module_with_lib 
               PRIVATE TEST_DIR="${CMAKE_CURRENT_LIST_DIR}")

target_link_libraries(mapped_file_module_with_lib cnr_param_example_lib::cnr_param_example_lib)
