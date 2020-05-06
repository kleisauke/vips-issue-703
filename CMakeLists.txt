cmake_minimum_required(VERSION 3.11)
project(vips-issue-703)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Find libvips (required)
find_package(PkgConfig)
pkg_check_modules(VIPS vips-cpp>=8.9 REQUIRED)

add_executable(test-single test-single.cpp)

target_include_directories(test-single
        PRIVATE
            ${VIPS_INCLUDE_DIRS}
        )

target_link_libraries(test-single
        PRIVATE
            ${VIPS_LDFLAGS}
        )

add_executable(test-anim test-anim.cpp)

target_include_directories(test-anim
        PRIVATE
            ${VIPS_INCLUDE_DIRS}
        )

target_link_libraries(test-anim
        PRIVATE
            ${VIPS_LDFLAGS}
        )
