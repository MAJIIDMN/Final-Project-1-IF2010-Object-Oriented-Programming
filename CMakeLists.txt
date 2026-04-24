cmake_minimum_required(VERSION 3.16)

project(NIMONSPOLY VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Output to ./bin (kept consistent across platforms)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Multiconfig generators (Visual Studio/Xcode) use per-config output dirs.
if(CMAKE_CONFIGURATION_TYPES)
  foreach(cfg ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER "${cfg}" cfg_upper)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfg_upper} "${CMAKE_SOURCE_DIR}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${cfg_upper} "${CMAKE_SOURCE_DIR}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${cfg_upper} "${CMAKE_SOURCE_DIR}/bin")
  endforeach()
endif()

# Sources
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(game ${PROJECT_SOURCES})

target_include_directories(game PRIVATE "${CMAKE_SOURCE_DIR}/include")

if(MSVC)
  target_compile_options(game PRIVATE /W4)
else()
  target_compile_options(game PRIVATE -Wall -Wextra)
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(RAYLIB REQUIRED IMPORTED_TARGET raylib)
target_link_libraries(game PRIVATE PkgConfig::RAYLIB)
target_compile_definitions(game PRIVATE NIMONSPOLY_ENABLE_RAYLIB=1)

add_custom_target(run
  COMMAND $<TARGET_FILE:game>
  DEPENDS game
  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
