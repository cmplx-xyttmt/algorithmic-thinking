cmake_minimum_required(VERSION 4.1)
project(algorithmic_thinking C)

set(CMAKE_C_STANDARD 11)

#add_executable(algorithmic_thinking
#        food_lines/food_lines.c
#        unique_snowflakes/unique_snowflakes.c
#        include_template.c)
add_executable(food_lines food_lines/food_lines.c)
add_executable(unique_snowflakes unique_snowflakes/unique_snowflakes.c)
