cmake_minimum_required(VERSION 3.16)

project(lb1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Путь к Qt
set(CMAKE_PREFIX_PATH "D:/program_files/QT_Creator/5.15.2/mingw81_64/lib/cmake")

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
    target_link_options(lb1 PRIVATE -mconsole -Wl,-subsystem,console)
endif()

target_link_libraries(lb1 PRIVATE Qt5::Core)

# === Crypto++ library (ДИНАМИЧЕСКАЯ линковка как в тесте) ===
set(CRYPTOPP_INCLUDE_DIR "D:/program_files/Compiler/MSYS2/ucrt64/include")
set(CRYPTOPP_LIBRARY "D:/program_files/Compiler/MSYS2/ucrt64/lib/libcryptopp.dll.a")  # ИЗМЕНЕНО на .dll.a!

if(EXISTS ${CRYPTOPP_INCLUDE_DIR}/cryptopp/cryptlib.h)
    target_include_directories(lb1 PRIVATE ${CRYPTOPP_INCLUDE_DIR})
    target_link_libraries(lb1 PRIVATE ${CRYPTOPP_LIBRARY})

    # Копирование всех необходимых DLL (как в тесте)
    add_custom_command(TARGET lb1 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "D:/program_files/Compiler/MSYS2/ucrt64/bin/libcryptopp.dll"
            "D:/program_files/Compiler/MSYS2/ucrt64/bin/libgcc_s_seh-1.dll"
            "D:/program_files/Compiler/MSYS2/ucrt64/bin/libstdc++-6.dll"
            "D:/program_files/Compiler/MSYS2/ucrt64/bin/libwinpthread-1.dll"
            $<TARGET_FILE_DIR:lb1>
        COMMENT "Copying all required DLLs to output directory"
    )

    message(STATUS "Crypto++ found from UCRT64: ${CRYPTOPP_LIBRARY}")
else()
    message(FATAL_ERROR "Crypto++ not found! Check path: ${CRYPTOPP_INCLUDE_DIR}")
endif()

# Удалены ws2_32 и pthread — для динамической линковки они не нужны!
