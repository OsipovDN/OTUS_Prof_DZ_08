cmake_minimum_required (VERSION 3.8)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

find_package (Boost REQUIRED COMPONENTS  filesystem  
program_options)
find_package_handle_standard_args(Boost DEFAULT_MSG
     ${Boost_INCLUDE_DIRS}
    )

project (bayan VERSION ${PROJECT_VESRION})
find_package(GTest)
configure_file(version.h.in version.h)

add_executable (
    bayan
    bayan.cpp
)
add_executable(bayan_gtest test/bayan_gtest.cpp)

target_link_libraries (bayan Boost::boost Boost::program_options Boost::filesystem)  

target_include_directories(
    bayan PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_include_directories(
    bayan PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
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
set_target_properties(bayan PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(bayan PRIVATE 
        -g
        -Wall
        -Wextra
        -Werror
        -pedantic
    )

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(bayan PRIVATE
        /EHsc
        /W4
        /WX
    )

endif()
