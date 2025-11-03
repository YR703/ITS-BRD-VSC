# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard"
  "D:/ITS-BRD-VSC/Programs/Aufgabe1/tmp/1"
  "D:/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard"
  "D:/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/tmp"
  "D:/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp"
  "D:/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src"
  "D:/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
