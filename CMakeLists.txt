# Generated Cmake Pico project file

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
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# set(CMAKE_BUILD_TYPE "Debug")
# set(PICO_DEOPTIMIZED_DEBUG 1)


# Set any variables required for importing FreeRTOS librarie
SET(FREERTOS_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS)
message("FreeRTOS Kernel located in ${FREERTOS_PATH}")


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)
# include FreeRTOS cmake
include(${FREERTOS_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(embarcatech-freertos-tarefa-1 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(embarcatech-freertos-tarefa-1
        main.c
        lib_ext/ssd1306_i2c.c
        src/app_semaphores.c
        src/buttons.c
        src/buzzer.c
        src/led_rgb.c
        src/oled/display.c
        src/oled/i2c.c
        )

pico_set_program_name(embarcatech-freertos-tarefa-1 "embarcatech-freertos-tarefa-1")
pico_set_program_version(embarcatech-freertos-tarefa-1 "0.1")

# no_flash means the target is to run from RAM
# pico_set_binary_type(embarcatech-freertos-tarefa-1 no_flash)

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(embarcatech-freertos-tarefa-1 1)
pico_enable_stdio_usb(embarcatech-freertos-tarefa-1 1)

# Add the standard library to the build
target_link_libraries(embarcatech-freertos-tarefa-1
        pico_stdlib
        hardware_i2c
        hardware_pwm
        FreeRTOS-Kernel-Heap4
        )

# Add the standard include files to the build
target_include_directories(embarcatech-freertos-tarefa-1 PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include
)

# Add any user requested libraries
target_link_libraries(embarcatech-freertos-tarefa-1 
        
        )

pico_add_extra_outputs(embarcatech-freertos-tarefa-1)

# === GENERATE AUTOMATIC DOCUMENTATION (OPTIONAL) ===

# Creates an option for the user to decide whether to generate documentation
# option(GENERATE_DOCUMENTATION "Generate project documentation using Doxygen" OFF)
option(GENERATE_DOCUMENTATION "Generate project documentation using Doxygen" ON)

# Try to find Doxygen installed
find_package(Doxygen)

if (DOXYGEN_FOUND AND GENERATE_DOCUMENTATION)
    message(STATUS "Doxygen found. Configuring documentation generation...")

    # Defines the configuration files
    set(DOXYGEN_IN  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Creates the documentation target
    add_custom_target(doc_doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating documentation with Doxygen..."
        VERBATIM
    )
    
    # Makes the documentation be generated only after compiling the project
    add_dependencies(embarcatech-freertos-tarefa-1 doc_doxygen)
else()
    if(GENERATE_DOCUMENTATION)
        message(WARNING "Doxygen not found! Documentation will not be generated")
    endif()
endif()