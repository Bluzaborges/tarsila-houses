cmake_minimum_required(VERSION 3.21)

project(tarsila-houses VERSION 1.0.0 LANGUAGES CXX)

find_package(SDL2 CONFIG REQUIRED)

set(TARSILA_HOUSES_SOURCES
    main.cpp
    include/App.h
    include/Circle.h
    include/Color.h
    include/CommandLine.h
    include/Config.h
    include/House.h
    include/Point.h
    include/Primitives.h
    include/Rectangle.h
    include/Shape.h
    include/Sun.h
    include/Triangle.h
    src/App.cpp
    src/CommandLine.cpp
    src/Config.cpp
    src/Primitives.cpp
)

add_executable(tarsila-houses ${TARSILA_HOUSES_SOURCES})

source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}"
    FILES ${TARSILA_HOUSES_SOURCES}
)

target_compile_features(tarsila-houses PRIVATE cxx_std_17)
target_include_directories(tarsila-houses PRIVATE include)
target_link_libraries(tarsila-houses PRIVATE SDL2::SDL2)

add_custom_command(
    TARGET tarsila-houses
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/examples"
        "$<TARGET_FILE_DIR:tarsila-houses>/examples"
    VERBATIM
)

if(MSVC)
    target_compile_options(tarsila-houses PRIVATE /W4)
else()
    target_compile_options(tarsila-houses PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(WIN32)
    add_custom_command(
        TARGET tarsila-houses
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL2::SDL2>
            $<TARGET_FILE_DIR:tarsila-houses>
        VERBATIM
    )
endif()
