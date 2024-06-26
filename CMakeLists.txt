﻿cmake_minimum_required (VERSION 3.14)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

project (bayan VERSION ${PROJECT_VESRION})

set (CMAKE_CXX_STANDARD 17) 

find_package(GTest)
configure_file(version.h.in version.h)

find_package (Boost 1.74 REQUIRED COMPONENTS program_options filesystem system regex )

add_executable (
    bayan
    bayan.cpp
    src/FileScaner.cpp
    src/DuplicateSearch.cpp
    hashFunc/crc32
    hashFunc/md5
    hashFunc/sha1
)
add_executable(bayan_gtest test/bayan_gtest.cpp)

if(Boost_FOUND)
  target_include_directories(bayan PUBLIC ${Boost_INCLUDE_DIRS})
  target_link_libraries (bayan ${Boost_LIBRARIES})
endif()

target_include_directories(
    bayan PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories(
    bayan PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_include_directories(
    bayan PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/hashFunc
)

target_include_directories(
    bayan_gtest PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_include_directories(
    bayan_gtest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/test
)

target_include_directories(
    bayan_gtest PRIVATE
    ${GTEST_INCLUDE_DIRS}
)

target_link_libraries(
    bayan_gtest PRIVATE
    ${GTEST_BOTH_LIBRARIES}
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(bayan PRIVATE 
        #-g
        #-Wall
        #-Wextra
        #-Werror
        #-pedantic
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(bayan PRIVATE
        #/EHsc
        #/W4
        #/WX
    )

endif()

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(bayan_gtest bayan_gtest)
