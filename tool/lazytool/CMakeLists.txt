project(lazytool)

file(GLOB SRC_LIST *.cpp)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PUBLIC ${SRC_LIST})