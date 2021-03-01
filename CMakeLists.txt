cmake_minimum_required(VERSION 3.17)
project(GAME_sea_battle C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(
        GAME_sea_battle
        src/data.c
        src/main.c
        src/menu.c
        src/network.c
        src/render.c
        src/seabattle.h
)

if(WIN32)
    target_link_libraries(GAME_sea_battle wsock32 ws2_32)
elseif(WIN64)
    target_link_libraries(GAME_sea_battle wsock64 ws2_64)
endif()