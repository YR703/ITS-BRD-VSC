# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/Aufgabe3.Debug+ITSboard")
  file(MAKE_DIRECTORY "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/Aufgabe3.Debug+ITSboard")
endif()
file(MAKE_DIRECTORY
  "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/1"
  "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/Aufgabe3.Debug+ITSboard"
  "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/Aufgabe3.Debug+ITSboard/tmp"
  "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/Aufgabe3.Debug+ITSboard/src/Aufgabe3.Debug+ITSboard-stamp"
  "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/Aufgabe3.Debug+ITSboard/src"
  "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/Aufgabe3.Debug+ITSboard/src/Aufgabe3.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/Aufgabe3.Debug+ITSboard/src/Aufgabe3.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/GSWS/ITS-BRD-VSC/Programs/Aufgabe3/tmp/Aufgabe3.Debug+ITSboard/src/Aufgabe3.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
