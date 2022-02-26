add_executable(Units utests.c ${PROJECT_SOURCE_DIR}/base/graph.c)

pkg_check_modules(GRAPHVIZ REQUIRED libgvc)

target_link_libraries(Units PUBLIC ${GRAPHVIZ_LIBRARIES})
target_include_directories(Units PUBLIC ${PROJECT_SOURCE_DIR}/base ${PROJECT_SOURCE_DIR}/tests ${GRAPHVIZ_INCLUDE_DIRS})

# For the unit test to access it
configure_file(${PROJECT_SOURCE_DIR}/tests/test_graph.dot ${PROJECT_BINARY_DIR}/tests/test_graph.dot)

add_test(NAME UnitTests COMMAND Units)
