# Install script for directory: /export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/export/home/kahlo/aghazanf/Radium-Engine")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3.2.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/export/home/kahlo/aghazanf/Radium-Engine/build-release/assimp-prefix/src/assimp-build/code/libassimp.so.3.2.0"
    "/export/home/kahlo/aghazanf/Radium-Engine/build-release/assimp-prefix/src/assimp-build/code/libassimp.so.3"
    "/export/home/kahlo/aghazanf/Radium-Engine/build-release/assimp-prefix/src/assimp-build/code/libassimp.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3.2.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so.3"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libassimp.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp" TYPE FILE FILES
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/anim.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/ai_assert.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/camera.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/color4.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/color4.inl"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/config.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/defs.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/cfileio.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/light.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/material.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/material.inl"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/matrix3x3.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/matrix3x3.inl"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/matrix4x4.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/matrix4x4.inl"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/mesh.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/postprocess.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/quaternion.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/quaternion.inl"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/scene.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/metadata.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/texture.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/types.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/vector2.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/vector2.inl"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/vector3.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/vector3.inl"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/version.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/cimport.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/importerdesc.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/Importer.hpp"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/DefaultLogger.hpp"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/ProgressHandler.hpp"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/IOStream.hpp"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/IOSystem.hpp"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/Logger.hpp"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/LogStream.hpp"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/NullLogger.hpp"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/cexport.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/Exporter.hpp"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "assimp-dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/assimp/Compiler" TYPE FILE FILES
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/Compiler/pushpack1.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/Compiler/poppack1.h"
    "/export/home/kahlo/aghazanf/Radium-Engine/3rdPartyLibraries/Assimp/code/../include/assimp/Compiler/pstdint.h"
    )
endif()

