project (CGNSFile)

include_directories (${CMAKE_SOURCE_DIR}/include)

file (GLOB_RECURSE ${PROJECT_NAME}_sources ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/source/*.c)

add_library (${PROJECT_NAME} ${${PROJECT_NAME}_sources})
