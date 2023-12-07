cmake_minimum_required(VERSION 3.22)
message(STATUS "CMake version is ${CMAKE_VERSION}")
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(FATAL_ERROR "In-source builds are not allowed")
endif()

# Default to `release` build if nothing was specified
# if(NOT CMAKE_BUILD_TYPE)
#   message(STATUS "No build type selected, default to release")
#   set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type")
# endif()

cmake_host_system_information(RESULT SYS_HOST_NAME QUERY HOSTNAME)
cmake_host_system_information(RESULT SYS_OS_NAME QUERY OS_NAME)
cmake_host_system_information(RESULT SYS_PHY_CORES QUERY NUMBER_OF_PHYSICAL_CORES)

message(STATUS "HOST name: ${SYS_HOST_NAME}")
message(STATUS "OS name: ${SYS_OS_NAME}")
message(STATUS "Physical cores: ${SYS_PHY_CORES}")
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  message(STATUS "Target is 64 bits")
else()
  message(STATUS "Target is 32 bits")
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#[=[
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported) # - Executed too long
if(ipo_supported)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()
message(STATUS "Interprocedual optimization: ${CMAKE_INTERPROCEDURAL_OPTIMIZATION}")
#]=]

# Set global properties
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


add_library(compiler_props INTERFACE)
target_compile_options(compiler_props
    INTERFACE
        # $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic -Werror>
        # $<$<CXX_COMPILER_ID:MSVC>:/W3 /WX /wd4668>) # 3rd level warning and higher, Werror
)


macro(add_day_project my_prj)
    project(${my_prj} CXX)
    add_executable(${PROJECT_NAME} ${my_prj}.cpp)
    message(STATUS "PROJECT_NAME = ${PROJECT_NAME}")
    message(STATUS "CMAKE_CURRENT_LIST_DIR = ${CMAKE_CURRENT_LIST_DIR}")

    if(MSVC)
        message(STATUS "MSVC compiler")
        target_compile_options(${PROJECT_NAME} PUBLIC "/ZI")
        target_link_options(${PROJECT_NAME} PUBLIC "/INCREMENTAL")
    endif()
    
    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

endmacro()

add_day_project(day1)
