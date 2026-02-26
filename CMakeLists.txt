cmake_minimum_required(VERSION 3.16)

project(lb1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(lb1
  main.cpp
  folderscanner.h folderscanner.cpp
  CryptoManager.h CryptoManager.cpp
)
target_link_libraries(lb1 PRIVATE Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS lb1
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# === Подключаем библиотеку Crypto++ (статическая линковка) ===
set(CRYPTOPP_INCLUDE_DIR "D:/program_files/Compiler/MSYS2/ucrt64/include")
set(CRYPTOPP_LIBRARY "D:/program_files/Compiler/MSYS2/ucrt64/lib/libcryptopp.a")

if(NOT EXISTS ${CRYPTOPP_INCLUDE_DIR}/cryptopp/cryptlib.h)
    message(FATAL_ERROR "Crypto++ headers not found!")
endif()

if(NOT EXISTS ${CRYPTOPP_LIBRARY})
    message(FATAL_ERROR "Crypto++ library not found!")
endif()

target_include_directories(lb1 PRIVATE ${CRYPTOPP_INCLUDE_DIR})
target_link_libraries(lb1 PRIVATE ${CRYPTOPP_LIBRARY})

# Для MinGW добавляем системные библиотеки
if(WIN32 AND MINGW)
    target_link_libraries(lb1 PRIVATE ws2_32 pthread)
endif()

message(STATUS "Crypto++ found at ${CRYPTOPP_INCLUDE_DIR}")
