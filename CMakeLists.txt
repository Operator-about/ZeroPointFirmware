cmake_minimum_required(VERSION 3.15)

project(ZeroPointFirmware-Pi4 ASM C)

set(LINKER_SCRIPT_PI4 "${CMAKE_CURRENT_SOURCE_DIR}/linker-pi4.ld")

set(SOURCES_PI4 ${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/asm/boot-pi4.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/asm/table.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/main-pi4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/GICv2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/SD.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/exFAT.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/UART.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/GPIO.c)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(${PROJECT_NAME}.elf ${SOURCES_PI4})

target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    "-T${LINKER_SCRIPT_PI4}"
    "-nostartfiles"
    "--specs=nosys.specs")
