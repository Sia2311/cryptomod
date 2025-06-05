cmake_minimum_required(VERSION 3.10)
project(CryptoMod)

set(CMAKE_CXX_STANDARD 17)

# Пути вывода
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugins)

# Главный бинарник
add_executable(cryptomod
    main.cpp
    generator.cpp
    inputOutput.cpp
    module_load.cpp
    utils.cpp
)

# Плагины (без префикса lib)
add_library(xor SHARED xor.cpp)
set_target_properties(xor PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
)

add_library(skitala SHARED skitala.cpp)
set_target_properties(skitala PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
)

add_library(codeword SHARED codeWord.cpp)
set_target_properties(codeword PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
)

# Установка бинарника
install(TARGETS cryptomod DESTINATION /opt/CryptoMod)

# Установка плагинов
install(FILES
    ${PLUGIN_OUTPUT_DIR}/xor.so
    ${PLUGIN_OUTPUT_DIR}/skitala.so
    ${PLUGIN_OUTPUT_DIR}/codeword.so
    DESTINATION /opt/CryptoMod/plugins
)

# Иконка и ярлык
install(FILES
    icon.png
    CryptoMod.desktop
    DESTINATION /opt/CryptoMod
)

# postinst скрипт
install(PROGRAMS ${CMAKE_SOURCE_DIR}/postinst DESTINATION /DEBIAN)

# CPack конфигурация
include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_NAME "cryptomod")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGING_INSTALL_PREFIX "/")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_SOURCE_DIR}/postinst")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "YourName")
include(CPack)
