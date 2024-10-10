set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)
project(physical_layer C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()
add_executable(main
        main.c
  slave.c
  master.c
  lib/list.c
  lib/debug.c
)

target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

# Uncomment if you want to flash your board via picotool.
pico_add_extra_outputs(main)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)

target_link_libraries(main
  pico_stdlib
  # Uncomment when using Pico W
  # pico_cyw43_arch_none
  hardware_spi
  pico_sync
)