# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard")
  file(MAKE_DIRECTORY "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard")
endif()
file(MAKE_DIRECTORY
  "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/1"
  "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard"
  "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/tmp"
  "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src/Aufgabe2.Debug+ITSboard-stamp"
  "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src"
  "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src/Aufgabe2.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src/Aufgabe2.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/infwtx851/GSWS/ITS-BRD-VSC/Programs/Aufgabe2/tmp/Aufgabe2.Debug+ITSboard/src/Aufgabe2.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
