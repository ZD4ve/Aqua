cmake_minimum_required(VERSION 3.21)
project(Aqua LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#SFML
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

#Gtest
find_package(GTest)
if (NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
endif ()




function(add_compile_options target)
    target_compile_options(${target}
        PRIVATE
            #-Werror
            -Wall
            -Wextra
            -Wpedantic
            #-Wconversion
    )
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${target}
            PRIVATE
                -fsanitize=address
                -fno-omit-frame-pointer
                -g
        )
        target_link_options(${target}
            PRIVATE
                -fsanitize=address
                -g
        )
    endif()
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        target_compile_options(${target}
            PRIVATE
                -Ofast
        )
        target_link_options(${target}
            PRIVATE
                -Ofast
        )
    endif()
endfunction()

add_library(${PROJECT_NAME}_lib)
target_include_directories(${PROJECT_NAME}_lib
    PUBLIC
        inc
)
target_link_libraries(${PROJECT_NAME}_lib
    PUBLIC
        sfml-graphics
)
target_sources(${PROJECT_NAME}_lib
    PRIVATE 
        src/breeder.cpp
        src/color.cpp
        src/engine.cpp
        src/event_handler.cpp
        src/fish.cpp
        src/force.cpp
        src/island.cpp
        src/iter.cpp
        src/net.cpp
)
add_compile_options(${PROJECT_NAME}_lib)


add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
    PRIVATE 
        src/main.cpp
)
add_compile_options(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_NAME}_lib
)

enable_testing() 

add_executable(${PROJECT_NAME}_test)
target_sources(${PROJECT_NAME}_test
    PRIVATE
        test/forcesT.cpp
        test/fishT.cpp

)
add_compile_options(${PROJECT_NAME}_test)
target_link_libraries(${PROJECT_NAME}_test
    PRIVATE
        ${PROJECT_NAME}_lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)