# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Uni/Master/S2_SS24/BTD_VU/ESP32_VSC_Project/v5.2.1/esp-idf/components/bootloader/subproject"
  "C:/Uni/Master/S2_SS24/BTD_VU/BTD_Ass2/screen/build/bootloader"
  "C:/Uni/Master/S2_SS24/BTD_VU/BTD_Ass2/screen/build/bootloader-prefix"
  "C:/Uni/Master/S2_SS24/BTD_VU/BTD_Ass2/screen/build/bootloader-prefix/tmp"
  "C:/Uni/Master/S2_SS24/BTD_VU/BTD_Ass2/screen/build/bootloader-prefix/src/bootloader-stamp"
  "C:/Uni/Master/S2_SS24/BTD_VU/BTD_Ass2/screen/build/bootloader-prefix/src"
  "C:/Uni/Master/S2_SS24/BTD_VU/BTD_Ass2/screen/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Uni/Master/S2_SS24/BTD_VU/BTD_Ass2/screen/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Uni/Master/S2_SS24/BTD_VU/BTD_Ass2/screen/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
