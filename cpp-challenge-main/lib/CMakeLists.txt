project(reactor LANGUAGES CXX)

file(GLOB_RECURSE ${PROJECT_NAME}_source_files *.cpp *.h)

add_library(${PROJECT_NAME} ${${PROJECT_NAME}_source_files})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:include>)

add_library(bisect::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} bisect::project_warnings bisect::project_options)

install(TARGETS ${PROJECT_NAME})
