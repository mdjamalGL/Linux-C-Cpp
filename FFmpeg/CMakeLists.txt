cmake_minimum_required(VERSION 3.10)
project(dplay C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-g")

find_package(SDL2 REQUIRED)
find_package(ffmpeg REQUIRED)

add_executable(dplay Play.c)

target_link_libraries(dplay PRIVATE
    ffmpeg::avformat
    ffmpeg::avcodec
    ffmpeg::swscale
    ffmpeg::avutil
    SDL2::SDL2
)