# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe_3/tmp/Aufgabe_3.Debug+ITSboard"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe_3/tmp/1"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe_3/tmp/Aufgabe_3.Debug+ITSboard"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe_3/tmp/Aufgabe_3.Debug+ITSboard/tmp"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe_3/tmp/Aufgabe_3.Debug+ITSboard/src/Aufgabe_3.Debug+ITSboard-stamp"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe_3/tmp/Aufgabe_3.Debug+ITSboard/src"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe_3/tmp/Aufgabe_3.Debug+ITSboard/src/Aufgabe_3.Debug+ITSboard-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/wer/ITS-BRD-VSC/Programs/Aufgabe_3/tmp/Aufgabe_3.Debug+ITSboard/src/Aufgabe_3.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/wer/ITS-BRD-VSC/Programs/Aufgabe_3/tmp/Aufgabe_3.Debug+ITSboard/src/Aufgabe_3.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
endif()
