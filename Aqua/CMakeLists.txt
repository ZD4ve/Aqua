cmake_minimum_required(VERSION 3.14)
project(Aqua LANGUAGES CXX)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
GIT_REPOSITORY https://github.com/SFML/SFML.git
GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE inc)
target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -fsanitize=address
            -fno-omit-frame-pointer
    )
    target_link_options(${PROJECT_NAME}
        PRIVATE
            -fsanitize=address
    )
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Ofast
    )
    target_link_options(${PROJECT_NAME}
        PRIVATE
            -Ofast
    )
endif()
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if(WIN32)
add_custom_command(
    TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)# -Werror
        
install(TARGETS ${PROJECT_NAME})



enable_testing()

#file(GLOB Tests test/*.cpp)

add_executable(
    tests test/force_test.cpp
)
target_include_directories(tests PRIVATE inc)
target_compile_features(tests PRIVATE cxx_std_20)
target_link_libraries(
  tests
  GTest::gtest
  GTest::gtest_main
  sfml-graphics
)

include(GoogleTest)
gtest_discover_tests(tests)
