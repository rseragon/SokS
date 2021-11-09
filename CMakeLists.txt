cmake_minimum_required(VERSION 3.14...3.21)

include(FindPkgConfig) # pkg-config

project(soks VERSION 0.9 LANGUAGES C)

file(GLOB SokS_SRC "base/*.c")

add_executable(${PROJECT_NAME} ${SokS_SRC})

pkg_check_modules(GRAPHVIZ REQUIRED libgvc)

if(NOT GRAPHVIZ_FOUND)
  message(FATAL_ERROR "GRAPHVIZ library not found")
endif()

find_package(OpenMP)
if(OpenMP_C_FOUND)
  target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_C)
else()
  message(FATAL_ERROR "${PROJECT_NAME} needs OpenMP to work")
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC ${GRAPHVIZ_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC src/ ${GRAPHVIZ_INCLUDE_DIRS} PUBLIC ${PROJECT_BINARY_DIR})

configure_file(${PROJECT_SOURCE_DIR}/base/SoksConfig.h.in ${PROJECT_BINARY_DIR}/SoksConfig.h)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

include(CTest)
enable_testing()

add_test(NAME First_run COMMAND "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")

add_test(NAME Print_version COMMAND "${CMAKE_BINARY_DIR}/${PROJECT_NAME}" "--version")
set_tests_properties(Print_version PROPERTIES PASS_REGULAR_EXPRESSION
  "SokS version ${PROJECT_VERSION}\n")
