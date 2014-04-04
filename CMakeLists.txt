# Copyright: CoDyCo Consortium (2013-..)
# Author: Andrea Del Prete - andrea.delprete@iit.it
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

SET(PROJECTNAME paramHelp)

PROJECT(${PROJECTNAME})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include
                    ${YARP_INCLUDE_DIRS} )

ADD_LIBRARY(${PROJECTNAME} ${folder_source} ${folder_header})

TARGET_LINK_LIBRARIES(${PROJECTNAME} ${YARP_LIBRARIES})

icub_export_library(${PROJECTNAME} INTERNAL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
                                   DEPENDS 
                                   DESTINATION include/paramHelp
                                   FILES ${folder_header})

cmake_minimum_required(VERSION 2.8.11)
project(paramHelp CXX)

set(VARS_PREFIX "paramHelp")

set(${VARS_PREFIX}_MAJOR_VERSION 0)
set(${VARS_PREFIX}_MINOR_VERSION 0)
set(${VARS_PREFIX}_PATCH_VERSION 1)
set(${VARS_PREFIX}_VERSION ${${VARS_PREFIX}_MAJOR_VERSION}.${${VARS_PREFIX}_MINOR_VERSION}.${${VARS_PREFIX}_PATCH_VERSION})

find_package(YCM REQUIRED)
include(YCMDefaultDirs)
ycm_default_dirs(${VARS_PREFIX})


find_package(YARP REQUIRED)


SET(folder_source src/paramHelperBase.cpp
                  src/paramHelperClient.cpp
                  src/paramHelperServer.cpp
                  src/paramHelpUtil.cpp)

SET(folder_header include/paramHelp/paramHelpUtil.h
                  include/paramHelp/paramProxyInterface.h
                  include/paramHelp/paramProxyBasic.h
                  include/paramHelp/paramHelperBase.h
                  include/paramHelp/paramHelperClient.h
                  include/paramHelp/paramHelperServer.h)

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

if(MSVC)
    message(STATUS "Building "${PROJECTNAME}" as a shared library is not currently supported on Windows")
    set(SHARED_LIBRARY FALSE)
endif()

if(SHARED_LIBRARY)
    set(LIB_TYPE SHARED)
else()
    set(LIB_TYPE STATIC)
endif()

set(LIB_TARGET_NAME paramhelp)

add_library(${LIB_TARGET_NAME} ${LIB_TYPE} ${folder_source} ${folder_header})

set_target_properties(${LIB_TARGET_NAME} PROPERTIES VERSION ${${VARS_PREFIX}_VERSION}
                                           SOVERSION ${${VARS_PREFIX}_VERSION}
                                           PUBLIC_HEADER "${folder_header}")
                                           
target_include_directories(${LIB_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${${VARS_PREFIX}_INSTALL_INCLUDEDIR}>"
                                            ${YARP_INCLUDE_DIRS})
                                            
target_link_libraries(${LIB_TARGET_NAME} ${YARP_LIBRARIES})

install(TARGETS paramhelp
        EXPORT paramHelp
        RUNTIME DESTINATION "${${VARS_PREFIX}_INSTALL_BINDIR}" COMPONENT bin
        LIBRARY DESTINATION "${${VARS_PREFIX}_INSTALL_LIBDIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${${VARS_PREFIX}_INSTALL_LIBDIR}" COMPONENT lib
        PUBLIC_HEADER DESTINATION "${${VARS_PREFIX}_INSTALL_INCLUDEDIR}/paramHelp" COMPONENT dev)

set_property(GLOBAL APPEND PROPERTY ${VARS_PREFIX}_TARGETS idyntree)

include(InstallBasicPackageFiles)
install_basic_package_files(paramHelp   VARS_PREFIX ${VARS_PREFIX}
                                        VERSION ${${VARS_PREFIX}_VERSION}
                                        COMPATIBILITY SameMajorVersion
                                        TARGETS_PROPERTY ${VARS_PREFIX}_TARGETS
                                        NO_CHECK_REQUIRED_COMPONENTS_MACRO)

include(AddUninstallTarget)
