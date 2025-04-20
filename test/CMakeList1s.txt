project(proto_test)

file(GLOB SRC_LIST *.cpp)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PUBLIC boost)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_PATH})

add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})