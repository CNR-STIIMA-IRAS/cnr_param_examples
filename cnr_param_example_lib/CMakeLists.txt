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
option(BUILD_UNIT_TESTS "Build the unit tests" ON)
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

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-compile-options.cmake.in"
               "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-compile-options.cmake" @ONLY)
# ##############################################################################
# END OPTIONS                                                                 ##
# ##############################################################################

# ##############################################################################
# DEPENDENCIES                                                                ##
# ##############################################################################
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-dependencies.cmake)

if(BUILD_AS_A_CATKIN_PACKAGE AND (NOT cnr_param_BUILD_AS_A_CATKIN_PACKAGE))
  message(FATAL_ERROR
    "The package cnr_param has been found, but it must be compiled as a CATKIN package as the package cnr_param_example_lib")
elseif((NOT BUILD_AS_A_CATKIN_PACKAGE) AND cnr_param_BUILD_AS_A_CATKIN_PACKAGE)
  message(FATAL_ERROR
    "The package cnr_param has been found, but the package cnr_param_example_lib must be compiled as an CATKIN package as cnr_param")
endif()

if(BUILD_AS_A_CATKIN_PACKAGE)
  list(APPEND DEPENDENCIES_INCLUDE_DIRS   "${catkin_INCLUDE_DIRS}")
  list(APPEND DEPENDENCIES_LINK_LIBRARIES "${catkin_LIBRARIES}")
else()
  list(APPEND DEPENDENCIES_TARGETS      cnr_param::cnr_param)
endif()

if(BUILD_AS_A_CATKIN_PACKAGE)
  catkin_package(
    INCLUDE_DIRS include
    LIBRARIES cnr_param_example_lib
    CATKIN_DEPENDS cnr_param
    CFG_EXTRAS ${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-compile-options.cmake)
endif()
# ##############################################################################
# END DEPENDANCIES        ##
# ##############################################################################

# ##############################################################################
# EXAMPLE
# ##############################################################################
add_library(cnr_param_example_lib SHARED src/cnr_param_example_lib.cpp)

list(APPEND BUILD_INTERFACE_INCLUDE_DIRS "${DEPENDENCIES_INCLUDE_DIRS}"
     "${CMAKE_CURRENT_SOURCE_DIR}/include")
list(APPEND INSTALL_INTERFACE_INCLUDE_DIRS "${DEPENDENCIES_INCLUDE_DIRS}"
     include)
target_include_directories(cnr_param_example_lib PUBLIC 
    PUBLIC
    "$<BUILD_INTERFACE:${BUILD_INTERFACE_INCLUDE_DIRS}>"
    "$<INSTALL_INTERFACE:${INSTALL_INTERFACE_INCLUDE_DIRS}>")
target_link_libraries(cnr_param_example_lib
  PUBLIC ${DEPENDENCIES_TARGETS}
  PUBLIC ${DEPENDENCIES_LINK_LIBRARIES})

add_library(cnr_param_example_lib::cnr_param_example_lib ALIAS cnr_param_example_lib)

list(APPEND TARGETS_LIST cnr_param_example_lib)
# ##############################################################################
# END EXAMPLE
# ##############################################################################

# ##############################################################################
# CONFIGURE AND INSTALL DESTINATIONS                                          ##
# ##############################################################################
if(BUILD_AS_A_CATKIN_PACKAGE)
  set(CNR_PACKAGE_LIB_DESTINATION     "${CATKIN_GLOBAL_LIB_DESTINATION}")
  set(CNR_PACKAGE_BIN_DESTINATION     "${CATKIN_GLOBAL_LIB_DESTINATION}/${PROJECT_NAME}")
  set(CNR_PACKAGE_INCLUDE_DESTINATION "${CATKIN_GLOBAL_INCLUDE_DESTINATION}/${PROJECT_NAME}")
  set(CNR_PACKAGE_CONFIG_DESTINATION  "${CATKIN_GLOBAL_SHARE_DESTINATION}/${PROJECT_NAME}")
else()
  set(CNR_PACKAGE_LIB_DESTINATION "lib")
  set(CNR_PACKAGE_BIN_DESTINATION "bin")
  set(CNR_PACKAGE_INCLUDE_DESTINATION "include")
  set(CNR_PACKAGE_CONFIG_DESTINATION "share/${PROJECT_NAME}/cmake")
endif()
# ##############################################################################
# END - CONFIGURE AND INSTALL DESTINATIONS                                    ##
# ##############################################################################

# ##############################################################################
# CONFIGURE THE PACKAGE                                           ##
# ##############################################################################
if(NOT BUILD_AS_A_CATKIN_PACKAGE)
  cnr_cmake_package_file(cnr_param_example_lib)
endif()
# ##############################################################################
# END - CONFIGURE THE PACKAGE                                           ##
# ##############################################################################

# ##############################################################################
# INSTALL THE PACKAGE                                           ##
# ##############################################################################
install(
  DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CNR_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
  PATTERN ".svn" EXCLUDE)


if(NOT BUILD_AS_A_CATKIN_PACKAGE)
  # ------------------------------------------------------------------------------
  # Install cmake targets files and also namespaced alias targets
  install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${CONFIG_NAMESPACE}"
    DESTINATION "${CNR_PACKAGE_CONFIG_DESTINATION}"
    FILE ${TARGETS_EXPORT_NAME}.cmake)

  # ------------------------------------------------------------------------------
  # Install cmake config files
  install(FILES "${PROJECT_CONFIG_OUTPUT}" "${VERSION_CONFIG}"
                "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${PROJECT_NAME}-dependencies.cmake"
          DESTINATION "${CNR_PACKAGE_CONFIG_DESTINATION}")
endif()
# ##############################################################################
# END INSTALL                                                                 ##
# ##############################################################################
