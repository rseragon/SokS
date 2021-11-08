cmake_minimum_required(VERSION 3.10)

include(FindPkgConfig) # pkg-config

project(soks VERSION 0.8 LANGUAGES C)

file(GLOB SokS_SRC "base/*.c")

add_executable(${PROJECT_NAME} ${SokS_SRC})

pkg_check_modules(GRAPHVIZ REQUIRED libgvc)

if(NOT GRAPHVIZ_FOUND)
  message(FATAL_ERROR "GRAPHVIZ library not found")
endif()

find_package(OpenMP)
if(OpenMP_C_FOUND)
  target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_C)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${GRAPHVIZ_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC src/ ${GRAPHVIZ_INCLUDE_DIRS})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
