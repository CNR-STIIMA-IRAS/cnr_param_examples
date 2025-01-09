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
  find_package(cnr_param REQUIRED PATHS "${CMAKE_INSTALL_PREFIX}/share/cnr_param/cmake_alternative" NO_DEFAULT_PATH)
elseif("$ENV{ROS_VERSION}" STREQUAL "2")
  find_package(ament_cmake REQUIRED)
  find_package(cnr_param REQUIRED PATHS "${CMAKE_INSTALL_PREFIX}/.." NO_DEFAULT_PATH)
else()
  find_package(cnr_param REQUIRED NO_MODULE)
endif()

## Catkin is a fake dependency. It is used to test the cmake configuration
if(${catkin_FOUND})
  set(PACKAGE_LIB_DESTINATION     "${CATKIN_GLOBAL_LIB_DESTINATION}")
  set(PACKAGE_BIN_DESTINATION     "${CATKIN_GLOBAL_LIBEXEC_DESTINATION}/${PROJECT_NAME}")
  set(PACKAGE_INCLUDE_DESTINATION "${CATKIN_GLOBAL_INCLUDE_DESTINATION}/${PROJECT_NAME}")
  set(CONFIG_INSTALL_DIR           "share/${PROJECT_NAME}/cmake_alternative")
else()
  set(PACKAGE_LIB_DESTINATION      "${CMAKE_INSTALL_PREFIX}/lib")
  set(PACKAGE_BIN_DESTINATION      "${CMAKE_INSTALL_PREFIX}/bin")
  set(PACKAGE_INCLUDE_DESTINATION  "${CMAKE_INSTALL_PREFIX}/include")
  set(CONFIG_INSTALL_DIR           "share/${PROJECT_NAME}/cmake")
endif()
# ##############################################################################
# END DEPENDANCIES        ##
# ##############################################################################

# ##############################################################################
# EXAMPLE
# ##############################################################################
add_library(cnr_param_example_lib SHARED src/cnr_param_example_lib.cpp)
target_include_directories(cnr_param_example_lib PUBLIC 
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>"
    "$<INSTALL_INTERFACE:include>")
target_link_libraries(cnr_param_example_lib cnr_param::cnr_param)

add_library(cnr_param_example_lib::cnr_param_example_lib ALIAS cnr_param_example_lib)

list(APPEND TARGETS_LIST cnr_param_example_lib)
# ##############################################################################
# END EXAMPLE
# ##############################################################################

# ##############################################################################
# INSTALL         ##
# ##############################################################################
list(APPEND HEADERS_DIRS include)

cnr_install(
  ${extracted_version}
  "${TARGETS_LIST}"
  "${HEADERS_DIRS}"
)
# ##############################################################################
# END INSTALL     ##
# ##############################################################################