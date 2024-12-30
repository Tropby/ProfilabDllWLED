cmake_minimum_required(VERSION 3.10.0)
project(ProfiLabDllWLED VERSION 0.1.0)

include (dep/profilabDll/profilabDll.cmake)

add_library(ProfiLabDllWLED SHARED src/profiLabDllWLED.cpp)

target_link_libraries(ProfiLabDllWLED ${EBCPP_LIBS} -lWs2_32)

target_compile_definitions(ProfiLabDllWLED PRIVATE WIN_EXPORT)