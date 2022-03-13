cmake_minimum_required(VERSION 3.10)

# debug
# set(CMAKE_BUILD_TYPE Debug)

# set project name
project(Metronome)

# add the executable
add_executable(metronome metronome.c)

# add compile flags
add_compile_options(-Wall -Wextra -pendatntic -Werror)
