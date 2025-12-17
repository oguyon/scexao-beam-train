cmake_minimum_required(VERSION 3.10)
project(scexao_beam_train C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_executable(scexao_beam_train main.c draw.c)

target_include_directories(scexao_beam_train PRIVATE ${GTK4_INCLUDE_DIRS})
target_link_libraries(scexao_beam_train PRIVATE ${GTK4_LIBRARIES} m)
target_compile_options(scexao_beam_train PRIVATE ${GTK4_CFLAGS_OTHER})
