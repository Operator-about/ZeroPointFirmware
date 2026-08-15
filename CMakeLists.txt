cmake_minimum_required(VERSION 3.15)

project(ZeroPointFirmware-Pi4 ASM C)


set(LINKER_SCRIPT_PI4 "${CMAKE_CURRENT_SOURCE_DIR}/linker-pi4.ld")

set(SOURCES_PI4 ${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/asm/boot-pi4.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/aarch64/asm/table.asm
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/main-pi4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/GIC.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/SD/SD-Standart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/SD/SD.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/FS/MBR.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/FS/exFAT-PhysicalLayer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/FS/exFAT-KernelLayer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/UART/PL011.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/source/GPIO.c)

set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -I${CMAKE_CURRENT_SOURCE_DIR}")
add_executable(${PROJECT_NAME}.elf ${SOURCES_PI4})

target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/SD)
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/FS)
target_include_directories(${PROJECT_NAME}.elf PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/UART)

target_link_options(${PROJECT_NAME}.elf PRIVATE
    "-T${LINKER_SCRIPT_PI4}"
    "-nostartfiles"
    "--specs=nosys.specs")
