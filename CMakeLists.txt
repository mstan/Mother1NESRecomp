cmake_minimum_required(VERSION 3.20)
project(Mother1NESRecomp C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(NESRECOMP_ROOT ${CMAKE_SOURCE_DIR}/nesrecomp)

# nesrecomp/ and recomp-ui/ are git submodules (see .gitmodules); the pinned
# commit is the gitlink recorded in this repo. After cloning run setup.sh/.bat
# (git submodule update --init) to fetch them at the recorded commits.

include(${NESRECOMP_ROOT}/runner/runner.cmake)

list(APPEND CMAKE_PREFIX_PATH "${NESRECOMP_ROOT}/runner/external/SDL2/cmake")
find_package(SDL2 REQUIRED)

file(GLOB MOTHER1_GEN CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/generated/mother1_full*.c"
    "${CMAKE_SOURCE_DIR}/generated/mother1_dispatch.c"
)

add_executable(Mother1NESRecomp
    ${NESRECOMP_RUNNER_SOURCES}
    extras.c
    nes_snapshot.c
    verify_mode.c
    watchdog.c
    ${MOTHER1_GEN}
)

target_compile_definitions(Mother1NESRecomp PRIVATE
    NESRECOMP_SPLIT_PARTS_EXTERNAL
    RECOMP_STACK_TRACKING
    WATCHDOG_ENABLED
)

target_include_directories(Mother1NESRecomp PRIVATE
    ${NESRECOMP_RUNNER_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(Mother1NESRecomp PRIVATE ${SDL2_LIBRARIES})

if(WIN32)
    target_link_libraries(Mother1NESRecomp PRIVATE ws2_32)
endif()

if(WIN32)
    add_custom_command(TARGET Mother1NESRecomp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${NESRECOMP_ROOT}/runner/external/SDL2/lib/x64/SDL2.dll"
            "$<TARGET_FILE_DIR:Mother1NESRecomp>"
    )
endif()

if(MSVC)
    target_compile_options(Mother1NESRecomp PRIVATE /W3 /WX-)
    target_compile_options(Mother1NESRecomp PRIVATE /wd4102)
endif()

# ---- Pre-boot GUI launcher (recomp-ui / Dear ImGui) ----
include(${CMAKE_SOURCE_DIR}/recomp-ui/recomp_ui.cmake)
recomp_target_launcher_ui(Mother1NESRecomp BOXART ${CMAKE_SOURCE_DIR}/boxart.tga)

# Mother 1 is single-player and uses battery-backed cartridge SRAM.
target_compile_definitions(Mother1NESRecomp PRIVATE
    NESRECOMP_GAME_PLAYERS=1
    NESRECOMP_GAME_BATTERY)
