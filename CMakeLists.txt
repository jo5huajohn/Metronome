cmake_minimum_required(VERSION 3.10)

#set project name
project(Metronome)

#add the executable
add_executable(metronome metronome.c)

# add compile flags
add_compile_options(-Wall -Wextra -pendatntic -Werror)
