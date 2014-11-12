# Copyright: CoDyCo Consortium (2013-..)
# Author: Andrea Del Prete - andrea.delprete@iit.it
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

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

option(COMPILE_AS_SHARED_LIBRARY "Compile ${PROJECT_NAME} as a shared library" TRUE)
option(ENABLE_TESTS "Enable test for paramHelp" FALSE)


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

#### Settings for rpath
if(${CMAKE_MINIMUM_REQUIRED_VERSION} VERSION_GREATER "2.8.12")
    message(AUTHOR_WARNING "CMAKE_MINIMUM_REQUIRED_VERSION is now ${CMAKE_MINIMUM_REQUIRED_VERSION}. This check can be removed.")
endif()
if(NOT (CMAKE_VERSION VERSION_LESS 2.8.12))
    if(NOT MSVC)
        #add the option to disable RPATH
        set(PARAMHELP_DISABLE_RPATH FALSE CACHE BOOL "Disable RPATH installation")
        mark_as_advanced(PARAMHELP_DISABLE_RPATH)
    endif()

    #Configure RPATH
    set(CMAKE_MACOSX_RPATH 1) #enable RPATH on OSX. This also suppress warnings on CMake >= 3.0
    # when building, don't use the install RPATH already
    # (but later on when installing)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
    #build directory by default is built with RPATH
    set(CMAKE_SKIP_BUILD_RPATH  FALSE)

    #This is relative RPATH for libraries built in the same project
    #I assume that the directory is 
    # - install_dir/something for binaries
    # - install_dir/lib for libraries
    file(RELATIVE_PATH _rel_path "${CMAKE_INSTALL_PREFIX}/bin" "${CMAKE_INSTALL_PREFIX}/lib")
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(CMAKE_INSTALL_RPATH "@loader_path/${_rel_path}")
    else()
        set(CMAKE_INSTALL_RPATH "\$ORIGIN/${_rel_path}")
    endif()

    # the RPATH to be used when installing, but only if it's not a system directory (copied form CMake File. To be tested)
    list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
    if("${isSystemDir}" STREQUAL "-1")
       set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
    endif("${isSystemDir}" STREQUAL "-1")

    # add the automatically determined parts of the RPATH
    # which point to directories outside the build tree to the install RPATH
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) #very important!

    if(PARAMHELP_DISABLE_RPATH)
        #what to do? disable RPATH altogether or just revert to the default CMake configuration?
        #I revert to default
        unset(CMAKE_INSTALL_RPATH) #remove install rpath
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
    endif()
endif()
##### end RPATH

set(LIB_TARGET_NAME paramhelp)

add_library(${LIB_TARGET_NAME} ${LIB_TYPE} ${folder_source} ${folder_header})

if(NOT MSVC AND ENABLE_TESTS)
add_executable(uTest src/test/uTest.cpp)
endif()

set_target_properties(${LIB_TARGET_NAME} PROPERTIES VERSION ${${VARS_PREFIX}_VERSION}
                                           SOVERSION ${${VARS_PREFIX}_VERSION}
                                           PUBLIC_HEADER "${folder_header}")
                                           
target_include_directories(${LIB_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>"
                                           "$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/${${VARS_PREFIX}_INSTALL_INCLUDEDIR}>"
                                            ${YARP_INCLUDE_DIRS})

target_link_libraries(${LIB_TARGET_NAME} ${YARP_LIBRARIES})

if(NOT MSVC AND ENABLE_TESTS)
target_link_libraries(uTest ${LIB_TARGET_NAME} ${YARP_LIBRARIES})
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
