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
# ##############################################################################
# END - PRELIMINARIES     ##
# ##############################################################################

# ##############################################################################
# OPTIONS                                                                     ##
# ##############################################################################
option(CMAKE_EXPORT_COMPILE_COMMANDS "Export Compile Commands" ON)
option(BUILD_AS_A_CATKIN_PACKAGE "Build as a catkin package" OFF)

if(BUILD_AS_A_CATKIN_PACKAGE)
  if(NOT "$ENV{ROS_VERSION}" STREQUAL "1")
    message(FATAL_ERROR
      "You selected CATKIN compilation option, but ROS environment is not sourced.")
  endif()
endif()

# ##############################################################################
# END OPTIONS                                                                 ##
# ##############################################################################

# ##############################################################################
# DEPENDENCIES                                                                ##
# ##############################################################################
if(cnr_param_example_lib_BUILD_AS_A_CATKIN_PACKAGE OR BUILD_AS_A_CATKIN_PACKAGE)
  find_package(catkin REQUIRED COMPONENTS cnr_param)
else()
  find_package(cnr_param REQUIRED)
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
  set(CNR_PACKAGE_INCLUDE_DESTINATION "${CATKIN_GLOBAL_INCLUDE_DESTINATION}")
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
  DESTINATION ${CNR_PACKAGE_INCLUDE_DESTINATION}/${PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
  PATTERN ".svn" EXCLUDE)

install(
    TARGETS cnr_param_example_lib
    EXPORT "${TARGETS_EXPORT_NAME}"
    ARCHIVE DESTINATION ${CNR_PACKAGE_LIB_DESTINATION}
    LIBRARY DESTINATION ${CNR_PACKAGE_LIB_DESTINATION}
    RUNTIME DESTINATION ${CNR_PACKAGE_BIN_DESTINATION}
)

if(NOT BUILD_AS_A_CATKIN_PACKAGE)
  # ------------------------------------------------------------------------------
  # Install cmake targets files and also namespaced alias targets
  install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    NAMESPACE "${CONFIG_NAMESPACE}"
    DESTINATION "${CNR_PACKAGE_CONFIG_DESTINATION}/${PROIECT_NAME}"
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
