# Kestrel flight computer firmware

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.1)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico2 CACHE STRING "Board type")

# Kestrel board: RP2354B (48 GPIOs, 2 MB in-package flash), defined in config/kestrel.h.
# Plain (non-cache) variables override the extension's PICO_BOARD line above.
set(PICO_BOARD_HEADER_DIRS ${CMAKE_CURRENT_LIST_DIR}/config)
set(PICO_BOARD kestrel)

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(firmware C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# ---------------------------------------------------------------------------
# Sources: every .c file in the code folders is built automatically.
# New files are picked up on the next build (CONFIGURE_DEPENDS), no edits here needed.
# ---------------------------------------------------------------------------
set(KESTREL_CODE_DIRS config system drivers devices fusion console flight)

set(KESTREL_SOURCES ${CMAKE_CURRENT_LIST_DIR}/main.c)
set(KESTREL_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
foreach(dir ${KESTREL_CODE_DIRS})
    file(GLOB_RECURSE dir_sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/${dir}/*.c)
    list(APPEND KESTREL_SOURCES ${dir_sources})

    # Every folder that holds a header becomes an include path,
    # so #include "i2c1.h" works wherever the header lives.
    file(GLOB_RECURSE dir_headers CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/${dir}/*.h)
    foreach(header ${dir_headers})
        get_filename_component(header_dir ${header} DIRECTORY)
        list(APPEND KESTREL_INCLUDE_DIRS ${header_dir})
    endforeach()
endforeach()
list(REMOVE_DUPLICATES KESTREL_INCLUDE_DIRS)

add_executable(firmware ${KESTREL_SOURCES})

pico_set_program_name(firmware "kestrel")
pico_set_program_description(firmware "Kestrel flight computer")
pico_set_program_version(firmware "0.1")

target_include_directories(firmware PRIVATE ${KESTREL_INCLUDE_DIRS})

# Warnings: catch mistakes early
target_compile_options(firmware PRIVATE -Wall -Wextra)

# PIO programs: any .pio file in the code folders gets its header generated
foreach(dir ${KESTREL_CODE_DIRS})
    file(GLOB_RECURSE pio_files CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/${dir}/*.pio)
    foreach(pio_file ${pio_files})
        pico_generate_pio_header(firmware ${pio_file})
    endforeach()
endforeach()

# stdio over USB (also lets picotool / the Run button reboot the board into BOOTSEL)
pico_enable_stdio_uart(firmware 0)
pico_enable_stdio_usb(firmware 1)

target_link_libraries(firmware
        pico_stdlib
        pico_multicore
        hardware_i2c
        hardware_spi
        hardware_uart
        hardware_pwm
        hardware_adc
        hardware_dma
        hardware_pio
        hardware_interp
        hardware_timer
        hardware_watchdog
        hardware_clocks
        )

pico_add_extra_outputs(firmware)
