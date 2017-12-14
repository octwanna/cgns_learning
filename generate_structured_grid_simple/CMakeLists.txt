project (generate_structured_grid_simple)

file (GLOB_RECURSE ${PROJECT_NAME}_sources ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/*.c)

add_executable (${PROJECT_NAME} ${${PROJECT_NAME}_sources})
