project(demo LANGUAGES CXX)

file(GLOB_RECURSE ${PROJECT_NAME}_source_files *.cpp *.h)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_source_files})

target_link_libraries(${PROJECT_NAME} bisect::reactor)

install(TARGETS ${PROJECT_NAME})
