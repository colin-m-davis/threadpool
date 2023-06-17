cmake_minimum_required(VERSION 3.0.0)
project(threadpool VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(PROJECT_ROOT_PATH ${PROJECT_SOURCE_DIR})

add_library(threadpool src/threadpool.cpp)
target_compile_options(threadpool PRIVATE
    -Werror -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
)

add_executable(unit_tests test/unit_tests.cpp)
target_include_directories(unit_tests PRIVATE ${PROJECT_ROOT_PATH}/src)
target_link_libraries(unit_tests threadpool)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
