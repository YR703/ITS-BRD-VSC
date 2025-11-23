# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
<<<<<<< Updated upstream
  "C:/Users/infwtx851/Downloads/ITS-BRD-VSC-testing/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard"
  "C:/Users/infwtx851/Downloads/ITS-BRD-VSC-testing/Programs/Aufgabe1/tmp/1"
  "C:/Users/infwtx851/Downloads/ITS-BRD-VSC-testing/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard"
  "C:/Users/infwtx851/Downloads/ITS-BRD-VSC-testing/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/tmp"
  "C:/Users/infwtx851/Downloads/ITS-BRD-VSC-testing/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp"
  "C:/Users/infwtx851/Downloads/ITS-BRD-VSC-testing/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src"
  "C:/Users/infwtx851/Downloads/ITS-BRD-VSC-testing/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp"
=======
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe1/tmp/1"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/tmp"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src"
  "C:/wer/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp"
>>>>>>> Stashed changes
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
<<<<<<< Updated upstream
    file(MAKE_DIRECTORY "C:/Users/infwtx851/Downloads/ITS-BRD-VSC-testing/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/infwtx851/Downloads/ITS-BRD-VSC-testing/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
=======
    file(MAKE_DIRECTORY "C:/wer/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/wer/ITS-BRD-VSC/Programs/Aufgabe1/tmp/Aufgabe1.Debug+ITSboard/src/Aufgabe1.Debug+ITSboard-stamp${cfgdir}") # cfgdir has leading slash
>>>>>>> Stashed changes
endif()
