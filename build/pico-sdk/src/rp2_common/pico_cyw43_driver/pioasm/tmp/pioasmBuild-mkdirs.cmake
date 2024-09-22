# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Users/yoshi/.pico-sdk/sdk/2.0.0/tools/pioasm"
  "C:/Users/yoshi/Documents/Pico/RTOS_setup/build/pioasm"
  "C:/Users/yoshi/Documents/Pico/RTOS_setup/build/pioasm-install"
  "C:/Users/yoshi/Documents/Pico/RTOS_setup/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/tmp"
  "C:/Users/yoshi/Documents/Pico/RTOS_setup/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
  "C:/Users/yoshi/Documents/Pico/RTOS_setup/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src"
  "C:/Users/yoshi/Documents/Pico/RTOS_setup/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/Users/yoshi/Documents/Pico/RTOS_setup/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/Users/yoshi/Documents/Pico/RTOS_setup/build/pico-sdk/src/rp2_common/pico_cyw43_driver/pioasm/src/pioasmBuild-stamp${cfgdir}") # cfgdir has leading slash
endif()
