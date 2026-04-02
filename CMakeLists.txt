cmake_minimum_required(VERSION 3.16)

project(lb1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Путь к Qt (должен быть установлен на машине)
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/mingw81_64/lib/cmake") # путь на другой машине

find_package(Qt5 REQUIRED COMPONENTS Core)

set(CMAKE_AUTOMOC ON)

add_executable(lb1
    main.cpp
    folderscanner.h
    folderscanner.cpp
    CryptoManager.h
    CryptoManager.cpp
)

if(WIN32)
    set_target_properties(lb1 PROPERTIES
        WIN32_EXECUTABLE FALSE
    )
    target_link_options(lb1 PRIVATE -mconsole)
endif()

target_link_libraries(lb1 PRIVATE Qt5::Core)

# ============================================
# АВТОМАТИЧЕСКАЯ ЗАГРУЗКА CRYPTO++ ИЗ GITHUB
# ============================================
include(FetchContent)

# 1. Скачиваем CMake-обёртку для Crypto++
FetchContent_Declare(
    cryptopp-cmake
    GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake.git
    GIT_TAG CRYPTOPP_8_9_0  # используйте последнюю стабильную версию
)

# 2. Скачиваем сам Crypto++
FetchContent_Declare(
    cryptopp
    GIT_REPOSITORY https://github.com/weidai11/cryptopp.git
    GIT_TAG CRYPTOPP_8_9_0
)

# Делаем оба доступными
FetchContent_MakeAvailable(cryptopp-cmake cryptopp)

# Копируем CMakeLists.txt из обёртки в папку с исходниками Crypto++
file(COPY ${cryptopp-cmake_SOURCE_DIR}/CMakeLists.txt
     DESTINATION ${cryptopp_SOURCE_DIR})

# Добавляем Crypto++ в сборку
add_subdirectory(${cryptopp_SOURCE_DIR} ${cryptopp_BINARY_DIR})

# Подключаем заголовки и библиотеку
target_include_directories(lb1 PRIVATE ${cryptopp_SOURCE_DIR})
target_link_libraries(lb1 PRIVATE cryptopp-static)

message(STATUS "Crypto++ will be fetched and built automatically")
