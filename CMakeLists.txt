# Copyright: CoDyCo Consortium (2013-..)
# Author: Andrea Del Prete - andrea.delprete@iit.it
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.8.11)
project(paramHelp CXX)

set(VARS_PREFIX "paramHelp")

set(${VARS_PREFIX}_MAJOR_VERSION 0)
set(${VARS_PREFIX}_MINOR_VERSION 0)
set(${VARS_PREFIX}_PATCH_VERSION 3)
set(${VARS_PREFIX}_VERSION ${${VARS_PREFIX}_MAJOR_VERSION}.${${VARS_PREFIX}_MINOR_VERSION}.${${VARS_PREFIX}_PATCH_VERSION})

find_package(YCM REQUIRED)

include(YCMDefaultDirs)
ycm_default_dirs(${VARS_PREFIX})


find_package(YARP REQUIRED)

option(COMPILE_AS_SHARED_LIBRARY "Compile ${PROJECT_NAME} as a shared library" TRUE)


SET(folder_source src/paramProxyBasic.cpp
                  src/paramHelperBase.cpp
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
    message(STATUS "Building ${PROJECTNAME} as a shared library is not currently supported on Windows")
    set(COMPILE_AS_SHARED_LIBRARY FALSE)
endif()

if(COMPILE_AS_SHARED_LIBRARY)
    set(LIB_TYPE SHARED)
else()
    set(LIB_TYPE STATIC)
endif()

option(PARAMHELP_ENABLE_RPATH "Enable support to RPATH for paramHelp library" TRUE)
mark_as_advanced(PARAMHELP_ENABLE_RPATH)

include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_PREFIX}/bin"
                          LIB_DIRS "${CMAKE_INSTALL_PREFIX}/lib"
                          DEPENDS PARAMHELP_ENABLE_RPATH
                          USE_LINK_PATH)

set(LIB_TARGET_NAME paramhelp)

add_library(${LIB_TARGET_NAME} ${LIB_TYPE} ${folder_source} ${folder_header})

if(NOT MSVC)
# add_executable(uTest src/test/uTest.cpp)
endif()

set_target_properties(${LIB_TARGET_NAME} PROPERTIES VERSION ${${VARS_PREFIX}_VERSION}
                                           SOVERSION ${${VARS_PREFIX}_VERSION}
                                           PUBLIC_HEADER "${folder_header}")
                                           
target_include_directories(${LIB_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${${VARS_PREFIX}_INSTALL_INCLUDEDIR}>"
                                            ${YARP_INCLUDE_DIRS})

target_link_libraries(${LIB_TARGET_NAME} ${YARP_LIBRARIES})

if(NOT MSVC)
# target_link_libraries(uTest ${LIB_TARGET_NAME} ${YARP_LIBRARIES})
endif()

install(TARGETS paramhelp
        EXPORT paramHelp
        RUNTIME DESTINATION "${${VARS_PREFIX}_INSTALL_BINDIR}" COMPONENT bin
        LIBRARY DESTINATION "${${VARS_PREFIX}_INSTALL_LIBDIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${${VARS_PREFIX}_INSTALL_LIBDIR}" COMPONENT lib
        PUBLIC_HEADER DESTINATION "${${VARS_PREFIX}_INSTALL_INCLUDEDIR}/paramHelp" COMPONENT dev)

set_property(GLOBAL APPEND PROPERTY ${VARS_PREFIX}_TARGETS ${LIB_TARGET_NAME})

include(InstallBasicPackageFiles)
install_basic_package_files(paramHelp   VARS_PREFIX ${VARS_PREFIX}
                                        VERSION ${${VARS_PREFIX}_VERSION}
                                        COMPATIBILITY SameMajorVersion
                                        TARGETS_PROPERTY ${VARS_PREFIX}_TARGETS
                                        NO_CHECK_REQUIRED_COMPONENTS_MACRO)

include(AddUninstallTarget)
